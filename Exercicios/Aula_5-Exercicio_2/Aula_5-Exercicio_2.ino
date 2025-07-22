#define BOTAOA (1<<PD1)
#define BOTAOB (1<<PD2)
#define BOTAOC (1<<PD3)
#define MOTOR1 (1<<PB0)
#define MOTOR2 (1<<PB1)
#define MOTOR3 (1<<PB2)

int main(){
  DDRD = 0;
  DDRB = MOTOR1 | MOTOR2 | MOTOR3;

  // Habilitando Pull-up nos botões
  PORTD = BOTAOA | BOTAOB | BOTAOC;

  // Desliga as saidas para iniciarem desligadas
  PORTB &= ~(MOTOR1 | MOTOR2 | MOTOR3);

  for(;;)
  {
    short int hp1,hp2,hp3,total;
    hp1 = 30;
    hp2 = 50;
    hp3 = 70;
    total = 0;
    
    if(!(PIND & BOTAOA)){
      PORTB |= MOTOR1;
      total +=hp1;
    }
    if(!(PIND & BOTAOB)){
      PORTB |= MOTOR2;
      total +=hp2;
    }
    if(!(PIND & BOTAOC)){
      PORTB |= MOTOR3;
      total +=hp3;
    }
    if(total>90){
      PORTB &= ~MOTOR1;
      total -=hp1;
      if(total>90){
        PORTB &= ~MOTOR2;
        total -=hp2;
      }
    }
  }
}
