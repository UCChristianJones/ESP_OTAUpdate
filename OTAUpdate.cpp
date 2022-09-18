#include "OTAUpdate.h"

#ifdef DEBUG
#ifndef p(v)
// add methods and vars here for debug only
#define p(v) Serial.println(v); 
#endif
#else
// redefine the above macros, but they dont do anything
#define p(v)
#endif

void OTAUpdate::ota_handle( void * parameter ) 
{
  for (;;) {
    ArduinoOTA.handle();
    delay(checkEveryxMSs);
  }
}

void OTAUpdate::Setup(const char* nameprefix, int checkEveryxMSs) 
{
  OTAUpdate::checkEveryxMSs = checkEveryxMSs;
  uint16_t maxlen = strlen(nameprefix) + 7;
  char *fullhostname = new char[maxlen];
	String mac = WifiSetup::GetMACAddress();
  snprintf(fullhostname, maxlen, "%s-%02x%02x%02x", nameprefix, mac[3], mac[4], mac[5]);
  ArduinoOTA.setHostname(fullhostname);
  delete[] fullhostname;
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";
    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    p("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() { p("\nEnd"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    //Serial.println("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    //Serial.println("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("\nAuth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("\nBegin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("\nConnect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("\nReceive Failed");
    else if (error == OTA_END_ERROR) Serial.println("\nEnd Failed");
  });
  ArduinoOTA.begin();
  p("OTA Initialized");
  xTaskCreate(OTAUpdate::ota_handle,"OTA_HANDLE",10000,NULL,1,NULL);
}

int OTAUpdate::checkEveryxMSs(10000);