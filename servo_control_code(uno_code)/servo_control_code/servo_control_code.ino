#include<Servo.h>
Servo servo1;
int pos1 = 0;
Servo servo2;
int pos2 = 10;
int input=12;

void setup() {
  // put your setup code here, to run once:
servo1.attach(9,164,2500);
servo2.attach(10);
servo1.write(20);
delay(50);
pinMode(12,INPUT);
digitalWrite(12, LOW);

}

void loop() 
{
  // put your main code here, to run repeatedly:
 
  // BLUETOOTH SECTION
  //if(Serial.available() > 0)
  //{
   // char data;
   // data = Serial.read();
   // if(data == 1)
 // { 

   // END BLUETOOTH SECTION
  // pos2 = 10;
  //servo1.write(180);
   //servo2.write(pos2);
//  delay(10);
 // servo1.write(90);

  // for(pos2 = 10; pos2 <= 170; pos2 += 5)
   //{
    if(digitalRead(12) == HIGH)

     {
       funct();
     }
   /*  while(1)
     {
     
     }*/
     //servo1.write();
     //delay(1000);

   
   /* pos2+=10;
    servo2.write(pos2);
    delay(1000);
    for(pos1 = 180; pos1 >=0; pos1 -= 5)
    {
      servo1.write(pos1);
      delay(1000);
    }
    pos2+=5;
    servo2.write(pos2);
    //delay(1000);
    */

  
   
//{


   //}

   
}

void funct()
{
//servo1.write(30);
//delay(1000);
for(pos1 = 30; pos1<=170; pos1 +=10)
  {
    if(digitalRead(12) == LOW)
    {
      break;
    }
     for(pos2 ; pos2 <= 170; pos2 +=10)
     {
      if(digitalRead(12) == LOW)
       {
      break;
       }
     
      servo2.write(pos2);
      delay(1000);
      
     }
    
     servo1.write(pos1);
     delay(1000);

    for(pos2 = 160; pos2>=10; pos2 -=10)
      {
        if(digitalRead(12) == LOW)
        {
         break;
       }
        servo2.write(pos2);
        delay(1000);
      }
    pos1+=10;
    if (pos1<170){
    servo1.write(pos1);
    delay(1000);
    }
    
    pos2 +=20;
 }
    

   
    }
