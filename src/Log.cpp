#include <Log.h>
#include <iostream>
#include <string>

namespace Snake
{
	void Log(const std::string& message)
	{
		std::cout << message << std::endl;
	}

	void LogError(const std::string& errorMessage)
	{
		std::cout << "[ERROR] " << errorMessage << std::endl;
	}
}