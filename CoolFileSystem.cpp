/**
*	\file CoolFileSystem.cpp
*	\brief CoolFileSystem Source File
*	\author Mehdi Zemzem
*	\version 1.0
*	\date 27/06/2017
*
*/

#include "FS.h"                // File Storage System >>> http://esp8266.github.io/Arduino/versions/2.0.0/doc/filesystem.html
#include "CoolFileSystem.h"
#include "ArduinoJson.h"      // Arduino JSON File controller  https://github.com/bblanchon/ArduinoJson



#define DEBUG 1

#ifndef DEBUG

#define DEBUG 0 

#endif


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

#if DEBUG == 1

	Serial.println( F("Entering CoolFileSystem.begin()") );
	Serial.println();	
	Serial.print( F("SPIFFS success ? ") );
	Serial.println(SPIFFS.begin());
	Serial.println();

#endif

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

#if DEBUG == 1

	Serial.println( F("Entering CoolFileSystem.saveSensorData()") );
	Serial.println();

#endif
	
	File sensorsData=SPIFFS.open("/sensorsData.json","a+");
	if(!sensorsData)
	{
	
	#if DEBUG == 1
	
		Serial.println( F("failed to append to /sensorsData.json") );
		Serial.println();
	
	#endif

		this->savedData=false;
		return (false);	
	}	

	DynamicJsonBuffer jsonBuffer(Sensor_JSON_SIZE);
	JsonObject& root = jsonBuffer.parseObject(data);

	if( root.success() )
	{
		root.printTo(sensorsData);
		sensorsData.close();
	
	#if DEBUG == 1
	
		Serial.println( F("saved data is : ") );
		root.printTo(Serial);
		Serial.println();
	
	#endif

		this->savedData=true;
		return (true);		
	}
	else
	{
	
	#if DEBUG == 1

		Serial.println( F("failed to parse json") );
	
	#endif

		this->savedData=false;
		return(false);
	}
	

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

#if DEBUG == 1

	Serial.println( F("Entering CoolFileSystem.updateConfigFiles") );
	Serial.println();
	
	Serial.println( F("input answer : ") );
	Serial.println(answer);
#endif
	//test
	char buffer[ answer.length()+2 ];
	answer.toCharArray( buffer, answer.length()+2  );
	//total json object	
	DynamicJsonBuffer jsonBuffer(JSON_SIZE);
	JsonObject& root = jsonBuffer.parseObject( buffer );

#if DEBUG == 1

	Serial.println( F("json object : ") );	
	root.printTo(Serial);

#endif

	if(! ( root.success() ))
	{
	
	#if DEBUG == 1

		Serial.println( F("failed to parse root ") );
		Serial.println();
	
	#endif

		return(false);
	}
	else
	{
	#if DEBUG == 1
		
		Serial.println( F("success to parse root ") );
		Serial.println();
		
	#endif	
	}
	
#if DEBUG == 1

	Serial.println( F("input message is : ") );
	root.printTo(Serial);
	Serial.println();

#endif
	//temp string
	String temp;

	//CoolBoard Configuration File

    	JsonObject& jsonCoolBoard=root["CoolBoard"];

#if DEBUG == 1

	Serial.println( F("before config CoolBoard json") );
	jsonCoolBoard.printTo(Serial);

#endif

	if(jsonCoolBoard.success())
	{
		File coolBoardConfig = SPIFFS.open("/coolBoardConfig.json", "w");	
		if(!coolBoardConfig)
		{	
		#if DEBUG == 1
			
			Serial.println( F("failed to write to coolBoardConfig.json") );

		#endif
			return(false);
		}
		
		jsonCoolBoard.printTo(coolBoardConfig);
		
		coolBoardConfig.close();


	#if DEBUG == 1

		Serial.println( F("CoolBoard Config") );
		jsonCoolBoard.printTo(Serial);
	
	#endif
		
	}
	else
	{
	
	#if DEBUG == 1 

		Serial.println( F("failed to parse CoolBoard ") );
	
	#endif

	}		

	
	//Cool Board Sensors Configuration File
    	JsonObject& jsonSensorsBoard=root["CoolSensorsBoard"];

