//==== Inclusão de bibliotecas ====//
#include <VirtualWire.h>             
 
//==== Mapemamento de Hardware e constantes =====//
#define pinLEDButton 4
#define pinStall 5  //acionamento para inversor no caso de Stall
#define pinNormal 6 //funcionamento normal
 
//===== Variáveis globais ========//
char message[VW_MAX_MESSAGE_LEN]; // Um buffer para armazenar as mensagens
char messageLength = VW_MAX_MESSAGE_LEN; // Tamanho máximo das mensagens recebidas

float vel_rx; 

 
void setup()
{
  //Inicia comunicação serialcom velocidade de 9600 BPS
  Serial.begin(9600);
  
  //Entradas/Saídas digitais
  pinMode(pinLEDButton, OUTPUT);
  pinMode(pinStall, OUTPUT);
  pinMode(pinNormal, OUTPUT);

  
  vw_set_rx_pin(2); //Seleciona o pino para recepção dos dados
  vw_setup(1000);   //Velocidade de comunicação em bps
  vw_rx_start();    //Começa a receber os dados

  
  
}//endSetup
 
void loop()
{
  if (vw_get_message(message, &messageLength))// Se houver dados para receber
  {
 
     
    if (message[0] == '1')// Se o primeiro byte do vetor message for igual a um...
    {
      digitalWrite(pinLEDButton, LOW);// Desliga o LED de telesinal
    }
    else 
      {
      digitalWrite(pinLEDButton, HIGH);// Liga o LED de telesinal
      }
     
    
    Serial.print(message[0]);
    Serial.print("  --  ");
    
    /*Serial.print(message[1]);
    Serial.print(message[2]);
    Serial.print(message[3]);
    Serial.print(message[4]);
    Serial.println(message[5]);*/

    String String_aux="";
    //concat() concatena os parametros (no caso um char) na string String_aux
    String_aux.concat(message[1]);
    String_aux.concat(message[2]);
    String_aux.concat(message[3]);
    String_aux.concat(message[4]);
    String_aux.concat(message[5]);

    Serial.print(String_aux);
    
    float vel_rx;
    vel_rx= String_aux.toFloat(); //converte de String para float
    //Serial.print(vel_rx);

    if (vel_rx >12)
    {
        digitalWrite(pinStall, HIGH);// Liga o acionamento de Stall
        digitalWrite(pinNormal, LOW);// Desliga o acionamento de Normal        
        Serial.println(" -- Stall");
    }
    else
    {
        digitalWrite(pinNormal, HIGH);// Ligo o acionamento de Normal
        digitalWrite(pinStall, LOW);// Desliga o acionamento de Stall        
        Serial.println(" -- Normal");        
    }
  
    

  }
 
}//endLoop
