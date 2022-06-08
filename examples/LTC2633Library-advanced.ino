/*
Advanced example of how to use the LTC2633Library:

Explenation of how to library works:

Following enums are used as options in the object instatiation:

typedef enum	// Sets the resolution of the DAC.
{
	LTC2633_12BIT = 12, // 0x0C
	LTC2633_10BIT = 10, // 0x0A
	LTC2633_8BIT =  8   // 0x08
} LTC2633_RESOLUTION;

typedef enum	// Depends upon the CA0 pin connection.
{
	LTC2633_CA0_GND 	= 0x10, //0b0010 000 Wr
	LTC2633_CA0_FLOAT 	= 0x11, //0b0010 001 Wr
	LTC2633_CA0_VCC 	= 0x12, //0b0010 010 Wr
	LTC2633_CA0_GLOBAL 	= 0x73, //0b1110 011 Wr
} LTC2633_I2C_ADDRESS;

Example:	
			LTC2633Library myDAC(LTC2633_12BIT, LTC2633_CA0_GND, &Wire);
			myDAC.begin();
			myDAC.A.write(4095);
			myDAC.B.write(2048);
			myDAC.ALL.write(0);
			
The available methods for myDAC.A/.B/.ALL are:	
		int8_t write(uint16_t data);				// Writes the value of data to the chosen DAC.
		int8_t writeVoltage(double volt);           // Write the closest value corresponding to the voltage volt.
		int8_t writeUpdateBoth(uint16_t data);      // Writes the value of data to both DACs.
		int8_t writeInputRegister(uint16_t data);   // Write to the DACs input register without updating the DACs output register.
		int8_t updateDACRegister();                 // Update the DACs output register.
		
		
    	void setI2CStandardClockSpeed();            // Use I2C 100kHz clock speed.
		void setI2CFastClockSpeed();                // Use I2C 400kHz clock speed.
		
		int8_t useInternalReference(void);          // Use internal reference.
    	int8_t useExternalReference(void);          // Use external reference.
		
		int8_t powerDown();                         // Power down a single DAC.
		int8_t powerDownChip();                     // the whole chip powers down.
		
		void CA0PinToGND();                         // Can be used to set the I2C Address.
		void CA0PinToFLOAT();                       // Can be used to set the I2C Address.
		void CA0PinToVCC();                         // Can be used to set the I2C Address.
		void CA0PinToGLOBAL();                      // Can be used to set the I2C Address.
		
		int8_t increamentOutput();                  // increment the DAC output by the value.
		int8_t decreamentOutput();                  // decrement the DAC output by the value.
		int8_t resetOutput();                       // set DAC output to 0.
*/

#include <LTC2633Library.h>

LTC2633Library myDAC(LTC2633_12BIT, LTC2633_CA0_GND, &Wire);

uint16_t value = 0;
double voltage = 2.5;
    
void setup()
{

    Serial.begin(115200);

    
    //Initialize the DAC and check the return value to determine if the communication was successful or not:
    if (myDAC.begin() == false)
    {
        Serial << "LTC2633 not detected!" << endl;
        while (1); // Endless loop
    }
    
	// I2C specific functions:
		// Can be used to set the I2C Address:
		myDAC.CA0PinToGND();
		myDAC.CA0PinToFLOAT();
		myDAC.CA0PinToVCC();
		myDAC.CA0PinToGLOBAL();
		
		// Standard Speed = 100kHz(default), Fast Speed = 400kHz.
		//myDAC.setI2CStandardClockSpeed();
		myDAC.setI2CFastClockSpeed();
	
	// Set reference source functions:
		// Uses internal reference as default.
		myDAC.useInternalReference();
		//useExternalReference();
	
	// Write functions:
		// Write a value to a DAC A:
		myDAC.A.write(value);
		
		// Write a value to a DAC B:
		myDAC.B.write(value);
		
		// This will make both DAC A & DAC B to be set to 0:
		myDAC.ALL.write(value);
		
		// Write and output the closest digital code to reflect the voltage passed to the function in form of a double variable:
		myDAC.B.writeVoltage(voltage);
		
	// Mostly used for internal use according to datasheet:
		// Write to the DACs input register without updating the DACs output register, i.e. the DACs output voltage doesn't change.
		myDAC.A.writeInputRegister(value);
		// Update the DACs output register so the output voltage changes to reflect the content of the input register.
		myDAC.A.updateDACRegister();
		
	// Power Managedment functions:	
		// Power down a single DAC:
		myDAC.B.powerDown();
		
		// All the following 4 lines have the same result, the whole chip powers down:
		myDAC.powerDownChip();
		myDAC.A.powerDownChip();
		myDAC.B.powerDownChip();
		myDAC.ALL.powerDownChip();
	
	// After having powered down 1 DAC, 2 DACs or the whole chip, it powers back up the next time you write any DAC register:
	myDAC.A.write(0);
	
	// increment/decrement DAC output:
		// stepSize is used by the functions below this block:
		myDAC.A.setStepSize(stepSize);
		myDAC.A.getStepSize();
		
		// increment/decrement the DAC output by the value of stepSize, default is 1:
		myDAC.A.incrementOutput();
		myDAC.A.decrementOutput();
		// set DAC output to 0:
		myDAC.A.zeroOutput();
    
}

void loop()
{
    
    
}