/*
 * Log.h
 *
 *  Created on: May 13, 2017
 *      Author: josh
 */

#ifndef SOURCE_TECHNICAL_LOG_H_
#define SOURCE_TECHNICAL_LOG_H_

#include <iostream>
#include <string>

/**
 * @defgroup logging
 * It's supposed to be used for logging, but I don't really use it.
 * @{
 */

/**
 * Output some text to stdout with the
 * prefix [INFO].
 * @param text Text to output.
 */
void info(std::string text);
/**
 * Output some text to stdout with the
 * prefix [WARN].
 * @param text Text to output.
 */
void warn(std::string text);
/**
 * Output some text to stderr with the
 * prefix [ERROR].
 * @param text Text to output.
 */
void error(std::string text);

#endif /* SOURCE_TECHNICAL_LOG_H_ */
