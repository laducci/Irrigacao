#include <WiFi.h> //biblioteca para Wi-Fi
#include "DHT.h"// biblioteca para o sensor DHT

#define DHTPIN 5
#define DHTTYPE DHT22
#define RELE_PIN 4

DHT dht(DHTPIN, DHTTYPE); //define o pino do sensor e o tipo de sensor DHT
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(RELE_PIN, OUTPUT);
  digitalWrite(RELE_PIN, HIGH);

  // cria uma rede Wi-Fi chamada "ESP_Temp"
  WiFi.softAP("ESP_Temp", "12345678");
  IPAddress IP = WiFi.softAPIP(); // obtém o IP do ponto de acesso
  Serial.print("Acesse via IP: ");
  Serial.println(IP); //mostra no Serial o IP do ponto de acesso

  server.begin();
}

void loop() {
  WiFiClient client = server.available(); // verifica se há um cliente conectado
  float temperatura = dht.readTemperature(); // lê a temperatura do sensor

  if (!isnan(temperatura)) {
    digitalWrite(RELE_PIN, temperatura > 21.0 ? LOW : HIGH);
  }

  if (client) {
    String html = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"; // resposta HTTP
    html += "<html><body><h1>Temperatura: ";
    html += isnan(temperatura) ? "Erro" : String(temperatura) + " °C"; // exibe a temperatura ou erro se a leitura falhar
    html += "</h1></body></html>";

    client.print(html); // envia a resposta ao cliente
    delay(1);
    client.stop(); // fecha a conexão com o cliente
  }

  delay(1000); // delay de 1 segundo entre as leituras
}