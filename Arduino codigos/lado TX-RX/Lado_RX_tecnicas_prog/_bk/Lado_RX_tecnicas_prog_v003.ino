//==== Inclusão de bibliotecas ====//
#include <VirtualWire.h>             
 
//==== Mapemamento de Hardware e constantes =====//
#define pinLEDButton 4
#define pinStall 5  //acionamento para inversor no caso de Stall
#define pinNOStall 6 //funcionamento normal


#define pinRLMotor 11 //porta para acionar a rele que liga o motor
#define pinRDMotor 10 //porta para acionar a rele que desliga o motor
#define pinRLSentidoMov 12 //porta para acionar a rele de sentido de movimento do motor


#define pinSinalAEnc 9  //sinal A do encoder


 
//===== Variáveis globais ========//

int Revolucoes_ten=300;//numero de pulsos necessarios para 10 revolucoes no eixo do redutor, a ser definido experimentalmente 
                       //o encoder gera 30 pulsos por volta, mas devido a inercia e a curva de parada nao serao 300 pulsos 
                       //para se obter 10 revolucoes, e sim o valor menor

char message[VW_MAX_MESSAGE_LEN]; // Um buffer para armazenar as mensagens
char messageLength = VW_MAX_MESSAGE_LEN; // Tamanho máximo das mensagens recebidas
boolean Stall; //True para sistema em Stall, e False para sistema fora de Stall 
float vel_rx; //velocidade em m/s do vento recebido via RF


void setup()
{
  //Inicia comunicação serialcom velocidade de 9600 bps
  Serial.begin(9600);
  delay(1000); //esse parada de 1s serve para evitar problemas quando for regravar o arduino
  Serial.println("Inicializando a Serial Módulo RX...");

  
  //Entradas/Saídas digitais
  pinMode(pinLEDButton, OUTPUT);
  pinMode(pinStall, OUTPUT);
  pinMode(pinNOStall, OUTPUT);

  pinMode(pinRLMotor, OUTPUT);
  pinMode(pinRLSentidoMov, OUTPUT);  


  pinMode (pinSinalAEnc,INPUT); 
  

  
  vw_set_rx_pin(2); //Seleciona o pino para recepção dos dados
  vw_setup(1000);   //Velocidade de comunicação em bps
  vw_rx_start();    //Começa a receber os dados

  Stall = true; //sistema inicializa em Stall
  digitalWrite(pinStall, HIGH);// Liga a sinalizacao de status de Stall, pois o sistema inicializa em Stall

  //o modulo de relés funciona com logica negativa, por isso os pinos de controle do motor sao inicializado com HIGH
  digitalWrite(pinRLMotor, HIGH);//garante que o rele que liga o motor fica desligado ao iniciar o arduino
  digitalWrite(pinRDMotor, HIGH);//garante que o rele que desliga o motor fica desligado ao iniciar o arduino
  digitalWrite(pinRLSentidoMov, HIGH);//garante que o rele que controla o sentido de giro do  motor fica desligado ao iniciar o arduino
  
  
}//endSetup
 
void loop()
{
  if (vw_get_message(message, &messageLength)){// Se houver dados para receber
  
 
     
    if (message[0] == '1'){// Se o primeiro byte do vetor message for igual a um...
      digitalWrite(pinLEDButton, LOW);// Desliga o LED de telesinal
    }
    else{
      digitalWrite(pinLEDButton, HIGH);// Liga o LED de telesinal
    }
     
    Serial.print(message[0]);
    Serial.print("  --  ");
    
    String String_aux="";
    //concat() concatena os parametros (no caso um char) na string String_aux
    String_aux.concat(message[1]);
    String_aux.concat(message[2]);
    String_aux.concat(message[3]);
    String_aux.concat(message[4]);
    String_aux.concat(message[5]);

    
    
    float vel_rx;
    vel_rx= String_aux.toFloat(); //converte de String para float
    //Serial.print(vel_rx);

    Serial.print(String_aux);
    if (vel_rx >12){
      Serial.println(" -- Stall");
    }
    else{
      Serial.println(" -- NO_Stall");
    }
/*
    if (vel_rx >12){
      digitalWrite(pinStall, HIGH);// Liga o acionamento de Stall
      digitalWrite(pinNormal, LOW);// Desliga o acionamento de Normal        
      Serial.println(" -- Stall");
      
      func_Stall(); //funcao que executa as rotinas de Stall
    }
    else{
      digitalWrite(pinNormal, HIGH);// Ligo o acionamento de Normal
      digitalWrite(pinStall, LOW);// Desliga o acionamento de Stall        
      Serial.println(" -- Normal");        

      func_NO_Stall(); //funcao que cancela o Stall
    }
*/


  //core das rotinas de chamadas de funcoes principais
  if (vel_rx >12){
    if (Stall ==false){
      func_Stall(); //funcao que entre no modo o Stall       
    }    
  }
  else{
    if (Stall ==true){
      func_NO_Stall(); //funcao que cancela o Stall       
    }
  }
  

  
  }//end do get_message
}//endLoop



