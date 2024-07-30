/**
 ******************************************************************************
 * @file    protocol.c
 * @brief   野火PID调试助手通讯协议解析
 ******************************************************************************
 */
#include "protocol.h"
#include <string.h>

/*协议帧解析结构体*/
struct prot_frame_parser_t {
  uint8_t *recv_ptr;  /*数据接收数组*/
  uint16_t r_oft;     /*读偏移*/
  uint16_t w_oft;     /*写偏移*/
  uint16_t frame_len; /*帧长度*/
  uint16_t found_frame_head;
};

/*定义一个协议帧解析结构体*/
static struct prot_frame_parser_t parser;
/*定义一个接收缓冲区*/
static uint8_t recv_buf[PROT_FRAME_LEN_RECV];
/*定义一个输出借口*/
UART_HandleTypeDef *debug_huart;


/**
 * @brief   初始化接收协议
 * @return  初始化结果.
 */
int32_t protocol_init(UART_HandleTypeDef *huart) {
  debug_huart = huart;
  /*全局变量parser清空*/
  memset(&parser, 0, sizeof(struct prot_frame_parser_t));

  /* 初始化分配数据接收与解析缓冲区*/
  parser.recv_ptr = recv_buf;

  return 0;
}

/**
 * @brief 计算校验和
 * @param init:校验和初始值
 * @param ptr:需要计算的数据
 * @param len:需要计算的长度
 * @retval 校验和
 */
uint8_t check_sum(uint8_t init, const uint8_t *ptr, uint8_t len) {
  /*校验和的计算结果*/
  uint8_t sum = init;

  while (len--) {
    sum += *ptr;  // 依次累加各个数据的值
    ptr++;
  }

  return sum;
}

/**
 * @brief   获取帧类型（帧命令）
 * @param   buf: 数据缓冲区
 * @param   head_oft: 帧头的偏移位置
 * @return  帧类型（帧命令）
 */
static uint8_t get_frame_type(const uint8_t *buf, uint16_t head_oft) {
  /*计算“帧命令”在帧数据中的位置*/
  uint16_t cmdIndex = head_oft + CMD_INDEX_VAL;

  return (buf[cmdIndex % PROT_FRAME_LEN_RECV] & 0xFF);
}

/**
 * @brief   获取帧长度
 * @param   buf: 数据缓冲区
 * @param   head_oft: 帧头的偏移位置
 * @return  帧长度.
 */
static uint16_t get_frame_len(const uint8_t *buf, const uint16_t head_oft) {
  /*计算“帧长度”在帧数据中的位置*/
  uint16_t lenIndex = head_oft + LEN_INDEX_VAL;

  return ((buf[(lenIndex + 0) % PROT_FRAME_LEN_RECV] << 0) |
          (buf[(lenIndex + 1) % PROT_FRAME_LEN_RECV] << 8) |
          (buf[(lenIndex + 2) % PROT_FRAME_LEN_RECV] << 16) |
          (buf[(lenIndex + 3) % PROT_FRAME_LEN_RECV] << 24));  // 合成帧长度
}

/**
 * @brief   获取crc-16校验值
 * @param   buf:  数据缓冲区.
 * @param   head_oft: 帧头的偏移位置
 * @param   frame_len: 帧长
 * @return  校验值
 */
static uint8_t get_frame_checksum(const uint8_t *buf, uint16_t head_oft,
                                  const uint16_t frame_len) {
  /*计算“校验和”在帧数据中的位置*/
  uint16_t crcIndex = head_oft + frame_len - 1;

  return (buf[crcIndex % PROT_FRAME_LEN_RECV]);
}

/**
 * @brief   查找帧头
 * @param   buf:  数据缓冲区.
 * @param   ring_buf_len: 缓冲区大小（常量，如128）
 * @param   start: 起始位置（读偏移）
 * @param   len: 需要查找的长度
 * @return  -1:没有找到帧头，其他值:帧头的位置.
 */
static int32_t recvbuf_find_header(const uint8_t *buf, const uint16_t ring_buf_len,
                                   const uint16_t start, const uint16_t len) {
  /*帧头是4字节，从0查找到len-4，逐个比对*/
  for (uint16_t i = 0; i < (len - 3); i++) {
    if (((buf[(start + i + 0) % ring_buf_len] << 0) |
         (buf[(start + i + 1) % ring_buf_len] << 8) |
         (buf[(start + i + 2) % ring_buf_len] << 16) |
         (buf[(start + i + 3) % ring_buf_len] << 24)) ==
        FRAME_HEADER) /*0x59485A53*/
    {
      return ((start + i) % ring_buf_len);
    }
  }
  return -1;
}

