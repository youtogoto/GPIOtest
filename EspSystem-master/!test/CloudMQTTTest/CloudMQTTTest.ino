#include <WiFi.h>
#include <PubSubClient.h>

// �ڑ����SSID�ƃp�X���[�h
const char ssid[] = "XXXXXXXXXX";
const char password[] = "XXXXXXXXXX";

// �����Őݒ肵�� CloudMQTT.com �T�C�g�� Instance info ����擾
const char* mqttServer = "m15.cloudmqtt.com";
const char* mqttDeviceId = "RoboCar";
const char* mqttUser = "niccngso";
const char* mqttPassword = "6UKWREecCBYB";
const int mqttPort = 13415;

// Subscribe ���� MQTT Topic ��
const char* mqttTopic_Signal = "KM/Signal";
const char* mqttTopic_Query = "KM/Query";

//Connect WiFi Client and MQTT(PubSub) Client
WiFiClient espClient;
PubSubClient g_mqtt_client(espClient);

void MQTT_callback(char* topic, byte* payload, unsigned int length) 
{
	Serial.print("received MQTT topic:"); 
	Serial.println(topic); 
	Serial.print("length = "); 
	Serial.println(length); 
	Serial.print("Payload : ");
	Serial.println((char*)payload);
	
	Serial.println();
}

// MQTT Client ���ڑ��ł��Ȃ�������ڑ��ł���܂ōĐڑ������݂邽�߂� MQTT_reconnect �֐�
void MQTT_reconnect() 
{
	// Loop until we're reconnected
	while (!g_mqtt_client.connected()) {
		Serial.print("Attempting MQTT connection...");
		// Attempt to connect
		if (g_mqtt_client.connect(mqttDeviceId, mqttUser, mqttPassword)) {
			Serial.println("connected");
			g_mqtt_client.subscribe(mqttTopic_Signal);
		} 
		else {
			Serial.print("failed, rc=");
			Serial.print(g_mqtt_client.state());
			Serial.println(" try again in 5 seconds");
			// Wait 5sec before retrying
			delay(1000);
		}
	}
}

void MQTT_publish_query()
{
	char payload[64];
	sprintf(payload, "MQTT test query by Nishimura");
	// MQTT broker��publish
	g_mqtt_client.publish(mqttTopic_Query, payload);
}



void setup() 
{
    Serial.begin(9600);

    // Connect WiFi
    WiFi.begin(ssid, password);

	int cnt = 0;
	int is_success = true;
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print("WiFi connecting to ");
		Serial.println(ssid);
		cnt ++;
		if(cnt > 30) {
			is_success = false;
			break;
		}
	}
	if(is_success) {
		// WiFi �A�N�Z�X�|�C���g����t�^���ꂽIP�A�h���X��\��
		Serial.print("Connected to the WiFi network. IP address: ");
		Serial.println(WiFi.localIP());
		// MQTT Server�̐ݒ�
		g_mqtt_client.setServer(mqttServer, mqttPort);
		// topic��subscribe�����Ƃ��̃R�[���o�b�N�֐���o�^
		g_mqtt_client.setCallback(MQTT_callback);
		// MQTT broker�Ƃ̐ڑ�
		MQTT_reconnect();
	}
	else {
		Serial.println("[Errol] Cannot connected to the WiFi network");
	}

}

int count = 0;
void loop() 
{
	if(!g_mqtt_client.connected()) {
		MQTT_reconnect();
	}
	g_mqtt_client.loop();
	
	count ++;
	if(count >= 10) {
		MQTT_publish_query();
		count = 0;
	}
	
	delay(200);
}

