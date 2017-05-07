// ConsoleProtocolDevelopment.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stm_protocol.h"

int main()
{
	uint8_t data[16] = { 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00 };
	struct stm_transfer_packet myFrame;

	myFrame.start_byte = CMD_JPEG;
	myFrame.data_crc = 0x0000;
	myFrame.data_len = 0x0010;
	myFrame.pData = data;

	stm_packet_details(myFrame);

    return 0;
}

