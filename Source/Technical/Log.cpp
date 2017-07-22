/*
 * Log.cpp
 *
 *  Created on: May 13, 2017
 *      Author: josh
 */

#include "Log.h"

void info(std::string text) {
	std::cout << "[INFO] " << text << std::endl;
}
void warn(std::string text) {
	std::cout << "[WARN] " << text << std::endl;
}
void error(std::string text) {
	std::cerr << "[ERROR] " << text << std::endl;
}
