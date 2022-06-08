/*
Example of how to use the LTC2633Class library:

The following is also used in the constructor to indicate the resolution of the LTC2633:
typedef enum	// Used in the object instatiation
{
	LTC2633_12BIT = 12, // 0x0C
	LTC2633_10BIT = 10, // 0x0A
	LTC2633_8BIT =  8   // 0x08
} LTC2633_RESOLUTION;

The following is used in the constructor to specify which voltage potential the CA0 pin is connected to:
typedef enum	// Used in the object instatiation
{
	LTC2633_CA0_GND 	= 0x10, //0b0010 000 Wr
	LTC2633_CA0_FLOAT 	= 0x11, //0b0010 001 Wr
	LTC2633_CA0_VCC 	= 0x12, //0b0010 010 Wr
	LTC2633_CA0_GLOBAL 	= 0x73, //0b1110 011 Wr
	LTC2633_GND 		= 0x10, //0b0010 000 Wr
	LTC2633_FLOAT 		= 0x11, //0b0010 001 Wr
	LTC2633_VCC 		= 0x12, //0b0010 010 Wr
	LTC2633_GLOBAL 		= 0x73, //0b1110 011 Wr
} LTC2633_I2C_ADDRESS;

The last parameter for the constructor is the address for a TwoWire instance, usually &Wire.
*/

#include <LTC2633Library.h>

LTC2633Library myDAC(LTC2633_12BIT, LTC2633_CA0_GND, &Wire);
    
void setup()
{

    Serial.begin(115200);

    
    //Initialize the DAC and check the return value to determine if the communication was successful or not:
    if (LTC2633.begin() == false)
    {
        Serial.println("LTC2633 not detected!");
        while (1); // Endless loop
    }
    
	// Write a value of 2048 to DAC A:
	myDAC.A.write(2048);
	
	// Write a value of 4095 to DAC B:
    myDAC.B.write(4095);
	
	// Write a value of 0 to both DACs:
	myDAC.ALL.write(0);
	
	// Write the closest value corresponding to a voltage of 1.2555V(2048) to DAC A:
	myDAC.A.writeVoltage(1.2555);
	
	// Write the closest value corresponding to a voltage of 2.5V(4095) to DAC A:
    myDAC.B.writeVoltage(2.5);
	
	// Write the closest value corresponding to a voltage of 1.0V to both DACs:
	myDAC.ALL.writeVoltage(1.0);
	
    
}

void loop()
{
    
    
}