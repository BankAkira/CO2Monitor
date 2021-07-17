/*
 * Created Date: Wednesday, October 17th 2018, 2:36:32 am
 * Author: akira
 * 
 * Copyright (c) 2018 
 */

//------------ UART K30 FR -------------//
byte readCO2[] = {0xFE, 0X44, 0X00, 0X08, 0X02, 0X9F, 0X25};  //Command packet to read Co2
byte response_1[] = {0,0,0,0,0,0,0};  //create an array to store the response

//multiplier for value. default is 1. set to 3 for K-30 3% and 10 for K-33 ICB
int valMultiplier = 1;


//-----Co2 UART function --------
void sendRequest_1(byte packet[]) {
  while(!Serial1.available())  //keep sending request until we start to get a response
  {
    Serial1.write(readCO2,7);
    delay(50);
  }
  
  int timeout=0;  //set a timeoute counter
  while(Serial1.available() < 7 ) //Wait to get a 7 byte response
  {
    timeout++;  
    if(timeout > 10)    //if it takes to long there was probably an error
      {
        while(Serial1.available())  //flush whatever we have
          Serial1.read();
          
          break;                        //exit and try again
      }
      delay(50);
  }
  
  for (int i=0; i < 7; i++)
  {
    response_1[i] = Serial1.read();
  }  
}


unsigned long getValue(byte packet[]) {
    int high = packet[3];         //high byte for value is 4th byte in packet in the packet
    int low = packet[4];          //low byte for value is 5th byte in the packet
    Serial.print("High : ");
    Serial.print(high);
    Serial.print(" Low : ");
    Serial.println(low);
    unsigned long val = high*256 + low; //Combine high byte and low byte with this formula to get value
    return val* valMultiplier;
}


void setup()  {
  // put your setup code here, to run once:
  Serial.begin(9600);     //Opens the main serial port to communicate with the computer
  Serial1.begin(9600);    //Opens the Serial1 port with a baud of 9600
}

void loop()  {
    sendRequest_1(readCO2);   //send request to Co2_1 sensor 
    unsigned long valCO2_1 = getValue(response_1);    //convert response from Co2_1 to CO2 value in ppm
    Serial.print("Co2 ppm = ");
    Serial.println(valCO2_1);
    delay(5000);
}
