/**
*	\file CoolFileSystem.cpp
*	\brief CoolFileSystem Source File
*	\author Mehdi Zemzem
*	\version 1.0
*	\date 27/06/2017
*
*/

#include "FS.h"                // File Storage System >>> http://esp8266.github.io/Arduino/versions/2.0.0/doc/filesystem.html
#include"CoolFileSystem.h"
#include "ArduinoJson.h"      // Arduino JSON File controller  https://github.com/bblanchon/ArduinoJson

/**
*	CoolFileSystem::begin():
*	This method is provided to start the
*	SPIFFS object.
*
*	\return true if SPIFFS was initialized correctly,
*	false otherwise
*/
bool CoolFileSystem::begin()
{
	return( SPIFFS.begin() );                                   //Initialize Filesystem

}

/**
*	CoolFileSystem::saveSensorData( data, data size ):
*	This method is provided to save the data on the local
*	memory when there is no internet available
*
*	sets the saved data flag to TRUE when successful
*
*	\return true if the data was saved,
*	false otherwise
*/
bool CoolFileSystem::saveSensorData(const char* data,int Sensor_JSON_SIZE)
{
	File sensorsData=SPIFFS.open("/sensorsData.json","a+");
	if(!sensorsData)
	{
		this->savedData=false;
		return (false);	
	}	

	DynamicJsonBuffer jsonBuffer(Sensor_JSON_SIZE);
	JsonObject& root = jsonBuffer.parseObject(data);
	
	root.printTo(sensorsData);
	sensorsData.close();
	
	this->savedData=true;
	return (true);		
}

/**
*	CoolFileSyste::updateConfigFiles( mqtt answer, answer size):
*	This method is provided to update the configuration files when
*	the appropriate mqtt answer is received:	-update : 1
*
*	\return true if the files are updated correctly,
*	false otherwise
*/
bool CoolFileSystem::updateConfigFiles(String answer,int JSON_SIZE)
{
	//String conversion to char*

	char *jsonRoot = new char[answer.length() + 1];
	strcpy(jsonRoot, answer.c_str());
	//total json object	
	DynamicJsonBuffer jsonBuffer(JSON_SIZE);
	JsonObject& root = jsonBuffer.parseObject(jsonRoot);


		
	//temp string
	String temp;

	//CoolBoard Configuration File

    	JsonObject& jsonCoolBoard=root["CoolBoard"];
	if(jsonCoolBoard.success())
	{
		File coolBoardConfig = SPIFFS.open("/coolBoardConfig.json", "w");	
		if(!coolBoardConfig)
		{	
			Serial.println("failed to open coolBoardConfig.json");
			return(false);
		}
		Serial.println("CoolBoard Config");
		jsonCoolBoard.printTo(Serial);
		
		jsonCoolBoard.printTo(coolBoardConfig);
		
		coolBoardConfig.close();
	}
	else
	{
		Serial.println("failed to pars coolBoard ");
	}		

	
	//Cool Board Sensors Configuration File
	DynamicJsonBuffer jsonSBoard;
    	JsonObject& jsonSensorsBoard=root["CoolSensorsBoard"];	
	if(jsonSensorsBoard.success())
	{	
		File coolBoardSensorsConfig = SPIFFS.open("/coolBoardSensorsConfig.json", "w");	
		if(!coolBoardSensorsConfig)
		{
			Serial.println("failed to open coolBoardSensors.json");
			return(false);
		}
		
		Serial.println("CoolBoardSensors Config");
		jsonSensorsBoard.printTo(coolBoardSensorsConfig);
		jsonSensorsBoard.printTo(Serial);
		coolBoardSensorsConfig.close();
	}
	else
	{
		Serial.println("failed to parse on board sensors ");	
	}
	
	
	
	//rtc configuration file
	DynamicJsonBuffer jsonR;
    	JsonObject& jsonRTC=root["rtc"];
	Serial.println("before config rtc json");
	jsonRTC.printTo(Serial);
	if(jsonRTC.success() )
	{
		File rtcConfig = SPIFFS.open("/rtcConfig.json", "w");	
		if(!rtcConfig)
		{
			Serial.println("failed to open rtcConfig.json");
			return(false);
		}
		Serial.println("RTC Config");
		jsonRTC.printTo(rtcConfig);
		jsonRTC.printTo(Serial);
		rtcConfig.close();
	
	}
	else
	{
		Serial.println("failed to parse rtc ");
	}

	
	
	
	
        //cool board led configuration
	DynamicJsonBuffer jsonLBoard;
    	JsonObject& jsonLedBoard=root["led"];
	Serial.println("before config Led json");
	jsonLedBoard.printTo(Serial);
	if(jsonLedBoard.success())
	{	
		File coolBoardLedConfig = SPIFFS.open("/coolBoardLedConfig.json", "w");	
		if(!coolBoardLedConfig)
		{
			Serial.println("failed to open led config");
			return(false);
		}
		Serial.println("CoolBoardLed Config");
		jsonLedBoard.printTo(coolBoardLedConfig);
		jsonLedBoard.printTo(Serial);
		coolBoardLedConfig.close();
	
	}
	else
	{
		Serial.println("failed to parse led");
	}
		

	

	//jetpack configuration
	DynamicJsonBuffer jsonJBoard;
    	JsonObject& jsonJetpack=root["jetPack"];
	Serial.println("before config jetpack json");
	jsonJetpack.printTo(Serial);
	if(jsonJetpack.success())
	{	
		File jetPackConfig = SPIFFS.open("/jetPackConfig.json", "w");	
		if(!jetPackConfig)
		{
			Serial.println("failed to open jetpack file");
			return(false);
		}
		Serial.println("jetpack Config");	
		jsonJetpack.printTo(jetPackConfig);
		jsonJetpack.printTo(Serial);
		jetPackConfig.close();
	}
	else
	{
		Serial.println("failed to parse jetpack");	
	}
	
	//irene configuration	
	DynamicJsonBuffer jsonIBoard;
    	JsonObject& jsonIrene=root["irene3000"];
	Serial.println("before config irene json");	
	jsonIrene.printTo(Serial);
	if(jsonIrene.success())
	{
		File irene3000Config = SPIFFS.open("/irene3000Config.json", "w");	
		if(!irene3000Config)
		{
			Serial.println("failed to open irene file");
			return(false);
		}
		Serial.println("irene3000 Config");
		jsonIrene.printTo(irene3000Config);
		jsonIrene.printTo(Serial);
		irene3000Config.close();
	
	}
	else
	{
		Serial.println("failed to parse irene");	
	}
	
	//external sensors
	DynamicJsonBuffer jsonESBoard;
    	JsonObject& jsonExternalSensors=root["externalSensors"];
	Serial.println("before config external Sensors json");
	jsonExternalSensors.printTo(Serial);
	if(jsonExternalSensors.success())
	{
		File externalSensorsConfig = SPIFFS.open("/externalSensorsConfig.json", "w");	
		if(!externalSensorsConfig)
		{
			Serial.println("failed to open external sensors file ");
			return(false);
		}
		Serial.println("externalSensors Config");
		jsonExternalSensors.printTo(externalSensorsConfig);
		jsonExternalSensors.printTo(Serial);
	
		externalSensorsConfig.close();

	}
	else
	{
		Serial.println("failed to parse external sensors");	
	}

	
	//mqtt config
	DynamicJsonBuffer jsonMQ;
    	JsonObject& jsonMQTT=root["mqtt"];
	Serial.println("before config mqtt json");
	jsonMQTT.printTo(Serial);
	if(jsonMQTT.success())
	{
		File mqttConfig = SPIFFS.open("/mqttConfig.json", "w");	
		if(!mqttConfig)
		{
			Serial.println("failed to open mqtt file ");		
			return(false);
		}
		Serial.println("mqtt config");
		jsonMQTT.printTo(mqttConfig);
		jsonMQTT.printTo(Serial);
		mqttConfig.close();
	}
	else
	{
		Serial.println("failed to parse mqtt");	
	}	
		
	return true;

}	

