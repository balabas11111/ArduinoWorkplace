#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "EspSettingsBox.h"
//#include "MqttHelper.h"
//#include "WiFiHelper.h"
#include "AbstractEvent.h"

#include <ESP8266WebServer.h>
#include "LedB.h"
#include "ButtonB.h"
#include "AnalogSensor.h"
#include "Pir.h"

#define FIRMVARE_VERSION "b@l@bas-soft SENSORZ v0.0.5"
//Robot Dyn//vem  // 12E
#define D0_PIN 16 //GPIO016 ////beeper
#define D1_PIN 5  //GPIO05  //DallasTemp
#define D2_PIN 4  //GPIO04  //OLED //SDA //blue
#define D3_PIN 0  //GPIO00  //OLED //SCL //green
#define D4_PIN 2  //GPIO02  //RedLed
#define D5_PIN 14 //GPIO14  //DHT
#define D6_PIN 12 //GPIO12  //DallasTemp red led2
#define D7_PIN 13 //GPIO13  //GreenLed
#define D8_PIN 15 //GPIO15  //button
#define RX_PIN 3
#define TX_PIN 1

#define SD3_PIN 10
#define SD2_PIN 9

const char *ssid = "balabasKiev5"; // ��� �������
const char *pass = "wuWylKegayg2wu22"; // ������ �������

const char *mqtt_server = "m23.cloudmqtt.com"; // ��� ������� MQTT
const int mqtt_port = 10186; // ���� ��� ����������� � ������� MQTT
const char *mqtt_user = "tpheglmk"; // ���� ��� ����������� � ������� MQTT
const char *mqtt_pass = "QgYRfVzDhQ31"; // ������ ��� ����������� � ������� MQTT

const char *baseTopic="topic/";
const char *topic_sub_s1="topic/switch1";
const char *topic_sub_s2="topic/switch2";

const char *topicBasePath="topic";

#define BUFFER_SIZE 100

WiFiClient wclient;
ESP8266WebServer server ( 80 );
PubSubClient client(mqtt_server, mqtt_port, callback, wclient);

EspSettingsBox espSettingsBox("/values.txt",true);
/*
WiFiHelper wiFiHelper(&espSettingsBox,&clearDisplay,displayLineFunction,&ledOnFunction,&ledOffFunction,&setupServerUrlsFunction);

MqttHelper mqttHelper(&espSettingsBox,processEvent,wclient);
*/
void clearDisplay(){};
void displayLineFunction(String message,int row,int column){};
void ledOnFunction(){};
void ledOffFunction(){};
void setupServerUrlsFunction(ESP8266WebServer server){};

ButtonB button_1("Switch1",D7_PIN,processEvent,false);
ButtonB button_2("Switch2",D6_PIN,processEvent,false);

LedB Switch_1("Lamp1",D5_PIN,button_1.getPressed(),processEvent);
LedB Switch_2("Lamp1",D8_PIN,button_2.getPressed(),processEvent);
AnalogSensor ldrSensor("lightLevSens",processEvent,100);
//Pir pir("IrDetector",D3_PIN,processEvent,true);

