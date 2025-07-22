int main()
{
  DDRD = 0b00001110;
  DDRB = 0b00001110;

  while(1){
    PORTD = 0b00001110;
    _delay_ms(1000);
    PORTD = 0b00000000;
    _delay_ms(1000);
  }
}
