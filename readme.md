# NodeMCU ile Kontrol Edilen Çift Kanallı Aydınlatma Projesi

Bu proje, ESP8266 tabanlı bir NodeMCU kartı kullanarak çift kanallı bir aydınlatma sistemini kontrol etmek için geliştirilmiştir. Proje, bir düğme aracılığıyla donanım sıfırlaması yapmayı, bir WiFi ağına bağlanmayı ve bir RESTful API'ye istek yaparak iki ayrı ışık kanalını kontrol etmeyi içerir.

#### Website Linki : http://test.ykaan.com.tr


## Donanım Gereksinimleri

- ESP8266 tabanlı NodeMCU kartı
- İki adet LED (örneğin, kırmızı ve yeşil)
- Bir adet düğme
- İki adet LDR (Light Dependent Resistor)

## Yazılım Gereksinimleri

- Node.JS
- React
- MongoDB

## Kurulum Adımları

1. Arduino IDE veya uyumlu bir entegre geliştirme ortamı kullanarak nodemcu.cpp scriptini NodeMCU kartına yükleyin.
2. Kullanılan kütüphaneleri yükleyin:
   - ESP8266WiFi
   - ESP8266HTTPClient
   - ArduinoJson
   - WiFiManager
3. Devreyi oluşturun:
   - LED'leri ve LDR'leri NodeMCU kartına bağlayın.
   - Düğmeyi NodeMCU kartına bağlayın.

## Kodun Açıklaması

- Kod, WiFi ağına bağlanmayı, düğmeye basıldığında donanımı sıfırlamayı ve bir RESTful API'ye istek yapmayı içerir.
- Düğmeye basıldığında, NodeMCU cihazı WiFi erişim noktası moduna geçer ve kullanıcıyı ağa bağlanması için yönlendirir.
- API isteği yapılır ve gelen JSON yanıtı işlenerek LED'ler kontrol edilir.


## API Kullanımı

#### Kanal durumlarını al

```http
  GET /get-state
```

| Parametre | Tip     | Açıklama                |
| :-------- | :------- | :------------------------- |
| `api_key` | `string` | **Gerekli**. Kanalların durumunu getiren istek |

#### Kanal durumunu güncelle

```http
  POST /update-state
```

| Parametre | Tip     | Açıklama                       |
| :-------- | :------- | :-------------------------------- |
| `body`      | `object` | **Gerekli**. Kanalların durumunu taşıyan istek verisi |

## Örnek Kullanım Senaryosu

   - NodeMCU cihazı WiFi erişim noktası moduna geçer.
   - Kullanıcı ağa bağlanır.
   - Lokal'de ise (http://localhost:5000/) portundan, değil ise http://test.ykaan.com.tr adresinden kanal kontrolleri yapılır, ışıkların durumu güncellenir.
   



