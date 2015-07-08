/*
 * protocol.h
 *
 *  Created on: 2015年6月29日
 *      Author: jk
 */

#ifndef SRC_PROTOCOL_H_
#define SRC_PROTOCOL_H_

#define START_FLAG (0xAA55)

#define RSP_MSG_OFFSET (0x80)
enum
{
	CMD_GET_DEVICE = 0x1,

	CMD_GET_DEVICE_RSP = CMD_GET_DEVICE + RSP_MSG_OFFSET,
};



#endif /* SRC_PROTOCOL_H_ */