#if DEBUG == 1 

	Serial.println( F("before config CoolSensorsBoard json") );
	jsonSensorsBoard.printTo(Serial);

#endif 
	
	if(jsonSensorsBoard.success())
	{	
		File coolBoardSensorsConfig = SPIFFS.open("/coolBoardSensorsConfig.json", "w");	
		if(!coolBoardSensorsConfig)
		{
		#if DEBUG == 1 
		
			Serial.println( F("failed to write coolBoardSensors.json") );

		#endif

			return(false);
		}
		
		jsonSensorsBoard.printTo(coolBoardSensorsConfig);
		coolBoardSensorsConfig.close();

	#if DEBUG == 1

		Serial.println("CoolBoardSensors Config");
		jsonSensorsBoard.printTo(Serial);

	#endif

	}
	else
	{

	#if DEBUG == 1

		Serial.println( F("failed to parse CoolSensorsBoard sensors ") );	
	
	#endif

	}
	
	
	
	//rtc configuration file
    	JsonObject& jsonRTC=root["rtc"];

#if DEBUG == 1 
	
	Serial.println( F("before config rtc json") );
	jsonRTC.printTo(Serial);

#endif
	if(jsonRTC.success() )
	{
		File rtcConfig = SPIFFS.open("/rtcConfig.json", "w");	
		if(!rtcConfig)
		{
		
		#if DEBUG == 1 

			Serial.println( F("failed to write rtcConfig.json") );

		#endif

			return(false);
		}

		jsonRTC.printTo(rtcConfig);
		rtcConfig.close();

	#if DEBUG == 1 

		Serial.println( F("RTC Config") );
		jsonRTC.printTo(Serial);
	
	#endif

	
	}
	else
	{
	
	#if DEBUG == 1 

		Serial.println( F("failed to parse rtc ") );
	
	#endif

	}

	
	
	
	
        //cool board led configuration
    	JsonObject& jsonLedBoard=root["led"];
	
#if DEBUG == 1 

	Serial.println( F("before config Led json") );
	jsonLedBoard.printTo(Serial);

#endif

	if(jsonLedBoard.success())
	{	
		File coolBoardLedConfig = SPIFFS.open("/coolBoardLedConfig.json", "w");	
		if(!coolBoardLedConfig)
		{
		
		#if DEBUG == 1 
		
			Serial.println( F("failed to write led config") );
		
		#endif

			return(false);
		}
		
		jsonLedBoard.printTo(coolBoardLedConfig);
		coolBoardLedConfig.close();

	#if DEBUG == 1 

		Serial.println( F("CoolBoardLed Config") );		
		jsonLedBoard.printTo(Serial);
	
	#endif

	
	}
	else
	{
	
	#if DEBUG == 1 

		Serial.println( F("failed to parse led") );
	
	#endif 

	}
		

	

	//jetpack configuration
    	JsonObject& jsonJetpack=root["jetPack"];

#if DEBUG == 1 

	Serial.println( F("before config jetpack json") );
	jsonJetpack.printTo(Serial);

#endif

	if(jsonJetpack.success())
	{	
		File jetPackConfig = SPIFFS.open("/jetPackConfig.json", "w");	
		if(!jetPackConfig)
		{
		#if DEBUG == 1

			Serial.println( F("failed to write jetpack file") );
		
		#endif

			return(false);
		}

		jsonJetpack.printTo(jetPackConfig);
		jetPackConfig.close();

	#if DEBUG == 1

		Serial.println( F("jetpack Config") );	
		jsonJetpack.printTo(Serial);
	
	#endif

	}

	else
	{
	
	#if DEBUG == 1 

		Serial.println( F("failed to parse jetpack") );	
	
	#endif

	}
	
	//irene configuration	
    	JsonObject& jsonIrene=root["irene3000"];
	
#if DEBUG == 1 

	Serial.println( F("before config irene json") );	
	jsonIrene.printTo(Serial);

