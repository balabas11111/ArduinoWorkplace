/*
 * ButtonB.h
 *
 *  Created on: 13 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_TIMETRIGGER_WIFIHELPER_H_
#define LIBRARIES_TIMETRIGGER_WIFIHELPER_H_

#include <ESP_Consts.h>
#include "Arduino.h"
#include "Loopable.h"
#include "Initializable.h"
#include "EspSettingsBox.h"
#include "DisplayHelper.h"
#include "FS.h"

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <PinDigital.h>

#include <AbstractItem.h>

class WiFiHelper:public Initializable,public Loopable {

public:
	WiFiHelper(String _name,EspSettingsBox *_settingsBox, DisplayHelper *_displayHelper/*, PinDigital *_signalPin*/,
			ESP8266WebServer *_server,std::function<void(void)> _serverPostInitFunc,boolean _disconnectOnStartIfConnected){
		name=_name;
		espSettingsBox=_settingsBox;
		displayHelper=_displayHelper;
		//signalPin=_signalPin;
		serverPostInitFunc=_serverPostInitFunc;
		disconnectOnStartIfConnected=_disconnectOnStartIfConnected;
		server=_server;

		initStaticPages=true;
		initFilesManager=true;

		initialized=false;
	}

	boolean initialize(boolean _init){
		if(_init){
			Serial.println(FPSTR(MESSAGE_WIFIHELPER_INIT_CONNECTION));

			displayDetails();
			if(WiFi.status()==WL_CONNECTED){
				Serial.println(FPSTR(MESSAGE_WIFIHELPER_WIFI_DISCONNECTING));
				WiFi.disconnect(1);
			}

			if(espSettingsBox->isAccesPoint){
				startAsAccessPoint();
			}else{
				startAsClient();
			}

			setupServer();
		}
		initialized=_init;
		return true;
	}

	boolean loop(){
		if(initialized){
			server->handleClient();
		}
		return initialized;
	}

	//---------------------------------------------------------------------------------------
	void setupServer(){

		Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
		Serial.println(FPSTR(MESSAGE_WIFIHELPER_SETUP_SERVER));

		Serial.print(FPSTR(MESSAGE_WIFIHELPER_WIFI_MODE_EQ));
		Serial.print(WiFi.getMode());
		Serial.print(FPSTR(MESSAGE_WIFIHELPER_GET_AUTO_CONNECT));
		Serial.print(WiFi.getAutoConnect());
		Serial.print(FPSTR(MESSAGE_WIFIHELPER_WIFI_STATUS_EQ));
		Serial.println(WiFi.status());

		server->onNotFound([this](){handleNotFound();});

		server->serveStatic("/", SPIFFS, String(FPSTR(ESPSETTINGSBOX_DEFAULT_PAGE)).c_str());

		if(initStaticPages){
			initStaticPagesInWebFolder();
		}

		if(initFilesManager){
			initFileManager();
		}

		if(serverPostInitFunc!=nullptr){
			Serial.println(FPSTR(MESSAGE_WIFIHELPER_POST_INIT_WEB_SERV_HANDLERS));
			serverPostInitFunc();
		}

		server->begin();

		Serial.println(FPSTR(MESSAGE_WIFIHELPER_SERVER_SETUP_COMPLETED));
		Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));

	}

	//-----------------------------------------------------------------------
	void initStaticPagesInWebFolder(){
		deployStaticFolder(FPSTR(ESPSETTINGSBOX_DEFAULT_WEB_FOLDER),
							FPSTR(ESPSETTINGSBOX_DEPLOY_EXT),
							"");
		deployStaticFolder(FPSTR(ESPSETTINGSBOX_SETTINGS_PATH),
									FPSTR(ESPSETTINGSBOX_DEPLOY_EXT),
									FPSTR(ESPSETTINGSBOX_SETTINGS_DEPLOY_PATH));
	}

	void deployStaticFolder(String basePath,String extensions,String baseUrl){
		Serial.print(FPSTR(MESSAGE_WIFIHELPER_DEPLOYING_PATH));
		Serial.print(basePath);
		Serial.print(FPSTR(MESSAGE_WIFIHELPER_AS_WEB_FILES));
		Serial.print(FPSTR(MESSAGE_WIFIHELPER_EXTENSIONS));
		Serial.print(extensions);
		Serial.print(FPSTR(MESSAGE_WIFIHELPER_TO_BE_DEPLOYED));
		Serial.print(FPSTR(MESSAGE_WIFIHELPER_BASE_URL_EQ));
		Serial.println(baseUrl);
		Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
		//        /js   /css     /html
		Dir dir=SPIFFS.openDir(basePath);

		while(dir.next()){
			File file=dir.openFile("r");

			String fileNameStr=String(file.name());

			String url=fileNameStr.substring(basePath.length());

			if(!url.startsWith("/")){
				url="/"+url;
			}else{
			}

			url=baseUrl+url;

			File f=dir.openFile("r");
			size_t size=f.size();
			f.close();

			uint8_t extIndex=fileNameStr.lastIndexOf(".");
			String extension=fileNameStr.substring(extIndex+1);

			boolean add=extensions.indexOf(extension)!=-1;

			if(add){
				Serial.print(FPSTR(MESSAGE_WIFIHELPER_ADDED_FILE));
				Serial.print(fileNameStr);
				Serial.print(FPSTR(MESSAGE_WIFIHELPER_SIZE_EQ));
				Serial.print(size);
				Serial.print(FPSTR(MESSAGE_WIFIHELPER_EXT_EQ));
				Serial.print(extension);
				Serial.print(FPSTR(MESSAGE_WIFIHELPER_URL_EQ));
				Serial.println(url);
				server->serveStatic(url.c_str(), SPIFFS, fileNameStr.c_str());
			}
		}
		Serial.println("-----------------------------------");
		//addFileHandlersForDir(base,basePath,baseUrl);
	}
	//-----------------------------------------------------------------------

	void handleNotFound(){
		/*if(!handleFileRead(server->uri()))
			  server->send(404, "text/plain", "FileNotFound");
*/
		server->send(404,"text/html", "notFound");
	}

	void handleTest(){
		server->send ( 200, "text/html", "I'm here" );
	}
	//-----------------------------------------------------------------------

	boolean startAsAccessPoint(){
		WiFi.mode(WIFI_AP);
		displayLine("Start",3,0);
		displayLine("       AccessPoint",5,0);
		Serial.println("STarting as access point");
		/*
		 WiFi.softAPConfig(espSettingsBox.apIp,espSettingsBox.gateIp,
				espSettingsBox.subnetIp);
				*/
		//WiFi.softAP(const_cast<char*>(espSettingsBox.ssidAP.c_str()),const_cast<char*>(espSettingsBox.passwordAP.c_str()));
		WiFi.softAP(const_cast<char*>(espSettingsBox->ssidAP.c_str()));
		//String apName="ESP "+ESP.getChipId();
		//WiFi.softAP(espSettingsBox.ssidAP);

		Serial.println ( "-----------" );
		Serial.print ( "SoftAP " );
		Serial.println ( espSettingsBox->ssidAP);
		Serial.print ( "IP address: " );
		Serial.println ( WiFi.softAPIP() );
		Serial.println ( "-----------" );

		Serial.print("ViFi status");
		Serial.println(WiFi.status());
		Serial.println ( "-----------------------------" );

		return true;
	}

	boolean startAsClient(){
		displayLine("connect to",2,0);
		displayLine(espSettingsBox->ssid,5,0);
		Serial.print("espSettingsBox.ssid=");
		Serial.println(espSettingsBox->ssid);
		Serial.print("espSettingsBox.password=");
		Serial.println(espSettingsBox->password);

		WiFi.disconnect(0);
		WiFi.mode(WIFI_STA);

		if(espSettingsBox->staticIp){
			WiFi.config(espSettingsBox->localIp,espSettingsBox->gateIp,
							espSettingsBox->subnetIp,espSettingsBox->dnsIp,espSettingsBox->dnsIp2);
		}else{
			WiFi.config(0U,0U,0U,espSettingsBox->dnsIp,espSettingsBox->dnsIp2);
		}

		WiFi.begin ( const_cast<char*>(espSettingsBox->ssid.c_str()),
				const_cast<char*>(espSettingsBox->password.c_str()) );

		// Wait for connection
		//if(signalPin!=nullptr){signalPin->turnOn();}

		connectToWiFiIfNotConnected();

		//if(signalPin!=nullptr){signalPin->turnOff();}

		return true;
	}

	boolean isWiFIConnected(){
		return WiFi.status() == WL_CONNECTED;
	}

	void connectToWiFiIfNotConnected(){
		uint8_t count=0;
		displayDetails();

		while(!isWiFIConnected() ){
			/*if(signalPin!=nullptr)
				signalPin->changeAndDelay(250);
				*/
			if(count==20){
				displayDetails();
				count=0;
			}else{
				Serial.print(".");
				delay(500);
				count++;
			}
			//
		}
		Serial.println();
		displayDetails();
	}

	String displayDetails(){
		delay(1);
		Serial.println("-----------wiFi diagnostic-------------------");
		WiFi.printDiag(Serial);

		wl_status_t status=WiFi.status();
		String statusStr="UNKNOWN";

		switch(status) {
		        case WL_CONNECTED:
		        	statusStr="WL_CONNECTED";
		        	break;
		        case WL_NO_SSID_AVAIL:
		        	statusStr="WL_NO_SSID_AVAIL";
		        	break;
		        case WL_CONNECT_FAILED:
		        	statusStr="WL_CONNECT_FAILED";
		        	break;
		        case WL_IDLE_STATUS:
		        	statusStr="WL_IDLE_STATUS";
		        	break;
		        case WL_SCAN_COMPLETED:
					statusStr="WL_SCAN_COMPLETED";
					break;
				default:
		        	statusStr="WL_DISCONNECTED";
		}
		Serial.print("Status=");
		Serial.println(statusStr);
		Serial.println ( "" );
		Serial.print ( "SSID: " );
		Serial.println ( WiFi.SSID() );

		Serial.print(" IP Type: " );

		if(espSettingsBox->staticIp){
			Serial.println ( " STATIC IP ");
		}else{
			Serial.println ( " DNS IP ");
		}

		Serial.print ( " IP : " );
		if(espSettingsBox->isAccesPoint){
			Serial.println ( WiFi.softAPIP() );
		}else{
			Serial.println ( WiFi.localIP() );
		}

		Serial.print(" Mac:");
		Serial.println(WiFi.macAddress());

		Serial.println("------------------------------");

		return statusStr;
	}

	void checkAuthentication(){
		if(espSettingsBox->settingsUser.length()!=0 && espSettingsBox->settingsPass.length()!=0){
			Serial.println("Authentication is REQUIRED for setupPage");
			if(!server->authenticate(const_cast<char*>(espSettingsBox->settingsUser.c_str()),
				const_cast<char*>(espSettingsBox->settingsPass.c_str()))){
				return server->requestAuthentication();
			}else{
				Serial.println("User entered correct User/pass");
			}
		}else{
			Serial.println("Authentication is not required for setupPage");
		}
	}
	//------------------------------
	//display helper functions
	boolean cleanDisplay(){
		return displayHelper->clearDisplay();
	}

	boolean displayLine(String str,int row,int col){
		return displayHelper->addStringToDisplay(str, row, col, name);
	}
	//-------------------------------
	String executeGetRequest(String url){

		if(url==""){
			return "";
		}

		Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
		Serial.print(FPSTR(MESSAGE_WIFIHELPER_GET));
		Serial.print(FPSTR(MESSAGE_WIFIHELPER_URL_EQ));
		Serial.println(url);

		http.begin(url);
		delay(1);

		int httpCode = http.GET();                                                                  //Send the request
		delay(1);

		Serial.print(FPSTR(MESSAGE_WIFIHELPER_HTTP_STATUS_EQ));
		Serial.println(httpCode);

		if (httpCode > 0) { //Check the returning code

		  String payload = http.getString();
		  delay(1);
		  /*
		  Serial.print(FPSTR(MESSAGE_WIFIHELPER_RESPONSE_EQ));
		  Serial.println(payload);
		  Serial.print(FPSTR(MESSAGE_HORIZONTAL_LINE));
*/
		  return payload;
		}

		Serial.print(FPSTR(MESSAGE_HORIZONTAL_LINE));

		http.end();

		return "";
	}

	String executeFormPostRequest(String url,String body){

			Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
			Serial.print(FPSTR(MESSAGE_WIFIHELPER_POST));
			Serial.print(FPSTR(MESSAGE_WIFIHELPER_URL_EQ));
			Serial.println(url);
			Serial.print(FPSTR(MESSAGE_WIFIHELPER_HTTP_BODY_EQ));
			Serial.println(body);

			boolean begin=http.begin(url);

			Serial.print(FPSTR(MESSAGE_WIFIHELPER_HTTP_BEGIN_EQ));
			Serial.println(begin);

			yield();

			http.addHeader("Content-Type","application/x-www-form-urlencoded");

			int httpCode = http.POST(body);
			yield();

			Serial.print(FPSTR(MESSAGE_WIFIHELPER_HTTP_STATUS_EQ));
			Serial.println(httpCode);

			if (httpCode > 0) {

			  String payload = http.getString();

			  Serial.print(FPSTR(MESSAGE_WIFIHELPER_RESPONSE_EQ));
			  Serial.println(payload);
			  Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));

			  return payload;
			}

			Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));

			http.end();

			return "";
		}
	//---------------------------------------------------------------------
		//fileManager section
		void initFileManager(){

			Serial.println("Deploying Filemanager /edit");
			Serial.println("-----------------------------------");
			#ifdef ESP8266
				Dir dir = SPIFFS.openDir("/");
				while (dir.next()) {
				  String fileName = dir.fileName();
				  size_t fileSize = dir.fileSize();
				  Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
				}
			#else
				listDir(SPIFFS, "/", 0);
			#endif

			server->on("/list", HTTP_GET, [this](){handleFileList();});
			//load editor
			server->on("/edit", HTTP_GET, [this](){
			if(!handleFileRead("/web/edit.htm")) server->send(404, "text/plain", "FileNotFound");
			});
			//create file
			server->on("/edit", HTTP_PUT, [this](){handleFileCreate();});
			//delete file
			server->on("/edit", HTTP_DELETE, [this](){handleFileDelete();});
			//first callback is called after the request has ended with all parsed arguments
			//second callback handles file uploads at that location
			server->on("/edit", HTTP_POST, [this](){ server->send(200, "text/plain", ""); }, [this](){handleFileUpload();});

			  //get heap status, analog input value and all GPIO statuses in one json call
			  server->on("/all", HTTP_GET, [this](){
			    String json = "{";
			    json += "\"heap\":"+String(ESP.getFreeHeap());
			    json += ", \"analog\":"+String(analogRead(A0));
			#ifdef ESP8266
			    json += ", \"gpio\":"+String((uint32_t)(((GPI | GPO) & 0xFFFF) | ((GP16I & 0x01) << 16)));
			#endif
			    json += "}";
			    server->send(200, "text/json", json);
			    json = String();
			  });

			  Serial.println("HTTP server started");
		}

		String formatBytes(size_t bytes){
		  if (bytes < 1024){
		    return String(bytes)+"B";
		  } else if(bytes < (1024 * 1024)){
		    return String(bytes/1024.0)+"KB";
		  } else if(bytes < (1024 * 1024 * 1024)){
		    return String(bytes/1024.0/1024.0)+"MB";
		  } else {
		    return String(bytes/1024.0/1024.0/1024.0)+"GB";
		  }
		}

		String getContentType(String filename){
		  if(server->hasArg("download")) return "application/octet-stream";
		  else if(filename.endsWith(".htm")) return "text/html";
		  else if(filename.endsWith(".html")) return "text/html";
		  else if(filename.endsWith(".css")) return "text/css";
		  else if(filename.endsWith(".js")) return "application/javascript";
		  else if(filename.endsWith(".png")) return "image/png";
		  else if(filename.endsWith(".gif")) return "image/gif";
		  else if(filename.endsWith(".jpg")) return "image/jpeg";
		  else if(filename.endsWith(".ico")) return "image/x-icon";
		  else if(filename.endsWith(".xml")) return "text/xml";
		  else if(filename.endsWith(".pdf")) return "application/x-pdf";
		  else if(filename.endsWith(".zip")) return "application/x-zip";
		  else if(filename.endsWith(".gz")) return "application/x-gzip";
		  return "text/plain";
		}

		bool handleFileRead(String path){
		  Serial.println("handleFileRead: " + path);
		  if(path.endsWith("/")) path += "index.htm";
		  String contentType = getContentType(path);
		  String pathWithGz = path + ".gz";
		  if(SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)){
		    if(SPIFFS.exists(pathWithGz))
		      path += ".gz";
		    File file = SPIFFS.open(path, "r");
		    //size_t sent =
		    server->streamFile(file, contentType);
		    file.close();
		    return true;
		  }
		  return false;
		}

		void handleFileUpload(){
		  if(server->uri() != "/edit") return;

		  HTTPUpload& upload = server->upload();

		  if(upload.status == UPLOAD_FILE_START){
		    String filename = upload.filename;
		    if(!filename.startsWith("/")) filename = "/"+filename;
		    Serial.print("handleFileUpload Name: "); Serial.println(filename);
		    fsUploadFile = SPIFFS.open(filename, "w");
		    filename = String();
		  } else if(upload.status == UPLOAD_FILE_WRITE){
		    //DBG_OUTPUT_PORT.print("handleFileUpload Data: "); DBG_OUTPUT_PORT.println(upload.currentSize);
		    if(fsUploadFile)
		      fsUploadFile.write(upload.buf, upload.currentSize);
		  } else if(upload.status == UPLOAD_FILE_END){
		    if(fsUploadFile)
		      fsUploadFile.close();
		    Serial.print("handleFileUpload Size: "); Serial.println(upload.totalSize);
		  }
		}

		void handleFileDelete(){
		  if(server->args() == 0) return server->send(500, "text/plain", "BAD ARGS");
		  String path = server->arg(0);
		  Serial.println("handleFileDelete: " + path);
		  if(path == "/")
		    return server->send(500, "text/plain", "BAD PATH");
		  if(!SPIFFS.exists(path))
		    return server->send(404, "text/plain", "FileNotFound");
		  SPIFFS.remove(path);
		  server->send(200, "text/plain", "");
		  path = String();
		}

		void handleFileCreate(){
		  if(server->args() == 0)
		    return server->send(500, "text/plain", "BAD ARGS");
		  String path = server->arg(0);
		  Serial.println("handleFileCreate: " + path);
		  if(path == "/")
		    return server->send(500, "text/plain", "BAD PATH");
		  if(SPIFFS.exists(path))
		    return server->send(500, "text/plain", "FILE EXISTS");
		  File file = SPIFFS.open(path, "w");
		  if(file)
		    file.close();
		  else
		    return server->send(500, "text/plain", "CREATE FAILED");
		  server->send(200, "text/plain", "");
		  path = String();
		}

		void returnFail(String msg) {
		  server->send(500, "text/plain", msg + "\r\n");
		}

		#ifdef ESP8266
		void handleFileList() {
		  if(!server->hasArg("dir")) {
		    returnFail("BAD ARGS Dir is missed in params");
		    return;
		  }

		  String path = server->arg("dir");
		  Serial.println("handleFileList: " + path);
		  Dir dir = SPIFFS.openDir(path);
		  path = String();

		  String output = "[";
		  while(dir.next()){
		    File entry = dir.openFile("r");
		    if (output != "[") output += ',';
		    bool isDir = false;
		    output += "{\"type\":\"";
		    output += (isDir)?"dir":"file";
		    output += "\",\"name\":\"";
		    output += String(entry.name()).substring(1);
		    output += "\"}";
		    entry.close();
		  }

		  output += "]";
		  server->send(200, "text/json", output);
		}
		#else
		void handleFileList() {
		  if(!server.hasArg("dir")) {
		    returnFail("BAD ARGS");
		    return;
		  }
		  String path = server.arg("dir");
		  if(path != "/" && !SPIFFS.exists((char *)path.c_str())) {
		    returnFail("BAD PATH");
		    return;
		  }
		  File dir = SPIFFS.open((char *)path.c_str());
		  path = String();
		  if(!dir.isDirectory()){
		    dir.close();
		    returnFail("NOT DIR");
		    return;
		  }
		  dir.rewindDirectory();

		  String output = "[";
		  for (int cnt = 0; true; ++cnt) {
		    File entry = dir.openNextFile();
		    if (!entry)
		    break;

		    if (cnt > 0)
		      output += ',';

		    output += "{\"type\":\"";
		    output += (entry.isDirectory()) ? "dir" : "file";
		    output += "\",\"name\":\"";
		    // Ignore '/' prefix
		    output += entry.name()+1;
		    output += "\"";
		    output += "}";
		    entry.close();
		  }
		  output += "]";
		  server.send(200, "text/json", output);
		  dir.close();
		}

		void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
		  DBG_OUTPUT_PORT.printf("Listing directory: %s\n", dirname);

		  File root = fs.open(dirname);
		  if (!root) {
		    DBG_OUTPUT_PORT.println("Failed to open directory");
		    return;
		  }
		  if (!root.isDirectory()) {
		    DBG_OUTPUT_PORT.println("Not a directory");
		    return;
		  }

		  File file = root.openNextFile();
		  while (file) {
		    if (file.isDirectory()) {
		      DBG_OUTPUT_PORT.print("  DIR : ");
		      DBG_OUTPUT_PORT.println(file.name());
		      if (levels) {
		        listDir(fs, file.name(), levels - 1);
		      }
		    } else {
		      DBG_OUTPUT_PORT.print("  FILE: ");
		      DBG_OUTPUT_PORT.print(file.name());
		      DBG_OUTPUT_PORT.print("  SIZE: ");
		      DBG_OUTPUT_PORT.println(file.size());
		    }
		    file = root.openNextFile();
		  }
		}
		#endif

		//end fileManager



private:
	ESP8266WebServer *server;
	String name;
	EspSettingsBox *espSettingsBox;
	DisplayHelper *displayHelper;
	//PinDigital *signalPin;
	std::function<void(void)> serverPostInitFunc;
	boolean disconnectOnStartIfConnected;

	//std::function<void(void)> handleHttpWidget;
	//std::function<PinEvent(PinEvent)> processPinEvent;

	boolean initStaticPages;
	boolean initFilesManager;

	File fsUploadFile;
	HTTPClient http;

};


#endif /* LIBRARIES_TIMETRIGGER_WIFIHELPER_H_ */