/**
 * @brief   计算未解析的数据的长度
 * @param   frame_len: 帧长度（数据中记录的帧长度）
 * @param   ring_buf_len: 缓冲区大小（常量，如128）
 * @param   start: 起始位置（读偏移）
 * @param   end: 结束位置（写偏移）
 * @return  未解析的数据长度
 */
static int32_t recvbuf_get_len_to_parse(const uint16_t frame_len,
                                        const uint16_t ring_buf_len,
                                        const uint16_t start, const uint16_t end) {
  uint16_t unparsed_data_len; /*未解析的数据长度*/

  // 读偏移<=写偏移，说明数据在环形缓存区中是连续存储的
  if (start <= end) {
    unparsed_data_len = end - start;
  }
  // 否则，数据被分成了两部分，一部分在缓冲区结尾，一部分在缓冲区开头
  else {
    // 缓冲区结尾处的长度 + 缓冲区开头处处的长度
    unparsed_data_len = (ring_buf_len - start) + end;
  }

  if (frame_len > unparsed_data_len) {
    /*数据中记录的帧长度 > 未解析的数据长度*/
    return 0;
  } else {
    return unparsed_data_len;
  }
}

/**
 * @brief   接收数据写入缓冲区
 * @param   buf:  数据缓冲区.
 * @param   ring_buf_len: 缓冲区大小（常量，如128）
 * @param   w_oft: 写偏移
 * @param   data: 需要写入的数据
 * @param   data_len: 需要写入数据的长度
 * @return  void.
 */
void recvbuf_put_data(uint8_t *buf, const uint16_t ring_buf_len, uint16_t w_oft,
                      const uint8_t *data, const uint16_t data_len) {
  /*要写入的数据超过了缓冲区尾*/
  if ((w_oft + data_len) > ring_buf_len) {
    /*计算缓冲区剩余长度*/
    uint16_t data_len_part = ring_buf_len - w_oft;

    /*数据分两段写入缓冲区*/
    memcpy((buf + w_oft), data, data_len_part); /*先将一部分写入缓冲区尾*/
    memcpy(buf, (data + data_len_part),
           (data_len - data_len_part)); /*再将剩下的覆盖写入缓冲区头*/
  } else {
    memcpy(buf + w_oft, data, data_len); /*直接将整个数据写入缓冲区*/
  }
}

/**
 * @brief   协议帧解析
 * @param   data: 返回解析出的帧数据
 * @param   data_len: 返回帧数据的大小
 * @return  帧类型（命令）
 */
