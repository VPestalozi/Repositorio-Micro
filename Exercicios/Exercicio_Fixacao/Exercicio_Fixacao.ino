#define LIGA (1<<PC0)
#define MOTORD (1<<PD7)
#define MOTORE (1<<PD6)
#define S1 (1<<PB0)
#define S2 (1<<PB1)
#define DESLIGA (1<<PC1)

int cont=0;
int cont2=0;

ISR (TIMER0_COMPA_vect){
  if(cont==6875){
    PORTD &=~(MOTORD | MOTORE);
    cont = 0;
  }
  cont++;
}

ISR (TIMER0_COMPB_vect){
  if(cont2==9375){
    PORTD &=~(MOTORD | MOTORE);
    cont = 0;
  }
  cont++;
}

ISR (PCINT0_vect){
  
}

int main(){
  
  DDRD |= MOTORD | MOTORE; // Coloca o motor como saida
  PORTD &=~(MOTORD | MOTORE); // Inicia o motor desiligado
  PORTC |= LIGA | DESLIGA; // Pull-up no botão liga e desilga

  // Definindo o Timer
  TCCR0A = (1<<WGM01);
  TCCR0B=(1<<CS01)|(1<<CS00);
  TIMSK0=(1<<OCIE0A);
  OCR0A = 200; // Timer de 5,5 segundos
  OCR0B = 
  
  
  PCICR |= (1<<PCIE0); // Ativa a possibilidade de interrupção nos pinos de PB0 a PB7
  PCMSK0 |= (1<<PCINT0) | (1<<PCINT1); // Ativando interrupção no pino PB0 e PB1
  sei();
  for(;;){
    if(!(PINC & LIGA)){
      PORTD |= MOTOR;
      
    }
  }
  
}
