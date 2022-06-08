#ifndef _LTC2633_H_
#define _LTC2633_H_

#include <Arduino.h>
#include <Wire.h>

typedef enum
{
	WRITE_INPUT_N 					= 0X00, // 0b0000 0000
    UPDATE_DAC_N 					= 0X10, // 0b0001 0000
    WRITE_INPUT_N_UPDATE_DAC_ALL 	= 0X20, // 0b0010 0000
    WRITE_INPUT_N_UPDATE_DAC_N 		= 0X30, // 0b0011 0000
    POWER_DOWN_N 					= 0X40, // 0b0100 0000
    POWER_DOWN_CHIP 				= 0X50, // 0b0101 0000
    SELECT_INTERNAL_REFERENCE 		= 0X60, // 0b0110 0000
    SELECT_EXTERNAL_REFERENCE 		= 0X70, // 0b0111 0000
    NO_OPERATION 					= 0XF0  // 0b1111 0000
} LTC2633_COMMAND;

typedef enum
{
	LTC2633_CA0_GND 	= 0x10, //0b0010 000 Wr
	LTC2633_CA0_FLOAT 	= 0x11, //0b0010 001 Wr
	LTC2633_CA0_VCC 	= 0x12, //0b0010 010 Wr
	LTC2633_CA0_GLOBAL 	= 0x73, //0b1110 011 Wr
	LTC2633_GND 		= 0x10, //0b0010 000 Wr
	LTC2633_FLOAT 		= 0x11, //0b0010 001 Wr
	LTC2633_VCC 		= 0x12, //0b0010 010 Wr
	LTC2633_GLOBAL 		= 0x73  //0b1110 011 Wr
} LTC2633_I2C_ADDRESS;

typedef enum
{
	LTC2633_12BIT = 12, // 0x0C
	LTC2633_10BIT = 10, // 0x0A
	LTC2633_8BIT =  8   // 0x08
} LTC2633_RESOLUTION;

typedef enum
{
	LTC2633_DAC_A 	= 0X00, // 0b0000 0000
    LTC2633_DAC_B 	= 0X01, // 0b0000 0001
    LTC2633_DAC_ALL = 0X0F  // 0b0000 1111
} LTC2633_DAC_selection;


class LTC2633_single_DAC
{
  	public:

    	LTC2633_single_DAC(LTC2633_RESOLUTION resolution = LTC2633_12BIT, 
					LTC2633_I2C_ADDRESS I2C_address = LTC2633_CA0_GLOBAL, 
					LTC2633_DAC_selection DAC_selection = LTC2633_DAC_ALL, 
					TwoWire *theWire = &Wire)
					/*: _DAC_selected(DAC_selection),
					  _resolution(resolution),
					  _I2C_address(I2C_address),
					  _wire(theWire),
					  _data(0)
					*/
		{
			_DAC_selected = DAC_selection;
			_resolution = resolution;
			_I2C_address = I2C_address;
			_wire = theWire;
			_data = 0;
			
		};
		
		void beginDAC()
		{
			_fullScaleVoltage = 2.5;
			_N_steps = (pow(2, (float)_resolution) - 1);
			_LSB = _fullScaleVoltage / _N_steps;
		};
		
		//void writeCommand(uint8_t command, uint16_t data);
		int8_t writeCommand(LTC2633_COMMAND command, LTC2633_DAC_selection DAC_selection, uint16_t data)
		{
			if(data > (pow(2, (float)_resolution) - 1))
			{
				data = (pow(2, (float)_resolution) - 1);
			}
			
			_tempData = data << (16 - _resolution);
			_theHighByte = _tempData >> 8;
			_theLowByte  = _tempData & 0xFF;
			uint8_t commandByte = (command | DAC_selection);
			
			_wire->beginTransmission(_I2C_address);
			_wire->write(commandByte);
			_wire->write(_theHighByte);
			_wire->write(_theLowByte);
			int8_t result = _wire->endTransmission();
			return result;
		};
		
		int8_t writeCommand(LTC2633_COMMAND command, uint16_t data)
		{
			if(data > (pow(2, (float)_resolution) - 1))
			{
				data = (pow(2, (float)_resolution) - 1);
			}
			
			_tempData = data << (16 - _resolution);
			_theHighByte = _tempData >> 8;
			_theLowByte  = _tempData & 0xFF;
			uint8_t commandByte = (command | _DAC_selected);
			
			_wire->beginTransmission(_I2C_address);
			_wire->write(commandByte);
			_wire->write(_theHighByte);
			_wire->write(_theLowByte);
			int8_t result = _wire->endTransmission();
			return result;
		};
		
		int8_t writeVoltage(double volt)
		{
			double data = _N_steps * (volt / _fullScaleVoltage);
			
			int8_t result = writeCommand(WRITE_INPUT_N_UPDATE_DAC_N, (uint16_t)data);
			return result;
		}
		
		int8_t write(uint16_t data)
		{
			_data = data;
			int8_t result = writeCommand(WRITE_INPUT_N_UPDATE_DAC_N, data);
			return result;
		};
		
		
		int8_t writeUpdateBoth(uint16_t data)
		{
			_data = data;
			int8_t result = writeCommand(WRITE_INPUT_N_UPDATE_DAC_ALL, data);
			return result;
		};
		
