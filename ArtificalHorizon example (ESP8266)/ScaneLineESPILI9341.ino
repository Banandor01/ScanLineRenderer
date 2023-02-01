
#include "../ScaneLineRenderer/scaneLineRenderer.h"
#include "driver/Adafruit_ILI9341_fast_as.h"
#include "Touch_ESP_ILI9341/XPT2046.h"

//#include "ArtificalHorizon.h"
//#include "altimeter.h"
#include "airspeedindicator.h"

#include <ESP8266WiFi.h> 

#include "RsBuffer.h"

#define NOTOUCH

// Simple constructor. You must use the following pins:
//  ILI9341 pin --> ESP8266 pin  MOSI  --> GPIO12 (nodeMCU: D7)  CLK   --> GPIO14 (nodeMCU: D5/CLK)
//  CS  --> GPIO15 (nodeMCU: D8)  DC  --> GPIO2 (nodeMCU: D4)  Reset to 3.3v  GND to ground  MISO may be left unconnected
Adafruit_ILI9341 tft = Adafruit_ILI9341(); 

#ifdef TOUCH
XPT2046 touch(4, 5);
#endif

SmartBuffer srbuffer(MessageReceived);
ScaneLineRenderer renderer(320);

//ArtificalHorizon aHorizon;
//Altimeer am;
AirSpeedIndicator aSpeed;


#ifdef TOUCH
static void calibratePoint(uint16_t x, uint16_t y, uint16_t &vi, uint16_t &vj) {
	// Draw cross	
	hspi_init();
	tft.drawFastHLine(x - 8, y, 16, 0xff);
	tft.drawFastVLine(x, y - 8, 16, 0xff);

	touch.begin(tft.width(), tft.height());
	while (!touch.isTouching()) {
		delay(10);
	}
	touch.getRaw(vi, vj);
	// Erase by overwriting with black
	hspi_init();
	tft.drawFastHLine(x - 8, y, 16, 0);
	tft.drawFastVLine(x, y - 8, 16, 0);
	touch.begin(tft.width(), tft.height());
}

void calibrate() {
	uint16_t x1, y1, x2, y2;
	uint16_t vi1, vj1, vi2, vj2;
    touch.begin(tft.width(), tft.height());

	touch.getCalibrationPoints(x1, y1, x2, y2);
	calibratePoint(x1, y1, vi1, vj1);
	delay(5000);
	touch.begin(tft.width(), tft.height());
	calibratePoint(x2, y2, vi2, vj2);
	touch.setCalibration(vi1, vj1, vi2, vj2);
	
	hspi_init();
	tft.setTextColor(ILI9341_CYAN);
	tft.setTextSize(2);
	tft.println("Calibration Params");
	tft.println("");
	tft.setTextSize(3);
	tft.println(vi1);
	tft.println(vj1);
	tft.println(vi2);
	tft.println(vj2);
	
	delay(10000);

	tft.print(F("valami"));
	
}
#endif

void setup() {

	ESP.wdtEnable(15);
	WiFi.forceSleepBegin();// turn off ESP8266 RF

	delay(1);
	Serial.begin(38400);

#ifdef TOUCH
	Serial.println("Init TouchScreen.");

	//touch.begin(tft.width(), tft.height());
	//touch.setRotation(XPT2046::ROT270);
	//touch.begin(tft.width(), tft.height());			// Must be done before setting rotation

	//calibrate();  // No rotation!! */
#endif

	// Init tft
	Serial.println("Init TFT.");
	tft.begin();
	tft.setRotation(1);
	Serial.println(testFillScreen());
	delay(500);

	renderer.ResetObjects2();
	//aHorizon.CreateAH(renderer);
	//am.Create(renderer);	
	aSpeed.Create(renderer);
}

uint8_t HexStrToByte(char ch1, char ch2) {
	/*Serial.println("-------");
	Serial.print(ch1);
	Serial.println(ch2);*/
	byte tens = (ch1 <= '9') ? ch1 - '0' : ch1 - 'A' + 10;
	byte ones = (ch2 <= '9') ? ch2 - '0' : ch2 - 'A' + 10;
	return (16 * tens) + ones;
}

float roll = 0;
float pitch = 0;

void MessageReceived(uint8_t * message, uint8_t received) {	
	
	/*Serial.print((char)message[0]);
	Serial.print((char)message[1]);
	Serial.print((char)message[2]);
	Serial.print((char)message[3]);*/

	uint8_t msb = HexStrToByte(message[0], message[1]);
	uint8_t lsb = HexStrToByte(message[2], message[3]);

	int16_t temp;
	uint8_t * p = (uint8_t * )&temp;
	*p = lsb;
	p++;
	*p = msb;

	roll = temp / 10.0;

	p = (uint8_t *)&temp;

	msb = HexStrToByte(message[4], message[5]);
	lsb = HexStrToByte(message[6], message[7]);

	*p = lsb;
	p++;
	*p = msb;

	pitch = temp / 10.0f;		
}


void loop(void) {
	static unsigned long t = millis();
	//Serial.println(millis() - t);
	t = millis();
   
	yield();

	srbuffer.Update();
	
	pitch += 5;
    aSpeed.SetValues(renderer, pitch, roll);
  
	// Output
/*	Serial.print(" Pitch = ");
	Serial.print(pitch);
	Serial.print(" Roll = ");
	Serial.print(roll);*/

#ifdef TOUCH
	// Touch screen library uses differnet spi library than the Adafruit fast library. It is a workaround, but not perfecet at all.
//	touch.begin(tft.height(), tft.width());
//	touch.setCalibration(130, 1807, 1856, 188);
//	touch.setRotation(XPT2046::ROT90);
	
	// Maybe would be enugh to reinit the general spi library
			
	/*
	if (touch.isTouching()) {
		uint16_t x, y;
		touch.getPosition(x, y);

		if (x < 120) roll += 3;
		if (x > 200) roll -= 3;
		if (y < 80)  pitch -= 3;
		if (y > 160) pitch += 3;

		tri.SetValue(x - 10, y, x, y + 10, x + 10, y, 1, 0x0ff0);
	}*/
	hspi_init();

#endif

	// Render to tft
	for (uint16_t i = 0; i < 240; i++) {
		renderer.RenderObjectsInLine(i);
		tft.drawFastHColors(0, i, 320, renderer.LineColors);	
		}	
  }


unsigned long testFillScreen() {
  unsigned long start = micros();
  tft.fillScreen(ILI9341_BLACK);
  tft.fillScreen(ILI9341_RED);
  tft.fillScreen(ILI9341_GREEN);
  tft.fillScreen(ILI9341_BLUE);
  tft.fillScreen(ILI9341_GREEN);
  return micros() - start;
}
