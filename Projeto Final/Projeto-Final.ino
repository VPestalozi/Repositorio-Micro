// Parte do UART
#define FOSC 16000000U
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#define MOTOR (1<<PD6)// definindo o motor
//Sensor de gotas(Botton) no pd2
// Contador no pino pd3
#define PINO 4 // Pino do sensor de ultrassonico no pc4
#define BUZZER (1<<PD7); // Buzzer no pino pd7

// Variaveis para o funcionamento do UART
char msg_tx[20];
char msg_rx[32];
int pos_msg_rx = 0;
int tamanho_msg_rx = 3;
unsigned int x = 0; 

int valorVolume = 0, tempIndu = 0; // Valor do volume desejado e do tempo de indução
int contador =0, gotas = 0; // Variaveis para o calculo do fluxo real
float fluxoR; // Fluxo real a ser injetado
int erro; // Variavel que guarda o fator de erro do aparelho
unsigned int Leitura_AD; // Variavel responsavel para leitura do sensor
float tensaoSensorU; // Variavel que quarda a tensão do sensor Ultrassonico

//Variaveis para o calculo do fluxo
int fluxoDefinido =0;
int potencia = 0;

// Prototipos das funcoes
//void UART_Init(unsignedintubrr);
//voidUART_Transmit(char*dados);

ISR(USART_RX_vect){
  // Escreve o valor recebido pela UART na posição pos_msg_rx do buffer msg_rx 
  msg_rx[pos_msg_rx++]=UDR0;
  if(pos_msg_rx == tamanho_msg_rx)
    pos_msg_rx=0;
}

ISR(TIMER2_OVF_vect){ // Contador para calcular o tempo entre cada gota
  contador++;
}

ISR(INT0_vect){ // Realização da ativação ou desativação do contador. Alem de conter um contador de gotas
  if(TIMSK2 & (1<<TOIE2)==0){
    TIMSK2 |= (1<<TOIE2); // Ativação do contador 
  }
  else
    TIMSK2 &=~(1<<TIMSK2); // Desativação do contador
  gotas++;
}

