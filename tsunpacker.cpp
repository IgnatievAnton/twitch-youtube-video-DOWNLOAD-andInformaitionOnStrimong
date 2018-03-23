/*
 * MpegtTS Basic Parser
 * Copyright (c) jeoliva, All rights reserved.

 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3.0 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 *License along with this library.
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <fstream>
#include <string>
#include "bitreader.h"
#include "tsparser.h"
#include <iostream>
#include <fstream>
#include <Windows.h>

using namespace std;
// Main method
int main(int argc, char *argv[])
{
	int bytes_read;
	uint8_t packet_buffer[TS_PACKET_SIZE];
	int n_packets = 0;

	TSParser tsParser;
	memset(&tsParser, 0, sizeof(TSParser));

	std::string urlLinc = argv[1];
	std::string fileName = argv[2];

	std::cout << urlLinc << "  " << fileName << std::endl;
	std::cin.get();
	ofstream file1;
	file1.open("name.txt", ios_base::out | ios_base::trunc);
	file1 << fileName;
	file1.close();
	ofstream file2;
	file1.open("url.txt", ios_base::out | ios_base::trunc);
	file1 << urlLinc;
	file1.close();
	string syscol = "script.bat";
	system(syscol.c_str());
	std::ifstream file(fileName, std::ios_base::binary);
	if (file.fail())
	{
		printf("Error opening the stream\nSyntax: tsunpacket FileToParse.ts\n");
		return -1;
	}

	// Parse file while we can read full TS packets
	while (file.read(reinterpret_cast<char *>(packet_buffer), TS_PACKET_SIZE))
	{
		bytes_read = file.gcount();

		if (packet_buffer[0] == TS_DISCONTINUITY)
		{
			printf("Discontinuity detected!\n");
			signalDiscontinuity(&tsParser, 0);
		}
		else if (bytes_read < TS_PACKET_SIZE)
		{
			printf("End of file!\n");
			break;
		}
		else if (packet_buffer[0] == TS_SYNC)
		{
			ABitReader bitReader;
			initABitReader(&bitReader, packet_buffer, bytes_read);

			parseTSPacket(&tsParser, &bitReader);

			n_packets++;
		}
	}

	printf("Number of packets found: %d\n", n_packets);

	// Freeing resources
	file.close();
	freeParserResources(&tsParser);
	std::cin.get();
	return 0;
}
