
#include <16F877.h>
#fuses XT,PUT,NOWDT,NOPROTECT,NOBROWNOUT // Sets a typical configuration fuses 
/*
// XT : Clock Type Select (case)
// PUT: Power Up Timer Enable
// NOWDT : NO_Watcdog Timer Enable
// NOPROTECT : NO_Program Code Protect
// NOBROWNOUT : NO_Brownout Reset Enable
*/

#use delay (CLOCK = 20M) // check the carte
#use RS232 (baud = 9600, XMIT = PIN_C6, RCV = PIN_C7)

#define Time 500
#define echo_pin PIN_B7    //
#define trig_pin PIN_B6    //
#define myLED    PIN_B5    //
int16 i, distance;

int1 wait_sensor(){
  i = 0;
  set_timer1(0);                           // Reset Timer1 Sets at zero
  while(!input(echo_pin) && (i < 1000))    // echo_pin still on LOW Level 
    i = get_timer1();                      // Read Timer1 and store its value in i
  if(i > 990)
    return 0;                                 // Time out
  else
    return 1;
}

int16 get_distance(){
  i = 0;
  set_timer1(0);
  while(input(PIN_B0) && (i < 25000))    // i -- > 416cm (stop because of hardware of the sensor)
    i = get_timer1();
  return i;
}

void main(){
  
  output_b(0);
  set_tris_b(1);                   // Configure RB0 pin as input
  delay_ms(1000);
  //setup_timer_2(); // Configure Timer 1 to increment by 1 every 1 us
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);


  setup_uart(9600);
  while(TRUE){
  
    // Send 10us pulse to HC-SR04 Trigger pin
    output_high(PIN_B1);
    delay_us(10);
    output_low(PIN_B1);
    // Read pulse comes from HC-SR04 Echo pin
    if(wait_sensor()){
      distance = get_distance();
      distance = i/63;   // Calculate the distance (see the Datasheet for more detaile)    
      printf("Distance : %lu ", distance);
      printf("cm");
      putc(13);
      if (distance < 100) 
         output_high(myLED);
      else
         output_low(myLED);       
      delay_ms(Time);
      }
    }
   
  delay_ms(100);
}
// End of code
