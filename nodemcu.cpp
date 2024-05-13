#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>

// Pin tanımları
const int ledpin1 = D1;     // Birinci LED'in pin numarası
const int ledpin2 = D2;     // İkinci LED'in pin numarası
const int buttonPin16 = 16; // Butonun pin numarası
const int LDR1 = D3;        // Birinci LDR sensörünün pin numarası
const int LDR2 = D4;        // İkinci LDR sensörünün pin numarası

// Buton durumu değişkenleri
bool buttonState = HIGH;
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

WiFiManager wifiManager; // WiFi yöneticisi nesnesi

// Cihazı yeniden başlatan fonksiyon
void resetDevice()
{
    ESP.restart();
}

void setup()
{
    Serial.begin(9600); // Seri haberleşme başlatılıyor

    // Pin modları ayarlanıyor
    pinMode(ledpin1, OUTPUT);
    pinMode(ledpin2, OUTPUT);
    pinMode(buttonPin16, INPUT_PULLUP);

    wifiManager.setConnectTimeout(10);         // Bağlantı süresi sınırı
    wifiManager.setConfigPortalTimeout(0);     // Konfigürasyon portalı süresi sınırı

    // Eğer daha önce bağlanılan bir ağ varsa, o ağa bağlanılıyor
    if (!WiFi.SSID().isEmpty())
    {
        Serial.print("Last connected network: ");
        Serial.println(WiFi.SSID());
        Serial.println("Connecting to last connected network...");
        WiFi.begin();
    }
}

// WiFi bağlantısını yeniden sağlayan fonksiyon
void reconnectWiFi()
{
    Serial.println("WiFi connection lost. Reconnecting...");
}

void loop()
{
    // WiFi bağlantısı kontrol ediliyor
    if (WiFi.status() != WL_CONNECTED)
    {
        reconnectWiFi();
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("WiFi connecting.");
    }

    // Buton durumu okunuyor
    int buttonReading = digitalRead(buttonPin16);

    // Buton debouncing işlemi
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
                // Buton basıldığında cihaz yeniden başlatılıyor
                Serial.println("Button pressed. Resetting device...");
                delay(10000); // 10 saniye bekleme süresi

                // WiFi yöneticisinin AP (Erişim Noktası) olayı tanımlanıyor
                wifiManager.setAPCallback([](WiFiManager *myWiFiManager)
                                          {
                                              Serial.println("Configuring access point...");
                                              Serial.println(myWiFiManager->getConfigPortalSSID());
                                              Serial.println("AP IP address:");
                                              Serial.println(WiFi.softAPIP());
                                          });

                // Konfigürasyon portalı başlatılıyor
                wifiManager.startConfigPortal("KCMTEKNO-CiftKanal");

                Serial.println("Connected to WiFi");
                delay(1000);
            }
        }
    }

    lastButtonState = buttonReading;

    // HTTPClient ve WiFiClient nesneleri oluşturuluyor
    WiFiClient client;
    HTTPClient http;

    const char *url = "http://test.ykaan.com.tr/get-state";

    // HTTP isteği başlatılıyor
    http.begin(client, url);

    // HTTP isteği yapılıyor
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK)
    {
        String payload = http.getString();

        // Alınan JSON verisi deserialize ediliyor
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
            // JSON verisi işleniyor
            if (doc.containsKey("lightState1"))
            {
                String lightState1 = doc["lightState1"].as<String>();
                Serial.print("lightState1: ");
                Serial.println(lightState1);

                // Birinci LED durumu ayarlanıyor
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

                // İkinci LED durumu ayarlanıyor
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
        // HTTP isteği başarısız olursa
        Serial.print("HTTP request failed with error code: ");
        Serial.println(httpCode);
        reconnectWiFi();
    }

    http.end();

    // LDR'lerin değerleri okunuyor
    int rRDL1 = digitalRead(LDR1) / 100;
    int rRDL2 = digitalRead(LDR2) / 100;

    delay(15); // Döngüyü 15 milisaniye beklet
}
