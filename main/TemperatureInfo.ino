#include <DHT.h>

#define dhtPin 4
#define DHTTYPE 11
DHT dht(dhtPin, DHTTYPE);

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  Serial.println();  

  float umiditate=dht.readHumidity();
  Serial.print("Umiditate: ");
  Serial.println(umiditate);

  float temperatura=dht.readTemperature();
  Serial.print("Temperatura: ");
  Serial.println(temperatura);

  delay(2000);
}