//funcao com as rotinas para colocar em Stall
int func_Stall (){
  Serial.println(" ***Iniciando movimento para Stall***");
  digitalWrite(pinNOStall, LOW);// Desliga a sinalizacao de status de NO_Stall  
  
  
  //ligando o motor. Pulso de um segundo
  digitalWrite(pinRLMotor, LOW); //liga rele de D1 do inversor  
  delay(1000);
  digitalWrite(pinRLMotor, HIGH);//desliga rele  de D1 do inversor
  
  digitalWrite(pinRLSentidoMov, HIGH); //desliga o rele de sentido de movimento para trocar o sentido do giro   
    
  func_leitura_encoder();

  //desligado o motor. Pulso de meio segundo  
  digitalWrite(pinRDMotor, LOW); //liga rele de D2 do inversor
  delay(500);
  digitalWrite(pinRDMotor, HIGH); //desliga rele de D2 do inversor


  
  digitalWrite(pinStall, HIGH);// Liga a sinalizacao de status de Stall
  
  Stall = true; //atualiza a variavel Stall
  Serial.println(" ***Movimentacao para Stall concluida***");
  return 0;
}


//funcao com as rotinas para sair do Stall
int func_NO_Stall (){

  Serial.println(" ***Iniciando movimento para NO Stall***");
  digitalWrite(pinStall, LOW);// Desliga a sinalizacao de status de Stalll        
  

  //ligando o motor. Pulso de um segundo
  digitalWrite(pinRLMotor, LOW); //liga rele de D1 do inversor  
  delay(1000);
  digitalWrite(pinRLMotor, HIGH);//desliga rele  de D1 do inversor
  
  digitalWrite(pinRLSentidoMov, LOW); //liga o rele de sentido de movimento para trocar o sentido do giro    
  
  func_leitura_encoder();

  digitalWrite(pinRLSentidoMov, HIGH); //desliga o rele de sentido de movimento para evitar o acionamento de 2 reles ao mesmo tempo
  //desligado o motor. Pulso de meio segundo  
  digitalWrite(pinRDMotor, LOW); //liga rele de D2 do inversor
  delay(500);
  digitalWrite(pinRDMotor, HIGH); //desliga rele de D2 do inversor



  
  digitalWrite(pinNOStall, HIGH);// Liga a sinalizacao de status de NO_Stall
  Stall = false; //atualiza a variavel Stall
  Serial.println(" ***Movimentacao para NO Stall concluida***");  
  return 0;
}



//funcao de leitura do encoder e controle de revolucoes
int func_leitura_encoder(){
  int encoderPosCount = 0; //numero de pulsos contados
  int pinALast;  //status do pino A anterior 
  int aVal; //novo status do pino A
  pinALast = digitalRead(pinSinalAEnc); //primeira leitura do sinal A para pinALast
  
  while (encoderPosCount<Revolucoes_ten){
    aVal = digitalRead(pinSinalAEnc); //leitura atual do pino A para aVal
    if (aVal != pinALast){ //detectada mudança no sinal A
      encoderPosCount ++; 
      Serial.print("Numero de Pulsos: "); 
      Serial.println(encoderPosCount);
    }
    pinALast = aVal; 
  }//end do while
  encoderPosCount=0;
  return 0;
}





