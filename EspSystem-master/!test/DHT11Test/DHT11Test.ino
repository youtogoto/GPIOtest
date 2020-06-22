#include <WiFi.h>
#include "DHT.h"
#include <Ambient.h>

const char* ssid = "XXXXXXXX";
const char* password = "XXXXXXXX";

WiFiClient client;
Ambient ambient;

DHT dht(33,DHT11);  //2�ԃ|�[�g

unsigned int channelId = 20947; // Ambient�̃`���l��ID
const char* writeKey = "10cdb0759b1a687e"; // ���C�g�L�[

void setup() {
    Serial.begin(115200);
    while (!Serial) ;

    WiFi.begin(ssid, password);  // Wi-Fi�l�b�g���[�N�ɐڑ�����
    while (WiFi.status() != WL_CONNECTED) {  // �ڑ����������ׂ�
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());  // ���[�J��IP�A�h���X���v�����g����

    ambient.begin(channelId, writeKey, &client); // �`���l��ID�ƃ��C�g�L�[���w�肵��Ambient�̏�����

    dht.begin();
}

void loop() {
    float temp = dht.readHumidity();
    float humid = dht.readTemperature();
    Serial.printf("temp: %.2f, humid: %.2f\r\n", temp, humid);

    ambient.set(1, temp);  // Ambient�̃f�[�^1�ɉ��x���Z�b�g����
    ambient.set(2, humid);  // �f�[�^2�Ɏ��x���Z�b�g����
    ambient.send();  // Ambient�ɑ��M����

    delay(300 * 1000);
}
