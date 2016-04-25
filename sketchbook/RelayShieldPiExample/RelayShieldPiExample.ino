//pins 7=R1,6=R2,5=R3,4=R4

// Arduino Pin to control R1
int RelayControl01 = 7;
// Arduino Pin to control R2
int RelayControl02 = 6;
// Arduino Pin to control R3
int RelayControl03 = 5;
// Arduino Pin to control R4
int RelayControl04 = 4;


// setup to run once when user press reset:
void setup()  { 
  // declare pin 7 to be an output:
  pinMode(RelayControl01, OUTPUT);
  // declare pin 6 to be an output:
  pinMode(RelayControl02, OUTPUT);
  // declare pin 5 to be an output:
  pinMode(RelayControl03, OUTPUT);
  // declare pin 4 to be an output:
  pinMode(RelayControl04, OUTPUT);
} 

// loop to cycle relay banks even and odd for 
// specified length of time, using delay(ms)
void loop()  { 
  // Turn R2,R4 on w/delay of 20sec
  
  // NO4 and COM4 Connected;
  digitalWrite(RelayControl04,HIGH);
  
  // NO2 and COM2 Connected;
  digitalWrite(RelayControl02,HIGH);
  delay(20000);
  
  //Turn R2,R4 off w/delay of 2sec
  
  // NO4 and COM4 Disconnected; 
  digitalWrite(RelayControl04,LOW);
  
  // NO2 and COM2 Disconnected;
  digitalWrite(RelayControl02,LOW);
  delay(2000);
  
  // Turn R1,R3 on w/delay of 5sec
  
  // NO3 and COM3 Connected;
  digitalWrite(RelayControl03,HIGH);
  
  // NO1 and COM1 Connected;
  digitalWrite(RelayControl01,HIGH);
  delay(5000);
  
  //Turn R1,R3 off w/delay of 0.1sec
  // NO3 and COM3 Disconnected;
  digitalWrite(RelayControl03,LOW);
  
  // NO1 and COM1 Disconnected;
  digitalWrite(RelayControl01,LOW);
  delay(100);
                         
}
