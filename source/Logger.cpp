/*
Logger.cpp --- Logging utilities
Copyright (C) 2025  Juan de la Cruz Caravaca Guerrero (Quadraxis_v2)
juan.dlcruzcg@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/


#include <string>
#include <ctime>
#include <sstream>
#include <chrono>
#include <cstdio>
#include <fstream>
#include <ios>
#include <ostream>

#include "../include/Logger.hpp"

/**
 * @brief Construct a new Logger object and erases the contents of CsPath if it exists
 *
 * @param CsName the name of the logger
 * @param CsPath the path to write the log
 */
Logger::Logger(const std::string& CsName, const std::string& CsPath) : _sName{CsName},
	_sLogPath{CsPath} {}


/**
 * @brief Logs a message with a certain log level
 *
 * @param CsMessage the message to log
 * @param logLevel the log level type of the message
 */
void Logger::Log(const std::string& CsMessage, LogLevel logLevel) const
{
	// Get local system time
	std::time_t timeNow{std::time(nullptr)};
	//std::time_t timeNow{std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())};
	std::tm* pTmLocalTime{std::localtime(&timeNow)};

	// Build the log message
	std::ostringstream ossLog{};
	ossLog << std::put_time(pTmLocalTime, "[%d-%m-%Y %H:%M:%S] ") << logLevel << " " << _sName << 
		" - " << CsMessage;

	// Log to standard error output
	std::fprintf(stderr, "%s\r", ossLog.str().c_str());

	// Log to a file in external storage
    std::ofstream ofstreamLog{_sLogPath, std::ios_base::out | std::ios_base::app};

    if (!ofstreamLog) throw std::ios_base::failure("Error opening file " + _sLogPath);

	ofstreamLog << ossLog.str() << std::endl;
	ofstreamLog.close();
}


/**
 * @brief Stream insertion operator overload for log level
 */
std::ostream& operator <<(std::ostream& ostreamOut, const Logger::LogLevel ClogLevel) noexcept
{
	switch (ClogLevel)
	{
	case Logger::DEBUG:
		return ostreamOut << "DEBUG";
		break;
	case Logger::ERROR:
		return ostreamOut << "ERROR";
		break;
	case Logger::INFO:
		return ostreamOut << "INFO";
		break;
	case Logger::TRACE:
		return ostreamOut << "TRACE";
		break;
	case Logger::WARN:
		return ostreamOut << "WARN";
		break;
	default:
		return ostreamOut;
		break;
	}
}
