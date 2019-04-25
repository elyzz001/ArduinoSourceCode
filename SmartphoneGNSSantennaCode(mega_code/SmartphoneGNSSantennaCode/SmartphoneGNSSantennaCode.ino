#include <SPI.h>
#include <SD.h>
#include "Wire.h"
#include <Time.h>


// Begin DS1307 RTC
#define DS1307_I2C_ADDRESS 0x68
#define TIME_MSG_LEN 11
#define TIME_HEADER 'T'
// End DS1307 RTC

// Begin SD Card
File myFile;
// End SD Card
int output=7;


// set up variables using the SD utility library functions:


const int chipSelect = 53;
//char floatStr[9];
char dataStr[24];

char  start='b';
char  d_send='s';
char c_end='e';

byte c;   // for incoming serial data
const unsigned char UBX_HEADER[] = { 0xB5, 0x62 };
//int mpos=0;
//bool istrue=false;
int npos=0,nposold=0;
int numMeas=0;
int i=48,j=0, x=43,stp=0;
int svid=0;
int cno=0;

int az=0,ele;
// static unsigned char cksum[2];
//
long startTime,currentTime;




void setup() {
     //pinMode(LED_BUILTIN,OUTPUT);
    // Serial.begin(115200);
    pinMode(7,OUTPUT); 
    digitalWrite(7, LOW);
     
     Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps

   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
//SD CARD SETUP

  //Wire.begin();  
 // Serial.begin(9600);
 Serial1.begin(9600);
  Serial.print("Initializing SD card...");

  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
  // pinMode(53, OUTPUT);
  //pinMode(10, OUTPUT);
pinMode(SS, OUTPUT);
   /*
  if (!SD.begin(chipSelect)) 
  {
    Serial.print("Init failed!");
    return;
  }
  
  Serial.println(F("initialization done."));
 */
if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
    //testcard();
  //Serial.println("Creating datamega.txt...");
//  myFile = SD.open("datamega.txt", FILE_WRITE);

    // Check to see if the file exists: 
  if (SD.exists("datamega.txt")) {
    Serial.println("datamega.txt exists.");
    SD.remove("datamega.txt");
    Serial.println("datamega.txt has been removed.");
     myFile = SD.open("datamega.txt", FILE_WRITE); 
       Serial.println("new datamega.txt has been created");
    
  }
  else {
    Serial.println("datamega.txt doesn't exist."); 
    myFile = SD.open("datamega.txt", FILE_WRITE); 
       Serial.println("new datamega.txt has been created"); 
  }
}

void collectdata()
{
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.   
   Serial1.begin(9600);
  Serial.begin(9600);
Serial.print("we are inside collectdata");
myFile = SD.open("datamega.txt", FILE_WRITE);

  while(1)
  {
      //send data only when you receive data:
      
        while (Serial.available()) {
                  c =Serial.read();
               // Serial.print("Buffer: ");
             //   Serial.println(c,HEX );
               npos+=1;
                
               if (j<2 )
               {
                      
                    if ( c == UBX_HEADER[j] )
                    {
                        j++;
                        //npos=1;
                       
                        
                        //Serial.print("We got the header: ");
                       // Serial.println(c,HEX );

                        if(j==1)
                         {
                         //  Serial.print("inside j==1 bitch ");
                          nposold=npos+1;
                         }
                        
                      else
                       npos=1;
                        
                    }
                    else
                         if(npos == nposold && j==1)
                          {
                           j=0;
                           }
                    
                     
               }
              
       
               if(npos==17 && j==2)
               {
                 numMeas= c;
              Serial.print("we got the numMeas");
              Serial.println(numMeas,DEC);
                 stp=47+32*numMeas;

              
                
                 //j=0;
                 //mpos=0;
                 //npos=0;
                 //nposold=0;
               }

             if(npos==x && j==2)
               {
                Serial.print("SVID is: ");
                svid= (int)c;
                Serial.println(c,DEC);
                  x+=32;
               }
           
               if(npos==i && j==2)
                   {
                   Serial.print("CNO is: ");
                   cno= (int)c;
                   Serial.println(c,DEC);
                    i+=32;
                    //WRITE DATA TO THE SD CARD
                    //call getAngle()
                    //add angles to the file
                   long currentint=millis()-startTime;
                   currentTime= (millis()-startTime)/1000;
                   Serial.print("CurrentTime in S is");
                                      Serial.println(currentTime);
 Serial.print("CurrentTime in MS is");
                                      Serial.println(currentint);
                   
                    if(currentTime%34>=17)
                    
                    {
                      az=(360-(360/17))-(currentTime%17)*(360/17);
                      
                    }
                    else
                    {
                    az=(currentTime%17)*(360/17);
                    }

                    ele=(currentTime/17)*12;                  
                    
                     sprintf(dataStr, "%02d,%02d,%03d,%03d",svid,cno,az,ele);
                     myFile.println(dataStr);
                    //Serial.println(i,DEC);
                    
                   }
                if(npos==stp)
                   {
                  
                  npos=0;
                  numMeas=0;
                  j=0;
                  i=48;
                  x=43;
                 // Serial.print("I am reseting this bitch");
                    }
               
                 }
                 if(Serial1.available())
                 
                { 
                  char d =Serial1.read();
                  if(d==c_end)
                    break;
                }
  }
                 myFile.close();
}
void loop()
{
  //Serial1.begin(9600);
    //Serial.begin(9600);

  if (Serial1.available() > 0) {
    char c=Serial1.read();
    Serial.print(c);
    if(c==start)
    {
           startTime=millis();
           digitalWrite(7, HIGH);
           delay(50);
           collectdata();
           digitalWrite(7, LOW);

    }
    if(c==d_send)
        senddata();
                // read the incoming byte:
             //  incomingByte =Serial1.readString();

                // say what you got:
                //Serial.print("I received: ");
                //Serial.println(incomingByte);
        }
// analogReference(INTERNAL);

}

void senddata()
{

File myFile;
Serial.print("inside send data fct");

 if (SD.exists("datamega.txt")) {
    Serial.println("datamega.txt exists.");
 }
 else
 {
  Serial.println("datamega.txt does not exists.");
  return;
 }
  

     myFile = SD.open("datamega.txt",FILE_READ);

   if (myFile) {
    while (myFile.available()) {
     // Serial.print("data available");
      Serial1.write(myFile.read());
    }
    myFile.close();
  }
}

void getAngle()
{

  
}


  




              


 
