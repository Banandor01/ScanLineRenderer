#pragma once

#include <Arduino.h>

#define MAXBUFFER 64
#define PACKETLENGHT 8

typedef void(*CallBack)(uint8_t *, uint8_t received);

class SmartBuffer {
public:
	SmartBuffer(CallBack callback);
	void Update();	
private:
	uint8_t packet[MAXBUFFER];
	uint8_t bytesInbuffer;
	bool receiving;
	CallBack m_callBack;
};