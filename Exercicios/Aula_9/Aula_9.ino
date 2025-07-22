#define LED PD7
#define LED1 PD5

int contador = 0;

ISR (TIMER0_OVF_vect) {
  if (contador > 61) {
    PORTD ^= (1 << LED); // inverte o estado do LED
    contador = 0;
  }
  contador ++;
}

int main()
{
  DDRD |= (1 << LED);
  DDRD |= (1 << LED1);
  TCCR0A = 0;
  TCCR0B = (1 << CS02) | (1 << CS00); 
  TIMSK0|= (1 << TOIE0); 
  sei(); 
  PORTD &= ~(1 << LED1);
  
  while (1) {
    PORTD &= ~(1 << LED1);
    _delay_ms(1000);
    PORTD |= (1 << LED1);
    _delay_ms(1000);
  }
}