		int8_t writeInputRegister(uint16_t data)
		{
			_data = data;
			int8_t result = writeCommand(WRITE_INPUT_N, data);
			return result;
		};
		
		int8_t updateDACRegister()
		{
			int8_t result = writeCommand(UPDATE_DAC_N, 0);
			return result;
		};
		
		
    	void setI2CStandardClockSpeed()
		{
			_wire->setClock(100000); 	// Use 100kHz I2C clock.
		};
		
		void setI2CFastClockSpeed()
		{
			_wire->setClock(400000); 	// Use 400kHz I2C clock.
		};
	
    	int8_t useInternalReference(void)
		{
			int8_t result = writeCommand(SELECT_INTERNAL_REFERENCE, 0);
			return result;
		};
		
    	int8_t useExternalReference(void)
		{
			int8_t result = writeCommand(SELECT_EXTERNAL_REFERENCE, 0);
			return result;
		};
		
		int8_t powerDown()
		{
			int8_t result = writeCommand(POWER_DOWN_N, 0);
			return result;
		};
		
		int8_t powerDownChip()
		{
			int8_t result = writeCommand(POWER_DOWN_CHIP, 0);
			return result;
		};
		
		void CA0PinToGND()
		{
			_I2C_address = LTC2633_CA0_GND;
		};
		
		void CA0PinToFLOAT()
		{
			_I2C_address = LTC2633_CA0_FLOAT;
		};
		
		void CA0PinToVCC()
		{
			_I2C_address = LTC2633_CA0_VCC;
		};
		
		void CA0PinToGLOBAL()
		{
			_I2C_address = LTC2633_CA0_GLOBAL;
		};
		
		
		int8_t increamentOutput()
		{
			_data++;
			int8_t result = writeCommand(WRITE_INPUT_N_UPDATE_DAC_N, _data);
			return result;
		};
		
		int8_t decreamentOutput()
		{
			_data--;
			int8_t result = writeCommand(WRITE_INPUT_N_UPDATE_DAC_N, _data);
			return result;
		};
		
		int8_t resetOutput()
		{
			_data = 0;
			int8_t result = writeCommand(WRITE_INPUT_N_UPDATE_DAC_N, _data);
			return result;
		};
		
		LTC2633_RESOLUTION _resolution;
		LTC2633_I2C_ADDRESS _I2C_address;
		LTC2633_DAC_selection _DAC_selected;
		TwoWire *_wire;
		uint16_t _data;
		uint16_t _tempData;
		uint8_t _theHighByte;
    	uint8_t _theLowByte;
		
		
		double _fullScaleVoltage;
		double _N_steps;
		double _LSB;
		
	  private:
		// 2.5V / 2^12:
		// 0.0006103515625 V
		//      610.3515625 µV
		//     0.610 mV
		//
		// 2.5V / (2^12 - 1):
		// 6.1050061050061050061050061050061e-4 V
		// 0.00061050061050061050061050061050061 V
		//      610.500 µV
		//     0.610500 mV
		
		// HOW TO WRITE THE CODE VALUE CORRESPONDING TO A CERTAIN VOLTAGE:
		// 	  Desired Voltage = volt;
		//    Steps available at the appropiate resolution = Nsteps;
		//    Reference Voltage or Full-scale Voltage = Vref;
		//    
		//    DAC_code = (Nsteps * (volt / Vref));
		// DAC code = maxResolution(4095) * (targetVoltage / referenceVoltage)
		//  
		// HOW TO CALCULATE THE OUTPUT VOLTAGE CORRESPONDING TO A CERTAIN CODE VALUE:
		// 	  Code Value Written to the DAC = data;
		//    Steps available at the appropiate resolution = Nsteps;
		//    Reference Voltage or Full-scale Voltage = Vref;
		//    Resulting Voltage = outputVoltage;
		//    
		//    outputVoltage = (Vref * (data / Vref));
		//
		//	0.0005 V
		//	0.516 mV
		//  0.522 mV offset voltage
		//
		//
		//
		//
};


class LTC2633 : public LTC2633_single_DAC
{
  public:
  	LTC2633(LTC2633_RESOLUTION resolution = LTC2633_12BIT, 
			LTC2633_I2C_ADDRESS I2C_address = LTC2633_CA0_GLOBAL, 
			TwoWire *theWire = &Wire)
			: A(resolution, I2C_address, LTC2633_DAC_A, theWire),
			  B(resolution, I2C_address, LTC2633_DAC_B, theWire),
			  ALL(resolution, I2C_address, LTC2633_DAC_ALL, theWire)
			  //LTC2633_single_DAC::_resolution(resolution),
			  //LTC2633_single_DAC::_I2C_address(I2C_address),
			  //LTC2633_single_DAC::_wire(theWire)
	{
		LTC2633_single_DAC::_resolution = resolution;
		LTC2633_single_DAC::_I2C_address = I2C_address;
		LTC2633_single_DAC::_wire = theWire;
	};
	
	void begin()
	{
		_wire->begin();
		_wire->setClock(100000); 	// Use 100kHz I2C clock.
		LTC2633_single_DAC::_DAC_selected = LTC2633_DAC_ALL;
		A.beginDAC();
		B.beginDAC();
		ALL.beginDAC();
		this->beginDAC();
	};
	
	LTC2633_single_DAC A;
	LTC2633_single_DAC B;
	LTC2633_single_DAC ALL;
		
};


#endif // _LTC2633_H_
		
		