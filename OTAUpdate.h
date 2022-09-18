#ifndef _OTA_
#define _OTA_

#include <WifiSetup.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>

class OTAUpdate
{
  static int checkEveryxMSs;
  static void ota_handle( void * parameter );
public:
	static void Setup(const char* nameprefix, int checkEveryxMSs=10000);
};
#endif