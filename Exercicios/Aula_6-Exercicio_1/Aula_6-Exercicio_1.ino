#define LED1 (1<<PD5)
#define LED2 (1<<PD4)
#define BOTAO (1<<PD2)

ISR(INT0_vect)
{
  PORTD |= LED2 ;//Liga LED2
  _delay_ms (1000);//Espera 1s
}
int main()
{
  DDRD |= LED1 | LED2;//Leds definidos como saídas
  PORTD |= BOTAO; //Habilita pull-up no BOTAO

  //PORTD = 0;//desliga as saídas do PORTD (os leds)
  EICRA |= (1<<ISC00) | (1<<ISC01);//gera transição de subida
  EIMSK |= (1<<INT0);//liga int0
  sei();//liga o serviço de interrupções

  while(1){
    PORTD |= LED1;
    _delay_ms(500);
    PORTD &=~ LED1;
    _delay_ms(500);
  }
}
