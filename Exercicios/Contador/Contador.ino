#define MOTOR PD5
#define LIGA PD2
#define DESLIGA PD3

int contador=0;

ISR(INT0_vect){
  TCCR0A = (1 << WGM01);
  TCCR0B = (1<<CS01)|(1<<CS00);
  TIMSK0 |= (1<<OCR0A);
  OCR0A = 200;
}

ISR (TIMER0_COMPA_vect){
  if(contador>7){
    PORTD |= (1<<MOTOR);
    contador = 0;
  }
  contador++;
}

int main()
{
  DDRD |= (1 << MOTOR);
  PORTD |= (1<<LIGA)|(1<<DESLIGA);
  EICRA |= (0<<ISC00) | (0<<ISC01);
  EIMSK |= (1<<INT0);
  sei();
  while(1){
  }
}
