
//Proto G Engineering www.youtube.com/c/protog42 3/26/2020

#include <WiFi.h>
#include "SSD1306.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>

SSD1306  display(0x3C, 5, 4);

const char* ssid = "YOUR WIFI SSID";
const char* password =  "YOUR WIFI PASSWORD";


void setup() {

  Serial.begin(115200);
  delay(4000);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");

  display.init();

  //display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.clear();

}

void loop() {

  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status

    HTTPClient http;

    http.begin("https://services1.arcgis.com/0MSEUqKaxRlEPj5g/ArcGIS/rest/services/Coronavirus_2019_nCoV_Cases/FeatureServer/1/query?where=OBJECTID%3E0&objectIds=298&time=&geometry=&geometryType=esriGeometryEnvelope&inSR=&spatialRel=esriSpatialRelIntersects&resultType=none&distance=0.0&units=esriSRUnit_Meter&returnGeodetic=false&outFields=Province_State%2C+Confirmed%2C+Deaths&returnGeometry=false&featureEncoding=esriDefault&multipatchOption=xyFootprint&maxAllowableOffset=&geometryPrecision=&outSR=&datumTransformation=&applyVCSProjection=false&returnIdsOnly=false&returnUniqueIdsOnly=false&returnCountOnly=false&returnExtentOnly=false&returnQueryGeometry=false&returnDistinctValues=false&cacheHint=false&orderByFields=&groupByFieldsForStatistics=&outStatistics=&having=&resultOffset=&resultRecordCount=&returnZ=false&returnM=false&returnExceededLimitFeatures=true&quantizationParameters=&sqlFormat=none&f=pjson&token="); //Specify the URL
    int httpCode = http.GET();                                        //Make the request

    if (httpCode > 0) { //Check for the returning code


      const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(3) + JSON_OBJECT_SIZE(1) + 2 * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + 2 * JSON_OBJECT_SIZE(6) + 2 * JSON_OBJECT_SIZE(7) + 590;
      DynamicJsonDocument doc(capacity);

      String payload = http.getString();
      
      const char* json = payload.c_str();

      deserializeJson(doc, json);

      const char* objectIdFieldName = doc["objectIdFieldName"]; // "OBJECTID"

      const char* uniqueIdField_name = doc["uniqueIdField"]["name"]; // "OBJECTID"
      bool uniqueIdField_isSystemMaintained = doc["uniqueIdField"]["isSystemMaintained"]; // true

      const char* globalIdFieldName = doc["globalIdFieldName"]; // ""
      const char* geometryType = doc["geometryType"]; // "esriGeometryPoint"

      int spatialReference_wkid = doc["spatialReference"]["wkid"]; // 4326
      int spatialReference_latestWkid = doc["spatialReference"]["latestWkid"]; // 4326

      JsonArray fields = doc["fields"];

      JsonObject fields_0 = fields[0];
      const char* fields_0_name = fields_0["name"]; // "Province_State"
      const char* fields_0_type = fields_0["type"]; // "esriFieldTypeString"
      const char* fields_0_alias = fields_0["alias"]; // "Province/State"
      const char* fields_0_sqlType = fields_0["sqlType"]; // "sqlTypeOther"
      int fields_0_length = fields_0["length"]; // 8000

      JsonObject fields_1 = fields[1];
      const char* fields_1_name = fields_1["name"]; // "Confirmed"
      const char* fields_1_type = fields_1["type"]; // "esriFieldTypeInteger"
      const char* fields_1_alias = fields_1["alias"]; // "Confirmed"
      const char* fields_1_sqlType = fields_1["sqlType"]; // "sqlTypeOther"

      JsonObject fields_2 = fields[2];
      const char* fields_2_name = fields_2["name"]; // "Deaths"
      const char* fields_2_type = fields_2["type"]; // "esriFieldTypeInteger"
      const char* fields_2_alias = fields_2["alias"]; // "Deaths"
      const char* fields_2_sqlType = fields_2["sqlType"]; // "sqlTypeOther"

      JsonObject features_0_attributes = doc["features"][0]["attributes"];
      const char* features_0_attributes_Province_State = features_0_attributes["Province_State"]; // "Florida"
      int features_0_attributes_Confirmed = features_0_attributes["Confirmed"]; // 1978
      int features_0_attributes_Deaths = features_0_attributes["Deaths"]; // 23

      Serial.print("Origin: ");
      Serial.println(features_0_attributes_Province_State);
      Serial.print("Confirmed:");
      Serial.println(features_0_attributes_Confirmed);
      Serial.print("Deaths:");
      Serial.println(features_0_attributes_Deaths);

      display.clear();
      display.setColor(WHITE);
      display.setTextAlignment(TEXT_ALIGN_CENTER);
      display.setFont(ArialMT_Plain_16);
      display.drawString(64, 0, String(features_0_attributes_Province_State) + " COVID-19");
      display.drawString(64, 22, String(features_0_attributes_Confirmed) + " Cases");
      display.drawString(64, 44, String(features_0_attributes_Deaths) + " Deaths");
      display.display();
    }

    else {
      Serial.println("Error on HTTP request");
    }

    http.end(); //Free the resources




  }

  delay(10000);

}
