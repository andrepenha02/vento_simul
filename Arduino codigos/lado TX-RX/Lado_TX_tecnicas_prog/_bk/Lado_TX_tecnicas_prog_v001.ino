//==== Inclusão de bibliotecas ====//
#include <VirtualWire.h>

//==== Mapemamento de Hardware e constantes =====//
#define Size 6 //6 bytes para enviar via RF. Pois sao 5 bytes da velocidade do vento "XX.XX" e mais um byte do botao de telesinal
#define pinButton 3
 
//===== Variáveis globais ========//
char TX_buffer[6];


char msg[8];
char velocidade_vento[5];
char c;
int  idx_msg=0;


void setup() {

  Serial.begin(9600);
  Serial.println("Inicializando a Serial...");

  
  //Entradas/Saídas digitais
  pinMode(pinButton, INPUT_PULLUP);
  vw_set_tx_pin(2); //Seleciona o pino de comunicação do TX 
  vw_setup(1000); // Velocidade de comunicação em bps
   velocidade_vento[0]='0';
   velocidade_vento[1]='0';
   velocidade_vento[2]='.';
   velocidade_vento[3]='0';
   velocidade_vento[4]='0';
  
  
}

void loop() { 


  if (Serial.available()){
     c=Serial.read();
    
    if (c!='\n'){
      msg[idx_msg]=c;
      idx_msg++;
    }    
    else{
      msg[idx_msg]='\0';   //fechamento da string com \0
      Serial.println(msg);
      idx_msg =0;

      velocidade_vento[0]=msg[0];
      velocidade_vento[1]=msg[1];
      velocidade_vento[2]=msg[2];
      velocidade_vento[3]=msg[3];
      velocidade_vento[4]=msg[4];

      //codido da comunicacao por RF
      //********************************
      TX_buffer[1] = velocidade_vento[0];
      TX_buffer[2] = velocidade_vento[1];
      TX_buffer[3] = velocidade_vento[2];
      TX_buffer[4] = velocidade_vento[3];
      TX_buffer[5] = velocidade_vento[4];
      //botao de telesinal
      if (digitalRead(pinButton) == 1) 
      {
        TX_buffer[0] = '1';
      }
      else 
      {
        TX_buffer[0] = '0';
      } 
      vw_send(TX_buffer, Size);
      vw_wait_tx();
      //final da comunicacao por RF
      //********************************
    }
  }

  



  
}




/*
char msg[8];
char c;
int  idx_msg=0;
void setup() {

  Serial.begin(9600);
  Serial.println("Inicializando a Serial...");
  
}

void loop() { 

  if (Serial.available()){
     c=Serial.read();
    
    if (idx_msg<8){
      msg[idx_msg]=c;
      idx_msg++;
    }    
    else{
      msg[idx_msg]='\0';   //fechamento da string com \0
      Serial.println(msg);
      idx_msg =0;
    }
  }
  
}

*/




/*
void setup() {
pinMode(13, OUTPUT);
digitalWrite(13,LOW);
Serial.begin(9600);
}

char valor;

void loop() {
  if (Serial.available()){
    valor = Serial.read();    
    if (valor=='k'){
      digitalWrite(13,HIGH);
    }
    if (valor=='n'){
      digitalWrite(13,LOW);
    }
  }
}

*/









/*
void setup(){
  Serial. begin(9600); //iniciando a serial com 9600 de baudrate
  Serial.println("Iniciando Serial...");
}

void loop(){
  Serial.println(millis()); //imprimir o tempo que o arduino está ligado
  delay(1000); //tempo entre as impressões
}

//imprimindo ao ligar o arduino "iniciando a serial"
//depois mostra o tempo que o arduino está ligado
//ao apertar reset tudo começa de novo
*/
