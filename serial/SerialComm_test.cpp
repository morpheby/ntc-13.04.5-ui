/*
 * SerialComm_test.cpp
 *
 *  Created on: 23.07.2013
 *      Author: morpheby
 */

#include "platform.h"

#include "SerialComm.h"

#include <iostream>
#include <algorithm>
#include <iterator>
#include <memory>

#include "Log.h"
#include "Logger.h"

void write_ping(comm::SerialComm &serial) {
	std::string dummy;

	std::cout << "Enter device address to try sending pre-formatted PING request" << std::endl;
	uint16_t addr = 0;
	std::cin >> addr;
	std::vector<uint16_t> rdbuf(20);

	uint8_t xorS = 0xA5 ^ addr ^ 0x1A ^ 0xAB ^ 0x00;

	std::vector<uint16_t> writeBuf = {
			((1<<8) | addr), // Communication request
			((1<<8) | addr),
			addr,						// Target address
			0x1A,						// PING + NOACK
			0xAB,						// That's us ;)
			0x00,						// No data
			xorS,
			0x00,						// No data
			0x100						// Communication termination
	};
	std::cout << "Flushing read buffer..." << std::endl;
	serial.flushRead();
	std::cout << "Sending ";
	std::copy(writeBuf.begin(), writeBuf.end(), std::ostream_iterator<uint16_t>(std::cout, " "));
	std::cout << " to device " << addr << "..." << std::endl;
	serial.write(writeBuf.begin(), writeBuf.end());
}

void blind_read(comm::SerialComm &serial) {
	std::vector<uint16_t> rdbuf(20);
	std::string dummy;

	std::cout << "Send some data over serial port (like ready message) from the device" << std::endl;
	serial.read(rdbuf.begin(), rdbuf.end());

	std::cout << std::hex;
	std::copy(rdbuf.begin(), rdbuf.end(), std::ostream_iterator<uint16_t>(std::cout, " "));
	std::cout << std::endl;


	std::cout << "OK. Please, check the data. If it is correct, press enter; "
			  << "otherwise press Ctrl+C" << std::endl;
	std::getline(std::cin, dummy);
}

int main(int argc, char **argv) {
	std::string port, dummy;

	auto logger = std::make_shared<util::Logger>();
	logger->addLog(std::make_shared<util::LogFile>("ntpc-comm", util::LogSeverity::TRACE, "ntpc-comm.log"));
	logger->addLog(std::make_shared<util::LogStream>("ntpc-comm", util::LogSeverity::ERROR, std::cerr.rdbuf()));

	try {
		std::cout << "Utility reads and writes raw data to/from serial 9-bit port. To start, "
				  << "enter tty device path: " << std::endl;
		std::getline(std::cin, port);

		comm::SerialComm serial(port);

		bool exit = false;
		do {
			std::cout << "1. Read data" << std::endl
					  << "2. Write ping reauest" << std::endl
					  << "3. Exit" << std::endl;
			int choice;
			std::cin >> choice;
			switch (choice) {
			case 1:
				blind_read(serial);
				break;
			case 2:
				write_ping(serial);
				break;
			case 3:
				exit = true;
				break;
			}
		} while(!exit);


		std::getline(std::cin, dummy);
	} catch(const util::posix_error_exception &e) {
		logger->logPosixError(e.getErrno(), e.getWhile());
	} catch(const std::exception &e) {
		logger->logException(e);
	} catch(const char *str) {
		logger->logException(str);
	} catch(const std::string &s) {
		logger->logException(s);
	}


	return 0;
}
