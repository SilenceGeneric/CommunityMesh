#include <LoRaMesh.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

// WiFi credentials
const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";

// LoRa settings
#define LORA_SCK 5
#define LORA_MISO 19
#define LORA_MOSI 27
#define LORA_SS 18
#define LORA_RST 14
#define LORA_DI0 26
#define LORA_FREQUENCY 915E6
#define LORA_MESH_ADDRESS 0x1

// HTTP server settings
#define HTTP_PORT 80
#define MAX_MESSAGE_LENGTH 50
#define MAX_MESSAGES 10

char loRaMessages[MAX_MESSAGES][MAX_MESSAGE_LENGTH];
int numMessages = 0;

AsyncWebServer server(HTTP_PORT);

void setup() {
    Serial.begin(9600);

    LoRa.setPins(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_SS, LORA_RST, LORA_DI0);
    if (!LoRa.begin(LORA_FREQUENCY)) {
        Serial.println("LoRa initialization failed.");
        while (true);
    }
    LoRa.enableCrc();
    LoRa.setSyncWord(0xFF);
    LoRa.meshSetAddress(LORA_MESH_ADDRESS);
    LoRa.meshSetChannel(0);
    LoRa.onReceive(receiveLoRaMessage);

    Serial.print("Connecting to WiFi: ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    server.on("/", handleRoot);
    server.on("/library", handleLibrary);
    server.begin();
    Serial.println("Web server started");
}

void loop() {
    LoRa.loop();
}

void receiveLoRaMessage(int packetSize) {
    if (packetSize == 0) {
        return;
    }

    String receivedMessage = "";
    while (LoRa.available()) {
        receivedMessage += (char)LoRa.read();
    }

    // Truncate the message if it's too long
    if (receivedMessage.length() >= MAX_MESSAGE_LENGTH) {
        receivedMessage = receivedMessage.substring(0, MAX_MESSAGE_LENGTH - 1);
    }

    // Check for duplicates before adding to the array
    for (int i = 0; i < numMessages; i++) {
        if (strcmp(loRaMessages[i], receivedMessage.c_str()) == 0) {
            Serial.println("Duplicate message received, not adding.");
            return;
        }
    }

    if (numMessages < MAX_MESSAGES) {
        strcpy(loRaMessages[numMessages++], receivedMessage.c_str());
    } else {
        // Delete the oldest message
        for (int i = 0; i < numMessages - 1; i++) {
            strcpy(loRaMessages[i], loRaMessages[i + 1]);
        }
        strcpy(loRaMessages[numMessages - 1], receivedMessage.c_str());
        Serial.println("Message board full, oldest message deleted");
    }
}

void handleRoot(AsyncWebServerRequest *request) {
    request->send(200, "text/html", "<h1>Library Access</h1><a href=\"/library\">Access Library</a>");
}

void handleLibrary(AsyncWebServerRequest *request) {
    String libraryContent = "<h1>Library Content</h1><ul>";
    
    for (int i = 0; i < numMessages; i++) {
        // Encode special characters in messages for HTML
        String encodedMessage = loRaMessages[i];
        encodedMessage.replace("&", "&amp;");
        encodedMessage.replace("<", "&lt;");
        encodedMessage.replace(">", "&gt;");
        libraryContent += "<li>" + encodedMessage + "</li>";
    }
    libraryContent += "</ul>";
    
    request->send(200, "text/html", libraryContent);
}