String processEvent(AbstractEvent event){
	//PIN_SC:01:001:onOffEvent:Button1
	//BUILTIN_LED
	/*
	 * test Urls
		http://192.168.0.178/runCommand?command=PIN_SC:14:1:Switch1:dispatchedByHtml
		http://192.168.0.178/runCommand?command=PIN_SC:14:0:Switch1:dispatchedByHtml
		http://192.168.0.178/runCommand?command=PIN_SC:15:1:Switch1:dispatchedByHtml
		http://192.168.0.178/runCommand?command=PIN_SC:15:0:Switch1:dispatchedByHtml
		http://192.168.0.178/runCommand?command=PIN_SC:15:0:Switch1:dispatchedByHtml

		http://192.168.0.178/runCommand?command=GET_ANALOG:17:0:lightLevSens:extCommand
		http://192.168.0.178/runCommand?command=GET_PIR_STATE:0:0:IrDetector:extCommand
		http://192.168.0.178/runCommand?command=PIN_GET_STATE:14:0:IrDetector:extCommand
		http://192.168.0.178/runCommand?command=GET_PIR_STATE:15:0:IrDetector:extCommand

	*/

	Serial.println("ProcessCommand "+event.getEventText());

	if(!event.isValid()){
		return "InvalidEvent(event="+event.print()+")";
	}

	if(event.isEventOfKind(COMMAND_PREFFIX_ANALOG_STATECHANGED)
			|| event.isEventOfKind(COMMAND_PREFFIX_ANALOG_STATE)
			|| event.isEventOfKind(PIN_CURRENT_STATE)
			|| event.isEventOfKind(COMMAND_PIR_STATE)){

		sendEventToTopic(event);

		return "Ok";
	}

	if(event.isEventOfKind(COMMAND_GET_PIR_STATE)){
		/*boolean result=pir.processEvent(event);
		return (result)?"Ok":"NotFound";
		*/
	}

	if(event.isEventOfKind(COMMAND_GET_ANALOG_DATA)){
		boolean result=ldrSensor.processEvent(event);
		return (result)?"Ok":"NotFound";
	}

	if(event.isEventOfKind(PIN_STATE_CHANGE_PREFFIX) || event.isEventOfKind(PIN_GET_STATE)){
		if(Switch_1.processEvent(event)){
			return "Ok";
		}

		if(Switch_2.processEvent(event)){
			return "Ok";
		}
	}

	if(event.isEventOfKind(COMMAND_PREFFIX_BUTTON_STATECHANGED)){
		int targetPin=-1;

		if(button_1.isDispatcherOfEvent(event)){
			targetPin=Switch_1.getPin();
		}else
			if(button_2.isDispatcherOfEvent(event)){
				targetPin=Switch_2.getPin();
			}

		AbstractEvent newEvent=AbstractEvent(PIN_STATE_CHANGE_PREFFIX,
												targetPin,
												event.getIntCommand(),
												"dispatchedByButton",event.getDispatcher());
		sendEventToTopic(event);
		return "NewEventDispatched:"+processEvent(newEvent);
	}

	return "NotFound";

}

void sendEventToTopic(AbstractEvent event){

	if(event.isValid()){
		String topic=String(baseTopic)+event.getDispatcher();
		String msg=event.getEventText();

		publishToTopic((char*) topic.c_str(),msg);
	}
}

void setup() {
  Serial.begin(115200);
  delay(10);

  setupWiFi();
  setupServer();

  Serial.println("Starting mqtt server");
  Serial.println();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
	connectMqttIfNotConnected();
	processMqtt();
  }

  button_1.loop();
  button_2.loop();
  ldrSensor.loop();
  //pir.loop();

  server.handleClient();
}


void sendData() {
	static int counter = 0;

	  String payload = "{\"micros\":";
	  payload += micros();
	  payload += ",\"counter\":";
	  payload += counter;
	  payload += "}";

	  publishToTopic(baseTopic,payload);

	  ++counter;
	  delay(5000);

  delay(1);
}

boolean publishToTopic(const char *topic, String payload){

	boolean result=false;

	if (client.connected()){
		Serial.print("Sending payload: ");
		Serial.println(payload);

		result=client.publish(topic, (char*) payload.c_str());

		if (result) {
		  Serial.println("Publish ok");
		}
		else {
		  Serial.println("Publish failed");
		}
    }

	return result;
}

// ������� ��������� ������ �� �������
void callback(char* topic, uint8_t* payload, unsigned int length) {

	String payloadIn = (char*)payload;
	String topicIn=String(topic);

	String msg;
		for(int i=0;i<length;i++){
			msg+=(char)payload[i];
		}

	String messageIn="Message received payload="+msg+" topic="+topic +" length="+length;

	Serial.println(messageIn);
	String result=processEvent(AbstractEvent(msg));
}

