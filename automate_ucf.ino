#include <SoftwareSerial.h>  //to include the serial library
#define DEBUG true
SoftwareSerial esp8266(2,3); // Pin assignment
void setup() 
{
  Serial.begin(9600);   // to set the baud rate
  esp8266.begin(9600);  
  pinMode(11,OUTPUT);   
  digitalWrite(11,LOW); 
  pinMode(12,OUTPUT);   
  digitalWrite(12,LOW); 
  sendData("AT+RST\r\n",2000,DEBUG);            //to resest the module to default
  sendData("AT+CWMODE=2\r\n",1000,DEBUG);       // mode as access point
  sendData("AT+CIFSR\r\n",1000,DEBUG);          // to show ip address
  sendData("AT+CIPMUX=1\r\n",1000,DEBUG);       
  sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG); // set server port to 80
}

void loop() 
{
  if(esp8266.available()) // check if the module is available
  { 
    if(esp8266.find("+IPD,"))
    { 
      delay(1000);       
      int connectionId = esp8266.read()-48;   // Subtract 48 from the character to get the number.
      esp8266.find("pin=");                  // finding pin
      int pinNumber = (esp8266.read()-48)*10; // getting first pin
      pinNumber += (esp8266.read()-48);       // getting the 2nd pin

      
      digitalWrite(pinNumber, !digitalRead(pinNumber));
     
     
      String closeCommand = "AT+CIPCLOSE="; // closes the connection
      closeCommand+=connectionId; 
      closeCommand+="\r\n";
      sendData(closeCommand,1000,DEBUG);     // Sending to ESP 8266 to close command
    } 
  } 
}


String sendData(String command, const int timeout, boolean debug) // function 
{
  String response = "";
   
  esp8266.print(command);           // Send the command to the ESP8266
  long int time = millis();
  while( (time+timeout) > millis()) 
  {
    while(esp8266.available())      // Checking ESP8266 status
    {
      char x = esp8266.read();      // read next character
      response+=x;                  // to store the response
    }
  }
  if(debug)
  { 
    Serial.print(response);         // Prinsts the response
  }
  return response;
} 