void UART_Transmit(char* dados){
  // Envia todos os caracteres do buffer dados ate chegar um final de linha
  while(*dados != 0){
    // Aguarda a transmissão acabar
    while(!(UCSR0A&(1<<UDRE0)));
    // Escreve o caractere no registro de tranmissão
    UDR0=*dados;
    // Passa para o próximo caractere do buffer dados
    dados++;
  }
}
void UART_Init(unsigned int ubrr){
  //Configuraabaudrate
  UBRR0H = (unsigned char)(ubrr >> 8);
  UBRR0L = (unsigned char)ubrr;
  // Habilita a recepcao, tranmissao e interrupcao na recepcao
  UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
  // Configura o formato da mensagem: 8 bits de dados e 1bits de stop
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

int main(){
  UART_Init(MYUBRR);
  
  //Configuração do PWM do motor
  TCCR0A=(1<<WGM01)|(1<<WGM00)|(1<<COM0A1); // Modo fast PWM
  TCCR0B = 1; // no Prescaler
  //Configuração da interrupção do push-botton
  EICRA = (1<<ISC01)|(1<<ISC00);
  EIMSK = (1<<INT0);
  //Configuração do timer
  TCCR2A = 0;
  TCCR2B = (1<<CS02)|(1<<CS00);
  //Configuração do conversor ad
  ADMUX |=(1<<REFS0);
  ADCSRA |= (1<<ADEN);
  ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
  ADCSRB = 0; //Conversão Única
  DIDR0 = (1 << PINO);
  sei();
  
  // Volume emitido pelo usuario
  UART_Transmit("Entre com o volume:\n");
  x = 0;
  valorVolume = 0;
  while(x == 0){
    valorVolume = (msg_rx[0]-48)*100+(msg_rx[1]-48)*10+(msg_rx[2]-48)*1;
    if(valorVolume!=(msg_rx[0]=='0')&&(msg_rx[1]=='0')&&(msg_rx[2]=='0')){
      msg_rx[0]='1';
      msg_rx[1]='2';
      msg_rx[2]='2';
      x=1;
    }
  }

  // Tempo da Indusão em minutos emitida pelo usuario
  UART_Transmit("Entre com o tempo de Indusao em minutos:\n");
  x = 0;
  tempIndu = 0;
  while(x == 0){
    tempIndu = (msg_rx[0]-48)*100+(msg_rx[1]-48)*10+(msg_rx[2]-48)*1;
    if(tempIndu!=(msg_rx[0]=='0')&&(msg_rx[1]=='0')&&(msg_rx[2]=='0')){
      msg_rx[0]='1';
      msg_rx[1]='2';
      msg_rx[2]='2';
      x=1;
    }
  }
  x = 0;

  //Calculo do Fluxo necessario
  fluxoDefinido = valorVolume/tempIndu;

  //Potencia do motor
  potencia = (fluxoDefinido/27000)*255; // Calculo da potencia do motor
  PORTD &= ~MOTOR; // Iniciando o motor desligado
  OCR0A = 0; // Definição do comparador A
  
  //Calculo do Fluxo real
  fluxoR = (gotas/((contador/15625)+1))*0.05;

  //Calculo do erro
  erro = ((fluxoR-fluxoDefinido)/fluxoDefinido)*100;
  PORTD &= ~BUZZER; // Iniciando o buzzer desligado
  
  while(1){
    OCR0A = potencia; // Definindo o PWM do motor

    // Realização da transformação do sinal ADC em sinal digital atraves da media das amostras
    ADMUX = (ADMUX & 0xF8) | PINO;
    unsigned int SomaLeitura = 0, MediaLeitura;
    for(int i=0; i < 20; i++){
      ADCSRA |= (1 << ADSC); //Inicia a conversão
      while((ADCSRA & (1<<ADSC)) == (1<<ADSC)); //Esperar a conversão
      Leitura_AD = ADC;
      SomaLeitura += Leitura_AD;
    }
    MediaLeitura = SomaLeitura/20;
    tensaoSensorU = (MediaLeitura*5)/1023.0; //Cálculo da Tensão
    if(tensaoSensorU>3.75){ // BUZZER ira ligar quando a tensão for maior que 3.75 (detecção de bolhas(a distancia do ultrassom diminui))
      PORTD |= BUZZER; // Liga o BUZZER
    }
    if(tensaoSensorU<=3.75){ // Buzzer desliga quando a tensao for igual a 3.75(5 centimetros). Obs(Para fins praticos o buzzer ira desligar a tensões menores que 3.75)
      PORTD &= ~BUZZER; // Iniciando o motor desligado
    }
    
    //Apresentacao do erro em porcentagem
    UART_Transmit("Demonstracao do erro: ");
    itoa(erro,msg_tx,10);
    UART_Transmit(msg_tx);
    UART_Transmit(" %\n");
    
    //Releitura dos valores do fluxo desejado e da indução, caso o usuario deseje
    // Releitura do volume
    if((msg_rx[0]=='s')&&(msg_rx[1]=='i')&&(msg_rx[2]=='m')){
      UART_Transmit("Entre com o volume:\n");
      x = 0;
      valorVolume = 0;
      while(x == 0){
        valorVolume = (msg_rx[0]-48)*100+(msg_rx[1]-48)*10+(msg_rx[2]-48)*1;
        if(valorVolume!=000){
          x=1;
        }
      }

      // Releitura do Tempo da Indusão em minutos emitida pelo usuario
      UART_Transmit("Entre com o tempo de Indusao em minutos:\n");
      x = 0;
      tempIndu = 0;
      while(x == 0){
        tempIndu = (msg_rx[0]-48)*100+(msg_rx[1]-48)*10+(msg_rx[2]-48)*1;
        if(tempIndu!=000){
          x=1;
        }
      }
      x = 0;

      //Recalibrando a potencia
      potencia = (fluxoDefinido/27000)*255;
    }
  }
}
