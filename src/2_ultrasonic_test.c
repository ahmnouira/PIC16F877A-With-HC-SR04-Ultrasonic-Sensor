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
#define echo_Front         PIN_B0
#define echo_Rear          PIN_B4
#define trig_Front           PIN_B1
#define trig_Rear            PIN_B3
#define myLED_F            PIN_B2
#define myLED_R            PIN_B5
#define Distance_Front              41
#define Distance_Rear               41
int16 i1,i2,dist1,dist2;

int1 wait_sensor_front(){
  i1 = 0;
  set_timer1(0);                           // Reset Timer1 Sets at zero
  while(!input(echo_Front) && (i1 < 1000))    // echo_pin still on LOW Level 
    i1 = get_timer1();                      // Read Timer1 and store its value in i
  if(i1 > 990)
    return 0;                              // Time out
  else
    return 1;
}
int16 get_distance_front(){
  i1 = 0;
  set_timer1(0);
  while(input(echo_Front) && (i1 < 25000))    // i -- > 416cm (stop because of hardware of the sensor)
    i1 = get_timer1();
  return i1;
}

int1 wait_sensor_rear(){
  i2 = 0;
  set_timer1(0);                           // Reset Timer1 Sets at zero
  while(!input(echo_Rear) && (i2 < 1000))    // echo_pin still on LOW Level 
    i2 = get_timer1();                      // Read Timer1 and store its value in i
  if(i2 > 990)
    return 0;                              // Time out
  else
    return 1;
}
int16 get_distance_rear(){
  i2 = 0;
  set_timer1(0);
  while(input(echo_Rear) && (i2 < 25000))    // i -- > 416cm (stop because of hardware of the sensor)
    i2 = get_timer1();
  return i2;
}
int16 pingSonarFront() {
   // Send 10us pulse to HC-SR04 Trigger pin
    output_high(trig_Front);
    delay_us(10);
    output_low(trig_Front);
    // Read pulse comes from HC-SR04 Echo pin
    if(wait_sensor_front()){
      dist1 = get_distance_front();
      dist1 = i1/63;   
      return dist1;
    }
}

int16 pingSonarRear() {
   // Send 10us pulse to HC-SR04 Trigger pin
    output_high(trig_Rear);
    delay_us(10);
    output_low(trig_Rear);
    // Read pulse comes from HC-SR04 Echo pin
    if(wait_sensor_rear()){
      dist2 = get_distance_rear();
      dist2 = i2/63;   
      return dist2;
    }
}
void main() {
  output_b(0);
  set_tris_b(0x11);                   
  setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);
  setup_uart(9600);
  delay_ms(1000);
  while(TRUE){
      printf("DistanceRear : %lu ", pingSonarRear());
      printf("cm");
      putc(13);
      printf("DistanceFront : %lu ", pingSonarFront());
      printf("cm");
      putc(13);
      if (pingSonarRear() < Distance_Rear) 
         output_high(myLED_R);
      else
         output_low(myLED_R);
     if (pingSonarFront() < Distance_Front) 
         output_high(myLED_F);
      else
         output_low(myLED_F);
       
      delay_ms(500);
    
}
}
