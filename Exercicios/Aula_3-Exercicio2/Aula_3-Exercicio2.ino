int main()
{
  DDRD = 0b00000110;

  while(1){
    PORTD = 0b00000000;
    _delay_ms(500);
    PORTD = 0b00000010;
    _delay_ms(500);
    PORTD = 0b00000100;
    _delay_ms(500);
    PORTD = 0b00000110;
    _delay_ms(500);
  }
}
