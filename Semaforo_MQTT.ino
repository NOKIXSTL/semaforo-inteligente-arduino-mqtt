#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

//Semaforo S1
int S1_VERDE = 4;
int S1_AMARELO = 3;
int S1_VERMELHO = 2;

//Semaforo S2
int S2_VERDE = 7;
int S2_AMARELO = 6;
int S2_VERMELHO = 5;

//Variaveis de Rede
byte mac[] = {0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED};
IPAddress ip(192, 168, 0, 168); //IP do Arduino
IPAddress server(192, 168, 0, 166); // IP do Servidor MQTT
EthernetClient ethClient;
PubSubClient client(ethClient);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem recebida [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT ...");
    if (client.connect("arduinoClient")) {
      Serial.println("conectado");
      client.publish("Semaforos","Semaforo Conectado");
      semaforo();
    } else {
      Serial.print("falhou, estado = ");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 1 segundos");
      delay(1000);
    }
  }
}

void semaforo(){
  while (client.connected()) {
    //time do semaforo 1
    int s1_VERDE = 10;
    int s1_AMARELO = 3;
    int s1_VERMELHO = 15;
    
    //tempo do semaforo 2
    int s2_VERDE = 10;
    int s2_AMARELO = 3;
    int s2_VERMELHO = 14; 

    //semaforo 1 - VERDE | semaforo 2 - VERMELHO
    digitalWrite(S1_VERDE, HIGH); 
    digitalWrite(S1_AMARELO, LOW); 
    digitalWrite(S1_VERMELHO, LOW); 
 
    digitalWrite(S2_VERDE, LOW); 
    digitalWrite(S2_AMARELO, LOW); 
    digitalWrite(S2_VERMELHO, HIGH); 
    
    for(;s1_VERDE > 0; s1_VERDE--){
      String str = "";
      str.concat("{");
        str.concat('"');str.concat("_id");str.concat('"');str.concat(":");str.concat("1");str.concat(",");
        str.concat('"');str.concat("tempo");str.concat('"');str.concat(":");str.concat(s1_VERDE);str.concat(",");
        str.concat('"');str.concat("sinal");str.concat('"');str.concat(":");str.concat('"');str.concat("VERDE");str.concat('"');
      str.concat("}");   
      Serial.println(str);
      int str_len = str.length() + 1; 
      char char_array[str_len];
      str.toCharArray(char_array, str_len);
      client.publish("semaforo", char_array);

      String str2 = "";
      str2.concat("{");
        str2.concat('"');str2.concat("_id");str2.concat('"');str2.concat(":");str2.concat("2");str2.concat(",");
        str2.concat('"');str2.concat("tempo");str2.concat('"');str2.concat(":");str2.concat(s2_VERMELHO);str2.concat(",");
        str2.concat('"');str2.concat("sinal");str2.concat('"');str2.concat(":");str2.concat('"');str2.concat("VERMELHO");str2.concat('"');
      str2.concat("}");   
      Serial.println(str2);
      
      int str2_len = str2.length() + 1; 
      char char2_array[str2_len];
      str2.toCharArray(char2_array, str2_len);
      client.publish("semaforo", char2_array);
      s2_VERMELHO--;
      delay(1000);  
    }

    //semaforo 1 - AMARELO | semaforo 2 - VERMELHO
    digitalWrite(S1_VERDE, LOW); 
    digitalWrite(S1_AMARELO, HIGH); 
    digitalWrite(S1_VERMELHO, LOW); 
    digitalWrite(S2_VERDE, LOW); 
    digitalWrite(S2_AMARELO, LOW); 
    digitalWrite(S2_VERMELHO, HIGH);
    for(;s1_AMARELO + 1 > 0; s1_AMARELO--){
      if(s1_AMARELO >= 1){
        String str = "";
        str.concat("{");
          str.concat('"');str.concat("_id");str.concat('"');str.concat(":");str.concat("1");str.concat(",");
          str.concat('"');str.concat("tempo");str.concat('"');str.concat(":");str.concat(s1_AMARELO);str.concat(",");
          str.concat('"');str.concat("sinal");str.concat('"');str.concat(":");str.concat('"');str.concat("AMARELO");str.concat('"');
        str.concat("}");   
        Serial.println(str);

        int str_len = str.length() + 1; 
        char char_array[str_len];
        str.toCharArray(char_array, str_len);
        client.publish("semaforo", char_array);
      } else {
        //Momento de delay de troca para iniciar verde
        //semaforo 1 - VERMELHO | semaforo 2 - VERMELHO
        digitalWrite(S1_AMARELO, LOW); 
        digitalWrite(S1_VERMELHO, HIGH); 
        s1_VERMELHO = 15;
        String str3 = "";
        str3.concat("{");
          str3.concat('"');str3.concat("_id");str3.concat('"');str3.concat(":");str3.concat("1");str3.concat(",");
          str3.concat('"');str3.concat("tempo");str3.concat('"');str3.concat(":");str3.concat(s1_VERMELHO);str3.concat(",");
          str3.concat('"');str3.concat("sinal");str3.concat('"');str3.concat(":");str3.concat('"');str3.concat("VERMELHO");str3.concat('"');
        str3.concat("}");   
        Serial.println(str3);
        int str3_len = str3.length() + 1; 
        char char3_array[str3_len];
        str3.toCharArray(char3_array, str3_len);
        client.publish("semaforo", char3_array);    
        s1_VERMELHO--;    
      }
      String str2 = "";
        str2.concat("{");
          str2.concat('"');str2.concat("_id");str2.concat('"');str2.concat(":");str2.concat("2");str2.concat(",");
          str2.concat('"');str2.concat("tempo");str2.concat('"');str2.concat(":");str2.concat(s2_VERMELHO);str2.concat(",");
          str2.concat('"');str2.concat("sinal");str2.concat('"');str2.concat(":");str2.concat('"');str2.concat("VERMELHO");str2.concat('"');
        str2.concat("}");   
      Serial.println(str2);
      int str2_len = str2.length() + 1; 
      char char2_array[str2_len];
      str2.toCharArray(char2_array, str2_len);
      client.publish("semaforo", char2_array);
      s2_VERMELHO--;
      delay(1000);  
    }

    //semaforo 1 - VERMELHO | semaforo 2 - VERDE
    digitalWrite(S1_VERDE, LOW); 
    digitalWrite(S1_AMARELO, LOW); 
    digitalWrite(S1_VERMELHO, HIGH);
    digitalWrite(S2_VERDE, HIGH); 
    digitalWrite(S2_AMARELO, LOW); 
    digitalWrite(S2_VERMELHO, LOW); 
    for(;s2_VERDE > 0; s2_VERDE--){
      String str = "";
      str.concat("{");
        str.concat('"');str.concat("_id");str.concat('"');str.concat(":");str.concat("1");str.concat(",");
        str.concat('"');str.concat("tempo");str.concat('"');str.concat(":");str.concat(s1_VERMELHO);str.concat(",");
        str.concat('"');str.concat("sinal");str.concat('"');str.concat(":");str.concat('"');str.concat("VERMELHO");str.concat('"');
      str.concat("}"); 
      Serial.println(str);
      int str_len = str.length() + 1; 
      char char_array[str_len];
      str.toCharArray(char_array, str_len);
      client.publish("semaforo", char_array);

      String str2 = "";
      str2.concat("{");
        str2.concat('"');str2.concat("_id");str2.concat('"');str2.concat(":");str2.concat("2");str2.concat(",");
        str2.concat('"');str2.concat("tempo");str2.concat('"');str2.concat(":");str2.concat(s2_VERDE);str2.concat(",");
        str2.concat('"');str2.concat("sinal");str2.concat('"');str2.concat(":");str2.concat('"');str2.concat("VERDE");str2.concat('"');
      str2.concat("}");
      Serial.println(str2);
      int str2_len = str2.length() + 1; 
      char char2_array[str2_len];
      str2.toCharArray(char2_array, str2_len);
      client.publish("semaforo", char2_array);
      s1_VERMELHO--;
      delay(1000);  
    }
    
    //semaforo 1 - VERMELHO | semaforo 2 - AMARELO
    digitalWrite(S1_VERDE, LOW); 
    digitalWrite(S1_AMARELO, LOW); 
    digitalWrite(S1_VERMELHO, HIGH); 
    digitalWrite(S2_VERDE, LOW); 
    digitalWrite(S2_AMARELO, HIGH); 
    digitalWrite(S2_VERMELHO, LOW);
    for(;s2_AMARELO + 1 > 0; s2_AMARELO--){
      String str = "";
      str.concat("{");
        str.concat('"');str.concat("_id");str.concat('"');str.concat(":");str.concat("1");str.concat(",");
        str.concat('"');str.concat("tempo");str.concat('"');str.concat(":");str.concat(s1_VERMELHO);str.concat(",");
        str.concat('"');str.concat("sinal");str.concat('"');str.concat(":");str.concat('"');str.concat("VERMELHO");str.concat('"');
      str.concat("}"); 
      Serial.println(str);
      int str_len = str.length() + 1; 
      char char_array[str_len];
      str.toCharArray(char_array, str_len);
      client.publish("semaforo", char_array);
      s1_VERMELHO--;
      if(s2_AMARELO >= 1){
        String str2 = "";
        str2.concat("{");
          str2.concat('"');str2.concat("_id");str2.concat('"');str2.concat(":");str2.concat("2");str2.concat(",");
          str2.concat('"');str2.concat("tempo");str2.concat('"');str2.concat(":");str2.concat(s2_AMARELO);str2.concat(",");
          str2.concat('"');str2.concat("sinal");str2.concat('"');str2.concat(":");str2.concat('"');str2.concat("AMERELO");str2.concat('"');
        str2.concat("}");     
        Serial.println(str2);
        int str2_len = str2.length() + 1; 
        char char2_array[str2_len];
        str2.toCharArray(char2_array, str2_len);
        client.publish("semaforo", char2_array);
      } else {
        //Momento de delay de troca para iniciar verde
        //semaforo 1 - VERMELHO | semaforo 2 - VERMELHO
        digitalWrite(S2_AMARELO, LOW); 
        digitalWrite(S2_VERMELHO, HIGH);
        s2_VERMELHO = 15;
        
        String str3 = "";
        str3.concat("{");
          str3.concat('"');str3.concat("_id");str3.concat('"');str3.concat(":");str3.concat("2");str3.concat(",");
          str3.concat('"');str3.concat("tempo");str3.concat('"');str3.concat(":");str3.concat(s2_VERMELHO);str3.concat(",");
          str3.concat('"');str3.concat("sinal");str3.concat('"');str3.concat(":");str3.concat('"');str3.concat("VERMELHO");str3.concat('"');
        str3.concat("}"); 
        Serial.println(str3);
        int str3_len = str3.length() + 1; 
        char char3_array[str3_len];
        str3.toCharArray(char3_array, str3_len);
        client.publish("semaforo", char3_array);
        s2_VERMELHO--;
      }
      delay(1000);
    }
 }
}

void setup() {
  Serial.begin(57600);

  //Iniciando portas do semaforo 1
  pinMode(S1_VERDE, OUTPUT);
  pinMode(S1_AMARELO, OUTPUT);
  pinMode(S1_VERMELHO, OUTPUT);

  //Iniciando portas do semaforo 2
  pinMode(S2_VERDE, OUTPUT);
  pinMode(S2_AMARELO, OUTPUT);
  pinMode(S2_VERMELHO, OUTPUT);

  //iniciando os dois semafomos no vermelho
  digitalWrite(S1_VERDE, LOW); 
  digitalWrite(S1_AMARELO, LOW); 
  digitalWrite(S1_VERMELHO, HIGH); 
  digitalWrite(S2_VERDE, LOW); 
  digitalWrite(S2_AMARELO, LOW); 
  digitalWrite(S2_VERMELHO, HIGH);

  //setando servidor
  client.setServer(server, 1883);
  client.setCallback(callback);

  //Setando configurações de rede
  Ethernet.begin(mac, ip);
  delay(1500);
}

void loop() { 
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
