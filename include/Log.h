#ifndef LOG_H
#define LOG_H

#include <iostream>

namespace Snake
{
	void Log(const std::string& message);
	void LogError(const std::string& errorMessage);
}

#endif
