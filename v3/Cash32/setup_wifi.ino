/*
 * ********************************************************************
 *                              Setup WIFI
 * ********************************************************************
*/

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  int i = 0;
  while ((WiFi.status() != WL_CONNECTED) || (i == 10) ) {
    delay(500);
    i++;
    Serial.print(".");
  } 
  i = 0;

  // Actualiza el tiempo 
  configTime(gmtOffset_sec, daylightOffsett_sec, ntpServer);
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)){
    rtc.setTimeStruct(timeinfo);
  }

}
