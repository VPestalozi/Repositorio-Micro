//Definindo Constates
#define LIGA (1<<PD0)
#define DESLIGA (1<<PD1)
#define SENSOR (1<<PD2)
#define ESTEIRA (1<<PB0)
#define ALARME (1<<PB1)

int main(){
  DDRB |= ESTEIRA|ALARME;
  PORTB &=~(ESTEIRA|ALARME);

  // Realizando Pull-Up nos botões
  PORTD |= LIGA|DESLIGA;
  
  for(;;){
    if(!(PIND & LIGA) && !(PIND & SENSOR)){
      PORTB |= ESTEIRA;
      PORTB &=~(ALARME);
    }
    if(!(PIND & DESLIGA)){
      PORTB &=~(ESTEIRA);
    }
    if(PIND & SENSOR){
      PORTB &=~(ESTEIRA);
      PORTB |= ALARME;
    }
  }
}
