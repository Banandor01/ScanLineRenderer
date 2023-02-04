#pragma once
#include <iostream>

class ILogger {
public:
	virtual void Init() = 0;
	virtual void Log(const char* msg, unsigned int level = 1) = 0;
};

class ConsoleLogger : public ILogger {
public:
	void Log(const char* msg, unsigned int level = 1) override {
		std::cout << msg << std::endl;
	}
};