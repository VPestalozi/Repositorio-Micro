//Declaração de biblioteca

//Declaração de constates
#define CHAVE (1<<PD3)
#define DESLIGA (1<<PC1)
#define LIGA (1<<PC0)
#define BAIXO (1<<PD4)
#define ALTO (1<<PD5)
#define BOMBA (1<<PB0)


//Declaração de variaveis globais

//Declaração de Funções

int main(){
  //Direcionamento das Portas (DDRx) - Modificar só os pinos de saída
  DDRB = BOMBA;//PB0 como saída (Bomba)
  DDRC = 0;//Todos os pinos do portal são entradas
  DDRD = 0; //Todos os pinos do portal são entradas

  //Habilita o circuito de PULL-UP nos botões NFs (PORTx)
  PORTC = DESLIGA; //PC1 com circuito de pull-up habilitado

  //Estabelecer a condição inicial das saidas
  PORTB = BOMBA; // Bomba desligada

  for(;;){//Loop Infinito

    //Leitura da Chave(PD3)
    short int chave;
    chave = PIND & CHAVE;

    if(chave == CHAVE){ // Se chave estiver no modo manual
      
      //Leitura dos botões
      short int liga,desliga;
      desliga = PINC & 0b00000010;
      liga = PINC & 0b00000001;

      if(desliga == 0b00000000)//Botão Desliga pressionado
        PORTB = 0b00000000; //Desliga a Bomba

      if(liga == 0b00000001) //Botão Liga pressionado
        PORTB = 0b00000001; //Liga a Bomba
    }
    else{ // Chave no modo Automático

      //Leitura dos Sensores (Baixo e Alto)
      short int alto,baixo;
      baixo = PIND & BAIXO;
      alto = PIND & ALTO;

      if(baixo == BAIXO) && (alto == ALTO)) // Ambos acionados
      !!!!!! PORTB = BOMBA; //Desliga a bomba
      if((baixo == 0) && (alto == 0))//Ambos desacionados
      PORTB = BOMBA; // Liga a bomba
    }
    
    }
  }
}