/**
*	CoolFileSystem::isDataSaved():
*	This method is provided to report
*	wether there is sensor data saved in the
*	File System.
*
*	\return true if there is data saved, false
*	otherwise
*/
bool CoolFileSystem::isDataSaved()
{
	return( this->savedData );
}

/**
*	CoolFileSystem::getSensorData():
*	This method is provided to return the 
*	sensor data saved in the File System
*
*	\return string json of the saved sensor 
*	data file
*/
String CoolFileSystem::getSensorSavedData()
{
	//open sensors data file
	File sensorsData=SPIFFS.open("/sensorsData.json","r");
	
	if (!sensorsData)

	{
		return("failed to open file");
	}
	else
	{
		size_t size = sensorsData.size();

		// Allocate a buffer to store contents of the file.
		std::unique_ptr < char[] > buf(new char[size]);

		sensorsData.readBytes(buf.get(), size);

		DynamicJsonBuffer jsonBuffer;

		JsonObject & json = jsonBuffer.parseObject(buf.get());
		
		if (!json.success())
		{
			return("failed to parse json");
		}
		else
		{	
			//the return string
			String sensorDataString;
			
			//print the json to the string
			json.printTo(sensorDataString);
			
			//close the file
			sensorsData.close();

			//delete data in the file
			File sensorsData=SPIFFS.open("/sensorsData.json","w");
			if (!sensorsData)	
			{
				return("failed to delete data in the file");
			}

			sensorsData.close();
			
			//position the saved data flag to false
			this->savedData=false;			

			//return the string
			return(sensorDataString);		
		}
		
		
	}

}

