#include "Students.h"

#define WAIT_TIME           (20) // [ms]


void setup() 
{
	Serial.begin(115200);
	
}

void loop() 
{
	if(Serial.available() > 0) {
        // read character from serial port
		int getstr = Serial.read();
		if(getstr == 'a') {
            comment_Student_A();
        }
		else if(getstr == 'b') {
            comment_Student_B();
        }
		else if(getstr == 'c') {
            comment_Student_C();
        }
		else if(getstr == 'd') {
            comment_Student_D();
        }
    }

    delay(WAIT_TIME);
}
