/*****************************************************************************
* @file    stm_protocol.c
* @author  George Vigelette
* @version V1.0.0
* @date    May 5, 2017
* @brief   Contains the implementation of the serial command processor.
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

/* Includes ------------------------------------------------------------------*/
#include "stm_protocol.h"

#include <string.h>
#include <malloc.h>
#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

static unsigned short crc16(const unsigned char* data_p, int length);
static void dump_hex(uint8_t* byteData, int length);

/* Private functions ---------------------------------------------------------*/

static unsigned short crc16(const unsigned char* data_p, int length) {
	unsigned char x;
	unsigned short crc = 0xFFFF;

	/*
	printf("\nCalculating CRC on DATA:\n");
	dump_hex(data_p, length);
	*/

	while (length--) {
		x = crc >> 8 ^ *data_p++;
		x ^= x >> 4;
		crc = (crc << 8) ^ ((unsigned short)(x << 12)) ^ ((unsigned short)(x << 5)) ^ ((unsigned short)x);
	}
	return crc;
}

static void dump_hex(uint8_t* byteData, int length) {

	uint8_t* pData;
	int x = 0;

	pData = byteData;

	if (pData == 0) {
		printf("No Data\n\n");
	}
	else {
		printf("Dumping %d Bytes\n", length);
		for (x = 1; x <= length; x++) {
			printf("0x%02X", *pData);
			if (x % 16 == 0) {
				printf("\n");
			}
			else {
				printf(" ");
			}

			pData++;
		}

		printf("\n\n");
	}
}

static uint8_t* _stm_packet_toBuffer(struct stm_transfer_packet pPacket) {
	uint16_t data_len = 0;
	uint8_t* serialBuffer = 0;
	uint8_t* pBuffer = 0;
	uint8_t* pCrcVal = 0;
	uint8_t* pCrcData = 0;

	serialBuffer = (uint8_t*)malloc(pPacket.data_len + 5);
	if (serialBuffer == 0) {
		/* failed to allocate memory buffer */
		printf("_stm_packet_toBuffer: Failed to allocate memory");
		return 0;
	}

	/* set pointer to buffer */
	pBuffer = serialBuffer;
	*pBuffer = pPacket.start_byte;
	pBuffer++;

	pCrcVal = pBuffer; /* save  location of crc for later*/
	pBuffer += 2;

	pCrcData = pBuffer; /* save  location of crc data start for later*/
	*(uint16_t*)pBuffer = pPacket.data_len;
	pBuffer += 2;

	/* add data */
	if (pPacket.data_len > 0) {
		if (pPacket.pData != 0) {
			memcpy(pBuffer, pPacket.pData, pPacket.data_len);
		}
		else {
			/* error invalid pointer data */
			printf("_stm_packet_toBuffer: Invalid pointer to data");
		}
	}
	else {
		/* no data */
	}

	/* crc generations and save to stream*/
	pPacket.data_crc = crc16(pCrcData, pPacket.data_len + 2);
	*(uint16_t*)pCrcVal = pPacket.data_crc;

	return serialBuffer;
}

void stm_packet_details(struct stm_transfer_packet sendFrame) {
	uint16_t datacount = 0;
	uint8_t* pData;

	datacount = sendFrame.data_len;

	printf("\n===> STM Packet Details: <===\n");
	printf("Start Byte: 0x%02X\n", sendFrame.start_byte);
	printf("Data Length:  0x%04X\n", sendFrame.data_len);
	printf("CRC 16: 0x%04X\n", sendFrame.data_crc);

	/* data bytes */

	printf("\nData:\n\n");
	pData = (uint8_t*)sendFrame.pData;
	if (datacount > 0)
		dump_hex(sendFrame.pData, datacount);
	else
		printf("\nNo Data Bytes\n\n");

	printf("===> End STM Packet Details: <===\n\n");
}

bool send_stm_packet(struct stm_transfer_packet sendFrame) {
	bool bRet = true;
	uint8_t* pBuffer;
	
	stm_packet_details(sendFrame);

	pBuffer = _stm_packet_toBuffer(sendFrame);
	if (pBuffer != 0) {

		printf("Created buffer successfully\n\n");
		dump_hex(pBuffer, sendFrame.data_len + 5);

		/* transmit buffer */

		/* free buffer */
		free(pBuffer);
		pBuffer = 0;
	}
	return bRet;
}