uint8_t protocol_frame_parse(uint8_t *data, uint16_t *data_len) {
  uint8_t frame_type = CMD_NONE; /*帧类型*/
  uint8_t checksum = 0;          /*校验和*/
  /*计算未解析的数据的长度*/
  uint16_t need_to_parse_len = recvbuf_get_len_to_parse(
      parser.frame_len, PROT_FRAME_LEN_RECV, parser.r_oft, parser.w_oft);
  if (need_to_parse_len < 9) {
    /*数据太少，肯定还不能同时找到帧头和帧长度*/
    return frame_type;
  }

  /*还未找到帧头，需要进行查找*/
  if (parser.found_frame_head == 0) {
    /* 帧头偏移,
       同步头为四字节，可能存在未解析的数据中最后一个字节刚好为同步头第一个字节的情况，
       因此查找同步头时，最后一个字节将不解析，也不会被丢弃*/
    int16_t header_oft = recvbuf_find_header(
        parser.recv_ptr, PROT_FRAME_LEN_RECV, parser.r_oft, need_to_parse_len);
    if (header_oft >= 0) {
      /* 已找到帧头*/
      parser.found_frame_head = 1;
      parser.r_oft = header_oft;

      /* 确认是否可以计算帧长*/
      if (recvbuf_get_len_to_parse(parser.frame_len, PROT_FRAME_LEN_RECV,
                                   parser.r_oft, parser.w_oft) < 9) {
        return frame_type;
      }
    } else {
      /* 未解析的数据中依然未找到帧头，丢掉此次解析过的所有数据*/
      parser.r_oft =
          ((parser.r_oft + need_to_parse_len - 3) % PROT_FRAME_LEN_RECV);
      return frame_type;
    }
  }

  /* 计算帧长，并确定是否可以进行数据解析*/
  if (0 == parser.frame_len) {
    parser.frame_len = get_frame_len(parser.recv_ptr, parser.r_oft);
    if (need_to_parse_len < parser.frame_len) {
      return frame_type;
    }
  }

  /* 帧头位置确认，且未解析的数据超过帧长，可以计算校验和*/
  if ((parser.frame_len + parser.r_oft - PROT_FRAME_LEN_CHECKSUM) >
      PROT_FRAME_LEN_RECV) {
    /* 数据帧被分为两部分，一部分在缓冲区尾，一部分在缓冲区头 */
    checksum = check_sum(checksum, parser.recv_ptr + parser.r_oft,
                         PROT_FRAME_LEN_RECV - parser.r_oft);
    checksum = check_sum(checksum, parser.recv_ptr,
                         parser.frame_len - PROT_FRAME_LEN_CHECKSUM +
                             parser.r_oft - PROT_FRAME_LEN_RECV);
  } else {
    /* 数据帧可以一次性取完*/
    checksum = check_sum(checksum, parser.recv_ptr + parser.r_oft,
                         parser.frame_len - PROT_FRAME_LEN_CHECKSUM);
  }

  if (checksum ==
      get_frame_checksum(parser.recv_ptr, parser.r_oft, parser.frame_len)) {
    /* 校验成功，拷贝整帧数据 */
    if ((parser.r_oft + parser.frame_len) > PROT_FRAME_LEN_RECV) {
      /* 数据帧被分为两部分，一部分在缓冲区尾，一部分在缓冲区头*/
      uint16_t data_len_part = PROT_FRAME_LEN_RECV - parser.r_oft;
      memcpy(data, parser.recv_ptr + parser.r_oft, data_len_part);
      memcpy(data + data_len_part, parser.recv_ptr,
             parser.frame_len - data_len_part);
    } else {
      /* 数据帧可以一次性取完*/
            memcpy(data, parser.recv_ptr + parser.r_oft, parser.frame_len);
        }
        *data_len = parser.frame_len;
        frame_type = get_frame_type(parser.recv_ptr, parser.r_oft);

        /* 丢弃缓冲区中的命令帧*/
        parser.r_oft = (parser.r_oft + parser.frame_len) % PROT_FRAME_LEN_RECV;
    }
    else
    {
        /* 校验错误，说明之前找到的帧头只是偶然出现的废数据*/
        parser.r_oft = (parser.r_oft + 1) % PROT_FRAME_LEN_RECV;
    }
    parser.frame_len = 0;
    parser.found_frame_head = 0;

    return frame_type;
}

/**
 * @brief   接收到的数据写入缓冲区
 * @param   data:  接收到的数据的数组.
 * @param   data_len: 接收到的数据的大小
 * @return  void.
 */
void protocol_data_recv(const uint8_t *data, const uint16_t data_len)
{
	/*数据写入缓冲区*/
    recvbuf_put_data(parser.recv_ptr, PROT_FRAME_LEN_RECV, parser.w_oft, data, data_len);

	/*计算写偏移*/
    parser.w_oft = (parser.w_oft + data_len) % PROT_FRAME_LEN_RECV;
}


/**
  * @brief 设置上位机的值
  * @param cmd:命令
  * @param ch: 曲线通道
  * @param data:参数指针
  * @param num:参数个数
  * @retval 无
  */
void set_computer_value(uint8_t cmd, uint8_t ch, void *data, uint8_t num)
{
	static packet_head_t set_packet;

	uint8_t sum = 0;    // 校验和
	num *= 4;           // 一个参数 4 个字节

	set_packet.head = FRAME_HEADER;     // 包头 0x59485A53
	set_packet.ch   = ch;              // 设置通道
	set_packet.len  = 0x0B + num;      // 包长
	set_packet.cmd  = cmd;             // 设置命令

	sum = check_sum(0, (uint8_t *)&set_packet, sizeof(set_packet));       // 计算包头校验和
	sum = check_sum(sum, (uint8_t *)data, num);                           // 计算参数校验和

	usart_send((uint8_t *)&set_packet, sizeof(set_packet));    // 发送数据头
	usart_send((uint8_t *)data, num);                          // 发送参数
	usart_send((uint8_t *)&sum, sizeof(sum));                  // 发送校验和
}

void usart_send(const uint8_t* data, const uint16_t size) {
  HAL_UART_Transmit(debug_huart, data, size, 0xFFFF);
}
