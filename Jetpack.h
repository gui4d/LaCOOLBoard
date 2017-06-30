/**
*	\file Jetpack.h
*	\brief Jetpack Header File
*	\author Mehdi Zemzem
*	\version 1.0
*	\date 27/06/2017
*
*/

#ifndef Jetpack_H
#define Jetpack_H

#include"Arduino.h"

#include <Ticker.h>


/**
*	\class Jetpack
*	\brief This class manages the Jetpack shield
*
*
*
*/
class Jetpack
{
public:


	void begin();		       //starts the Jetpack

 	void write(byte action);			//writes to the Jetpack
 
	void writeBit(byte pin,bool state); //writes to a single pin of the Jetpack
	
	void setBit(byte pin);
	
	void resetBit(byte pin);
 
	void doAction(const char* data, int JSON_SIZE);
       
	bool config();
	void printConf();
private:
	// byte actors; //pin for the actor 

	byte action= B00000000; 
	byte actorsNumber;

	struct state{
	const char* type;
	bool actif;
	int low;
	int high;
	bool temporal;	
	}actors[8];
	
	Ticker tickerSetHigh[8];
	Ticker tickerSetLow[8];
	
	const int clockPin = 4; //clock pin for the shift register
	const int dataPin = 15; //data  pin for the shift register
	const int EnI2C=5;	// I2C/Shift pin
	
};

#endif
