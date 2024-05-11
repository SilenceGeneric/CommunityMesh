#include <LoRa.h>
#include <WiFi.h>

const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";

#define LORA_SCK 5
#define LORA_MISO 19
#define LORA_MOSI 27
#define LORA_SS 18
#define LORA_RST 14
#define LORA_DI0 26
#define LORA_FREQUENCY 915E6
#define LORA_MESH_ADDRESS 0x1

#define HTTP_PORT 80
#define MAX_MESSAGE_LENGTH 50
#define MAX_MESSAGES 10

char receivedMessages[MAX_MESSAGES][MAX_MESSAGE_LENGTH];
int numMessages = 0;

WiFiServer server(HTTP_PORT);
WiFiClient client;

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

  server.begin();
  Serial.println("Web server started");
}

void loop() {
  LoRa.loop();
  client = server.available();
  if (client) {
    handleWebServerRequest(client);
    client.stop();
  }
}

void receiveLoRaMessage(int packetSize) {
  if (packetSize == 0) {
    return;
  }
  String receivedMessage = "";
  while (LoRa.available()) {
    receivedMessage += (char)LoRa.read();
  }
  if (numMessages < MAX_MESSAGES) {
    strcpy(receivedMessages[numMessages++], receivedMessage.c_str());
  } else {
    // Delete the oldest message
    for (int i = 0; i < numMessages - 1; i++) {
      strcpy(receivedMessages[i], receivedMessages[i + 1]);
    }
    strcpy(receivedMessages[numMessages - 1], receivedMessage.c_str());
    Serial.println("Message board full, oldest message deleted");
  }
}

void sendLoRaMessage(String message) {
  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket();
}

void handleWebServerRequest(WiFiClient client) {
  String request = client.readStringUntil('\r');
  if (request.indexOf("/messages") != -1) {
    String response = "<h1>Received Messages</h1><ul>";
    for (int i = 0; i < numMessages; i++) {
      response += "<li>" + String(receivedMessages[i]) + "</li>";
    }
    response += "</ul>";
    sendHttpResponse(client, "200 OK", "text/html", response);
  } else if (request.indexOf("/send") != -1) {
    String response;
    if (client.available()) {
      String message = client.readStringUntil('\r');
      int messageStartIndex = message.indexOf('=') + 1; // Assuming message data follows a key-value format (e.g., message=Hello)
      if (messageStartIndex > 0) {
        String userMessage = message.substring(messageStartIndex);
        sendLoRaMessage(userMessage); // Call the function to send the message via LoRa
        response = "Message sent successfully!";
      } else {
        response = "Invalid message format.";
      }
    } else {
      response = "No message data received.";
    }
    sendHttpResponse(client, "200 OK", "text/html", response);
  }
}

void sendHttpResponse(WiFiClient client, String status, String contentType, String content) {
  client.println("HTTP/1.1 " + status);
  client.println("Content-Type: " + contentType);
  client.println("Connection: close");
  client.println("Content-Length: " + String(content.length()));
  client.println();
  client.println(content);
}
