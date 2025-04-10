/*
Logger.hpp --- Logging utilities
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

#ifndef _LOGGER_HPP_
#define _LOGGER_HPP_

#include <string>

/**
 * @brief Class for logging messages
 */
class Logger
{
public:
    /* Getters and setters */
    const std::string& GetName() const noexcept;
    void SetName(const std::string& CsName) noexcept;
    const std::string& GetLogPath() const noexcept;
    void SetLogPath(const std::string CsLogPath) noexcept;

    /**
     * @brief Construct a new Logger object and erases the contents of CsPath if it exists
     *
     * @param CsName the name of the logger
     * @param CsPath the path to write the log
     */
    Logger(const std::string& CsName, const std::string& CsPath);

    /**
     * @brief Log a message at the DEBUG level
     *
     * @param CsMessage the message to log
     */
    void Debug(const std::string& CsMessage) const;

    /**
     * @brief Log a message at the ERROR level
     *
     * @param CsMessage the message to log
     */
    void Error(const std::string& CsMessage) const;

    /**
     * @brief Log a message at the INFO level
     *
     * @param CsMessage the message to log
     */
    void Info(const std::string& CsMessage) const;

    /**
     * @brief Log a message at the TRACE level
     *
     * @param CsMessage the message to log
     */
    void Trace(const std::string& CsMessage) const;

    /**
     * @brief Log a message at the WARN level
     *
     * @param CsMessage the message to log
     */
    void Warn(const std::string& CsMessage) const;

private:
    typedef enum {DEBUG, ERROR, INFO, TRACE, WARN} LogLevel;    /**< Log level types */

    friend std::ostream& operator <<(std::ostream& ostreamOut, const LogLevel ClogLevel) noexcept;

    std::string _sName;     /**< The name of the logger */
    std::string _sLogPath;  /**< The path of the log file */

    /**
     * @brief Logs a message with a certain log level
     *
     * @param CsMessage the message to log
     * @param logLevel the log level type of the message
     */
    void Log(const std::string& CsMessage, LogLevel logLevel) const;
};


inline const std::string& Logger::GetName() const noexcept { return _sName; }
inline void Logger::SetName(const std::string& CsName) noexcept { _sName = CsName; }
inline const std::string& Logger::GetLogPath() const noexcept { return _sLogPath; }
inline void Logger::SetLogPath(const std::string CsLogPath) noexcept { _sLogPath = CsLogPath; }


inline void Logger::Debug(const std::string& CsMessage) const { Log(CsMessage, LogLevel::DEBUG); }
inline void Logger::Error(const std::string& CsMessage) const { Log(CsMessage, LogLevel::ERROR); }
inline void Logger::Info(const std::string& CsMessage) const { Log(CsMessage, LogLevel::INFO); }
inline void Logger::Trace(const std::string& CsMessage) const { Log(CsMessage, LogLevel::TRACE); }
inline void Logger::Warn(const std::string& CsMessage) const { Log(CsMessage, LogLevel::WARN); }


/* Stream insertion operator overload for log level */
std::ostream& operator <<(std::ostream& ostreamOut, const Logger::LogLevel ClogLevel) noexcept;

#endif