void processHttpCommand(){
	if(server.args()!=0 && server.hasArg("command")){
		String command=server.arg("command");

		//AbstractEvent event=AbstractEvent(command);
		String result=processEvent(AbstractEvent(command));

		delay(1);
		server.send ( 200, "text/html", "{\"status\":\"Ok\",\"command\":"+command+",\"result\":"+result+"}" );
	}

	server.send(500,"text/html","{\"status\":\"Failed\",\"command\":\"No command received\"}");
}

void processMqtt(){
	if (client.connected()){
	  client.loop();
	  //sendData();
	}
}

boolean isMqttConnected(){
	return client.connected();
}

void connectMqttIfNotConnected(){
	String clState=String(client.state());
	String clConnected=String(client.connected());
	String wifiState=String(wclient.status());
	String wifiConnected=String(wclient.connected());

	//Serial.println("Client State="+clState+" clConnected="+clConnected+" wifiState="+wifiState+" wifiConnected"+wifiConnected);

	if (!client.connected()) {

	    	  String clientName;
	    	  clientName += "esp8266 "+millis();

	    	if (client.connect((char*) clientName.c_str(),mqtt_user,mqtt_pass)) {
	    	    Serial.println("Connected to MQTT broker");
	    	    Serial.print("Base Publish Topic is: ");
	    	    Serial.println(baseTopic);

	    	    if (client.publish(baseTopic, "hello from ESP8266")) {
	    	      Serial.println("Publish ok");
	    	    }
	    	    else {
	    	      Serial.println("Publish failed");
	    	    }

	    	    subscribeToTopic(topic_sub_s1);
	    	    subscribeToTopic(topic_sub_s2);
	    	  }
	    	  else {
	    	    Serial.println("MQTT connect failed");
	    	    Serial.println("Will reset and try again...");
	    	    delay(5000);
	    	    //abort();
	    	  }

	    	Serial.println("IP="+WiFi.localIP().toString());
	    }
}

void subscribeToTopic(const char *topicIn){
	Serial.print("Subscribing to "+String(topicIn));
	client.subscribe(topicIn);
	Serial.println("   ...Subscribed");
}

void setupWiFi(){
	Serial.println("-----Setup WiFi-----");

	if(isWiFIConnected()){
		Serial.println("Disconnecting wifi");
		WiFi.disconnect(0);
	}

	WiFi.mode(WIFI_STA);
	WiFi.config(espSettingsBox.localIp,espSettingsBox.gateIp,
			espSettingsBox.subnetIp,espSettingsBox.dnsIp,espSettingsBox.dnsIp2);
	WiFi.begin ( const_cast<char*>(espSettingsBox.ssid.c_str()),
			const_cast<char*>(espSettingsBox.password.c_str()) );

	Serial.println ( "" );
	Serial.print ( "Connected to " );
	Serial.println ( WiFi.SSID() );
	Serial.print ( "IP address: " );
	Serial.println ( WiFi.localIP() );

}

void connectWifiIfNotConnected(){

	Serial.println("connectWifiIfNotConnected");

	while(!isWiFIConnected()){
		connectWiFi();

		if(!isWiFIConnected()){
			delay(5000);
		}
	}
}

boolean isWiFIConnected(){
	return WiFi.status() == WL_CONNECTED;
}

void connectWiFi(){
	if (!isWiFIConnected()) {
	    Serial.print("Connecting to ");
	    Serial.print(ssid);
	    Serial.println("...");
	    WiFi.begin(ssid, pass);

	    if (WiFi.waitForConnectResult() != WL_CONNECTED) return;
	    Serial.println("WiFi connected");

		server.on ( "/runCommand", HTTP_GET, processHttpCommand );
		server.begin();

		Serial.println(WiFi.localIP());
	  }
}

void setupServer(){
	Serial.println("SetupServer");

	server.on ( "/runCommand", HTTP_GET, processHttpCommand );
	server.begin();

	Serial.println(WiFi.localIP());
}
