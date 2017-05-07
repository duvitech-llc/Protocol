#pragma once
/******************************************************************************
* @file    stm_protocol.h
* @author  George Vigelette
* @version V1.0.0
* @date    May 7, 2017
* @brief   This file contains the headers for the serial command processor.
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2017 Duvitech </center></h2>
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
******************************************************************************
*/


/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef STM_SERIAL_COMMAND_PROC_H_
#define STM_SERIAL_COMMAND_PROC_H_

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>
#include <stdbool.h>

/* Exported types ------------------------------------------------------------*/

enum enumCommandByte {
	CMD_ACK = 0x79,
	CMD_NACK = 0x1F,
	CMD_JPEG = 0xBA,
};

enum enumErrorCodes {
	ERR_CRC = 0x01,
	ERR_LEN = 0x02,
	ERR_HPG = 0x03,
};

struct stm_transfer_packet {
	uint8_t 	start_byte;
	uint8_t 	reserved[3];
	uint16_t 	data_crc;
	uint16_t 	data_len;
	uint8_t* 	pData;
};

struct stm_error_packet {
	uint8_t 	start_byte; /* should be a nack for error */
	uint8_t 	error_code;
	uint16_t 	pad;
	uint8_t* 	pErrStr;
};


/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void stm_packet_details(struct stm_transfer_packet sendFrame);
bool send_stm_packet(struct stm_transfer_packet sendFrame);

#endif /* STM_SERIAL_COMMAND_PROC_H_ */