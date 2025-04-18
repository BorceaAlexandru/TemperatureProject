#include <DHT.h>
#include <WiFi.h>

//Router info
const char* ssid = "SG";
const char* pass = "Marmelada#99";

//Server port
WiFiServer server(80);

//HTTP request variable
String header;

#define dhtPin 4
#define DHTTYPE 11
DHT dht(dhtPin, DHTTYPE);

// Timeout setup
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

void setup() {
  dht.begin();
  Serial.begin(115200);

  // WiFi setup
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Show IP and start server
  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");
    String currentLine = "";

    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      currentTime = millis();

      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;

        if (c == '\n') {
          if (currentLine.length() == 0) {
            // Send HTTP response
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // HTML content
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<meta http-equiv=\"refresh\" content=\"2\">"); //Refresh la 2 sec ca sa se updateze informatia
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}</style></head>");
            client.println("<body><h1>~ Temperature Server ~</h1>");

            float temp = dht.readTemperature();
            float hum = dht.readHumidity();

            if (isnan(temp) || isnan(hum)) {
              client.println("<p><strong>Failed to read from DHT sensor!</strong></p>");
            } else {
              client.println("<p>Temperature: " + String(temp) + " °C</p>");
              client.println("<p>Humidity: " + String(hum) + " %</p>");
            }

            client.println("</body></html>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }

    // Cleanup
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }

  delay(2000);
}
