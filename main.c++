#include <DHT.h>

// Define o pino do sensor e o tipo de sensor DHT
#define DHTPIN 12
#define DHTTYPE DHT22

// Define o pino do relé
#define RELE_PIN  14

// Instancia o sensor DHT
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);  // Inicializa a comunicação serial
  Serial.println("Controlando Bomba com DHT22 via Relé");

  dht.begin();  // Inicializa o sensor DHT
  pinMode(RELE_PIN, OUTPUT);  // Define o pino do relé como saída
  digitalWrite(RELE_PIN, LOW);  // Garante que o relé comece desligado

  Serial.println("Relé DESLIGADO, Bomba DESLIGADA");
}

void loop() {
  delay(2000);  // Aguarda 2 segundos entre as leituras

  float h = dht.readHumidity();  // Lê a umidade do sensor

  if (isnan(h)) {
    Serial.println("Falha ao ler o sensor DHT!");
    return;  // Se houver erro na leitura, interrompe o loop atual
  }

  Serial.print("Umidade: ");
  Serial.print(h);
  Serial.println(" %");

  // Se a umidade for menor que 50%, ativa a bomba
  if (h < 50) {
    digitalWrite(RELE_PIN, HIGH);  // Liga o relé
    Serial.println("Umidade abaixo de 50%, Bomba LIGADA (Relé ATIVADO)");
  } else {
    digitalWrite(RELE_PIN, LOW);  // Desliga o relé
    Serial.println("Umidade igual ou acima de 50%, Bomba DESLIGADA (Relé DESATIVADO)");
  }
}