#endif 

	if(jsonIrene.success())
	{
		File irene3000Config = SPIFFS.open("/irene3000Config.json", "w");	
		if(!irene3000Config)
		{

		#if DEBUG == 1 

			Serial.println( F("failed to write irene file") );
		
		#endif

			return(false);
		}

		jsonIrene.printTo(irene3000Config);
		irene3000Config.close();
	
	#if DEBUG == 1 
		
		Serial.println( F("irene3000 Config") );
		jsonIrene.printTo(Serial);
	
	#endif
	
	}
	else
	{
	
	#if DEBUG == 1 

		Serial.println( F("failed to parse irene") );	
	
	#endif 


	}
	
	//external sensors
    	JsonObject& jsonExternalSensors=root["externalSensors"];

#if DEBUG == 1 

	Serial.println( F("before config external Sensors json") );
	jsonExternalSensors.printTo(Serial);

#endif

	if(jsonExternalSensors.success())
	{
		File externalSensorsConfig = SPIFFS.open("/externalSensorsConfig.json", "w");	
		if(!externalSensorsConfig)
		{
		
		#if DEBUG == 1 

			Serial.println( F("failed to open external sensors file ") );
		
		#endif 

			return(false);
		}

#if DEBUG == 1 
		
		Serial.println( F("externalSensors Config") );
		jsonExternalSensors.printTo(Serial);

#endif 

		jsonExternalSensors.printTo(externalSensorsConfig);	
		externalSensorsConfig.close();

	}

	else
	{	

	#if DEBUG == 1
		
		Serial.println( F("failed to parse external sensors") );

	#endif


	}

	
	//mqtt config
    	JsonObject& jsonMQTT=root["mqtt"];
	
#if DEBUG == 1 

	Serial.println( F("before config mqtt json") );
	jsonMQTT.printTo(Serial);

#endif

	if(jsonMQTT.success())
	{
		File mqttConfig = SPIFFS.open("/mqttConfig.json", "w");	
		if(!mqttConfig)
		{
		
		#if DEBUG == 1 

			Serial.println( F("failed to open mqtt file ") );
		
		#endif
		
			return(false);
		}

#if DEBUG == 1 

		Serial.println( F("mqtt config") );
		jsonMQTT.printTo(Serial);

#endif
	
		jsonMQTT.printTo(mqttConfig);
		mqttConfig.close();
	}
	else
	{

	#if DEBUG == 1 

		Serial.println( F("failed to parse mqtt") );
	
	#endif

	
	}	

	//wifi config
    	JsonObject& jsonWifi=root["wifi"];
	
#if DEBUG == 1 

	Serial.println( F("before config wifi json") );
	jsonWifi.printTo(Serial);

#endif

	if(jsonWifi.success())
	{
		File wifiConfig = SPIFFS.open("/wifiConfig.json", "w");	
		if(!wifiConfig)
		{
		
		#if DEBUG == 1 

			Serial.println( F("failed to open wifi file ") );
		
		#endif
		
			return(false);
		}

#if DEBUG == 1 

		Serial.println( F("wifi config") );
		jsonWifi.printTo(Serial);

#endif
	
		jsonMQTT.printTo(wifiConfig);
		wifiConfig.close();
	}
	else
	{

	#if DEBUG == 1 

		Serial.println( F("failed to parse wifi") );
	
	#endif

	
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

#if DEBUG == 1 

	Serial.println( F("Entering CoolFileSystem.isDataSaved()") );
	Serial.println();
	Serial.print( F("savedData : ") );
	Serial.println(this->savedData);

#endif

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

#if DEBUG == 1 

	Serial.println( F("Entering CoolFileSystem.getSensorSavedData()") );
	Serial.println();

#endif

	//open sensors data file
	File sensorsData=SPIFFS.open("/sensorsData.json","r");
	
	if (!sensorsData)
	{

	#if DEBUG == 1 

		Serial.println( F("Failed to read /sensorsData.json") );

	#endif
 
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

		#if DEBUG == 1
		
			Serial.println( F("failed to parse json") );
		
		#endif
		
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
			#if DEBUG == 1
		
				Serial.println( F("failed to delete data in the file") );
		
			#endif

				return("failed to delete data in the file");
			}

			sensorsData.close();
			
			//position the saved data flag to false
			this->savedData=false;	
			
		#if DEBUG == 1 

			Serial.println( F("saved data : ") );
			Serial.println(sensorDataString);
			Serial.println();
		
		#endif

			//return the string
			return(sensorDataString);		
		}
		
		
	}

}

