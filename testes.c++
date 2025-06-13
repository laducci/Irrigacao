#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22
#define RELE_PIN 2

DHT dht(DHTPIN, DHTTYPE);

// Variáveis de teste
int totalLeituras = 0;
int falhas = 0;
unsigned long latenciaTotal = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando testes de desempenho...");
  
  dht.begin();
  pinMode(RELE_PIN, OUTPUT);
  digitalWrite(RELE_PIN, LOW);
  Serial.println("Relé DESLIGADO (estado inicial)");
}

void loop() {
  delay(2000); // Espera entre as leituras

  totalLeituras++;

  // Início da medição de latência
  unsigned long startTime = millis();
  float h = dht.readHumidity();
  unsigned long endTime = millis();
  unsigned long latencia = endTime - startTime;

  latenciaTotal += latencia;

  if (isnan(h)) {
    falhas++;
    Serial.println("Falha na leitura do sensor.");
    return;
  }

  // Exibe leitura
  Serial.print("Umidade: ");
  Serial.print(h);
  Serial.print("% | Latência: ");
  Serial.print(latencia);
  Serial.println(" ms");

  // Controle do relé com base na umidade
  if (h < 50) {
    digitalWrite(RELE_PIN, HIGH);
    Serial.println("Umidade baixa - Relé ATIVADO (bomba ligada)");
  } else {
    digitalWrite(RELE_PIN, LOW);
    Serial.println("Umidade adequada - Relé DESATIVADO (bomba desligada)");
  }

  // Após 10 leituras, calcula média
  if (totalLeituras == 10) {
    float eficiencia = ((10 - falhas) / 10) * 100.0;
    float mediaLatencia = latenciaTotal / 10;

    Serial.println("\nRESULTADO DOS TESTES (últimas 10 leituras)");
    Serial.print("Eficiência da comunicação: ");
    Serial.print(eficiencia);
    Serial.println(" %");

    Serial.print("Latência média: ");
    Serial.print(mediaLatencia);
    Serial.println(" ms");

    // Reseta contadores
    totalLeituras = 0;
    falhas = 0;
    latenciaTotal = 0;
    Serial.println("---------------------------\n");
  }
}
