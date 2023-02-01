#include "RsBuffer.h"

SmartBuffer::SmartBuffer(CallBack callback) {
	m_callBack = callback;
	for (uint8_t i = 0; i < MAXBUFFER; i++) {
		packet[i] = 0;
	}
		bytesInbuffer = 0;
		receiving = false;
}

void SmartBuffer::Update() {
		while (Serial.available() > 0) {
			//for (uint8_t x = 0; x < bytes; x++) {
				uint8_t rb = (uint8_t)Serial.read();
				Serial.print((char)rb);
				
				if (rb == '#') {
					bytesInbuffer = 0;
					receiving = true;
				}
				else if (receiving) {
					packet[bytesInbuffer] = rb;
					bytesInbuffer++;
				}
				// one pcaket in memory buffer  // TODO check summ and packet length in packet
				if (bytesInbuffer >= PACKETLENGHT) {
					receiving = false;
					if (m_callBack != NULL) {
						m_callBack(packet, bytesInbuffer-1);
					}
					bytesInbuffer = 0;
				}
			//}
		}
		//else { return; }
	}
