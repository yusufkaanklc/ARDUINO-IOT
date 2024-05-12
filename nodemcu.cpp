#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>

const int ledpin1 = D1;
const int ledpin2 = D2;
const int buttonPin16 = 16;
const int LDR1 = D3;
const int LDR2 = D4;

bool buttonState = HIGH;
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

WiFiManager wifiManager;

void resetDevice()
{
    ESP.restart();
}

void setup()
{
    Serial.begin(9600);

    pinMode(ledpin1, OUTPUT);
    pinMode(ledpin2, OUTPUT);
    pinMode(buttonPin16, INPUT_PULLUP);

    wifiManager.setConnectTimeout(10);
    wifiManager.setConfigPortalTimeout(0);

    if (!WiFi.SSID().isEmpty())
    {
        Serial.print("Last connected network: ");
        Serial.println(WiFi.SSID());
        Serial.println("Connecting to last connected network...");
        WiFi.begin();
    }
}

void reconnectWiFi()
{
    Serial.println("WiFi connection lost. Reconnecting...");
}

void loop()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        reconnectWiFi();
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("WiFi connecting.");
    }

    int buttonReading = digitalRead(buttonPin16);

    if (buttonReading != lastButtonState)
    {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay)
    {
        if (buttonReading != buttonState)
        {
            buttonState = buttonReading;

            if (buttonState == HIGH)
            {
                Serial.println("Button pressed. Resetting device...");
                delay(10000);

                wifiManager.setAPCallback([](WiFiManager *myWiFiManager)
                                          {
          Serial.println("Configuring access point...");
          Serial.println(myWiFiManager->getConfigPortalSSID());
          Serial.println("AP IP address:");
          Serial.println(WiFi.softAPIP()); });

                wifiManager.startConfigPortal("KCMTEKNO-CiftKanal");

                Serial.println("Connected to WiFi");
                delay(1000);
            }
        }
    }

    lastButtonState = buttonReading;

    WiFiClient client;
    HTTPClient http;

    const char *url = "http://test.ykaan.com.tr/get-state";

    http.begin(client, url);

    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK)
    {
        String payload = http.getString();

        DynamicJsonDocument doc(2048);

        DeserializationError error = deserializeJson(doc, payload);

        if (error)
        {
            Serial.print("Deserialization error: ");
            Serial.println(error.c_str());
            Serial.println(payload);
        }
        else
        {
            if (doc.containsKey("lightState1"))
            {
                String lightState1 = doc["lightState1"].as<String>();
                Serial.print("lightState1: ");
                Serial.println(lightState1);

                if (lightState1 == "true")
                {
                    digitalWrite(ledpin1, HIGH);
                }
                else if (lightState1 == "false")
                {
                    digitalWrite(ledpin1, LOW);
                }
                else
                {
                    Serial.println("Invalid state value");
                }
            }
            else
            {
                Serial.println("state not found in JSON");
            }
            if (doc.containsKey("lightState2"))
            {
                String lightState2 = doc["lightState2"].as<String>();
                Serial.print("lightState2: ");
                Serial.println(lightState2);

                if (lightState2 == "true")
                {
                    digitalWrite(ledpin2, HIGH);
                }
                else if (lightState2 == "false")
                {
                    digitalWrite(ledpin2, LOW);
                }
                else
                {
                    Serial.println("Invalid state value");
                }
            }
            else
            {
                Serial.println("state not found in JSON");
            }
        }
    }
    else
    {
        Serial.print("HTTP request failed with error code: ");
        Serial.println(httpCode);
        reconnectWiFi();
    }

    http.end();

    int rRDL1 = digitalRead(LDR1) / 100;
    int rRDL2 = digitalRead(LDR2) / 100;

    delay(15);
}
