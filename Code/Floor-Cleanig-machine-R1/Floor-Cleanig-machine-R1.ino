// This is the code for our floor cleaning machine. Which build for Desig protype project. This code fully owned by Group R1, Sasindu malhara (2022/E/126)

// Define pins
#define L1 2
#define L2 4
#define LENB 3

#define R1 5
#define R2 7
#define RENB 6

#define Roller 8
#define MOP 9
#define Pump 10
#define Dry 11

#define Usf_Echo A0
#define Usf_Trig A1

#define Usl_Echo A2
#define Usl_Trig A3

#define Usr_Echo A4
#define Usr_Trig A5

int motor_speed = 255;
int autoModeOn = 0;
pumpstate =0;
char incomingByte;

unsigned long prev_t = 0;
unsigned long curr_t = 0;

int readings[100];
byte mapsize = 100;
int i = 1;
byte unit_TH = 20000;
byte unit_TV = 15000;

byte l=0, b=0, r=0;

byte base_size = 0;

void setup() {
  pinMode(Pump, OUTPUT);
  pinMode(MOP, OUTPUT);
  pinMode(Roller, OUTPUT);
  pinMode(Dry, OUTPUT);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(LENB, OUTPUT);
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(RENB, OUTPUT);
  initializeUS();
  Serial.begin(9600); 
}


void loop()
{

  switch(autoModeOn)
  {
    case 0:
    while(autoModeOn == 0)
    {
      manualMode();     
    }
    break;
    
    case 1 :
    while(autoModeOn == 1)
    {
      automaticMode();     
    }
    break;
    
    delay(500);
  }

}


void automaticMode() {

  checkautomode();

  Serial.println("I is: ");
  Serial.print(i);
  l=0;
  b=0;
  r=0;
  if(readUsf() > 15) {
      if(i%2 != 0){
        prev_t = millis();
        while(readUsf() > 15){
          Serial.println("Inside 1");
          incomingByte = Serial.read();   //read incoming data
          if (incomingByte == 'X'){
            autoModeOn = 0;
            incomingByte='*';
            break;
          }
          move_f();
          
        }
      } else {
        if (i % 4  != 0){
          turn_r();
          move_oneblock();
          if(look_l() > 20){
            turn_l();
            prev_t = millis();
            while(readUsf() > 10){
              Serial.println("Inside 2");
              move_f();
              incomingByte = Serial.read();   //read incoming data
              if (incomingByte == 'X'){
                autoModeOn = 0;
                incomingByte='*';
                break;
              }
          break;
            }
            curr_t = millis();
//            addReading(i);
            l = (curr_t - prev_t)/unit_TH;
            turn_r();
            Serial.println("Inside 3");
            move_f();
            incomingByte = Serial.read();   //read incoming data
            if (incomingByte == 'X'){
              autoModeOn = 0;
              incomingByte='*';
            }
            delay(curr_t - prev_t);
          }
          turn_r();
          i++;
        } else {
          turn_l();
          move_oneblock();
          if(look_r()> 20){
            turn_r();
            prev_t = millis();
            while(readUsf() > 10){
              Serial.println("Inside 4");
              move_f();
              incomingByte = Serial.read();   //read incoming data
              if (incomingByte == 'X'){
                autoModeOn = 0;
                incomingByte='*';
                break;
              }
            }
            curr_t = millis();
//            addReading(i);
            r = (curr_t - prev_t)/unit_TH;
            turn_l();
            Serial.println("Inside 5");
            move_f();
            incomingByte = Serial.read();   //read incoming data
            if (incomingByte == 'X'){
              autoModeOn = 0;
              incomingByte='*';
            }
            delay(curr_t - prev_t);
          }
          turn_l();
          i++;
        }
      }
    } else {
      move_s();
      curr_t = millis();
      b = (curr_t - prev_t)/unit_TH;
//      addReading(i);
      i++;
      if(look_r() > 20){
        turn_r();
        incomingByte = Serial.read();   //read incoming data
        if (incomingByte == 'X'){
          autoModeOn = 0;
          incomingByte='*';
        }
      }
    }
}


void manualMode()
{
  if (Serial.available() > 0)   //check if any data is available
  {
    incomingByte = Serial.read();   //read incoming data
  }

  switch(incomingByte)    //based on received character execute respective commands
  {
    case 'A':
    autoModeOn = 1;
    incomingByte='*';
    break;

    case 'F':
    move_f();
    incomingByte='*';
    break;

    case 'f':
    move_s();
    incomingByte='*';
    break;
    
    case 'B':
    move_b();
    incomingByte='*';
    break;
    
    case 'b':
    move_s();
    incomingByte='*';
    break;

    case 'L':
    turn_l();
    incomingByte='*';
    break;

    case 'l':
    move_s();
    incomingByte='*';
    break;
    
    case 'R':
    turn_r();
    incomingByte='*';
    break;

    case 'r':
    move_s();
    incomingByte='*';
    break;
    
    case 'S':
    move_s();
    incomingByte='*';
    break;
    
    case 'P':
    pumpON();
    incomingByte='*';
    break;
    
    case 'p':
    pumpOFF(); 
    incomingByte='*';
    break;
    
    case 'M':
    digitalWrite(MOP, HIGH);
    incomingByte='*';
    break;
    
    case 'm':
    digitalWrite(MOP, LOW); 
    incomingByte='*';
    break;

    case 'K':
    digitalWrite(Roller, HIGH);
    incomingByte='*';
    break;
    
    case 'k':
    digitalWrite(Roller, LOW); 
    incomingByte='*';
    break;

    case 'Q':
    digitalWrite(Dry, HIGH);
    incomingByte='*';
    break;
    
    case 'q':
    digitalWrite(Dry, LOW); 
    incomingByte='*';
    break;

    case '1':
    motor_speed = 155;
    incomingByte='*';
    break;
    
    case '2':
    motor_speed = 205;
    incomingByte='*';
    break;
    
    case '3':
    motor_speed = 255;
    incomingByte='*';
    break;
    
    delay(5000);
  }
}




void move_s() {
  Serial.println("Stop");
  digitalWrite(L1, LOW); 
  digitalWrite(L2, LOW);
  digitalWrite(R1, LOW);
  digitalWrite(R2, LOW);
} 
  
void move_f() {
  Serial.println("Forward");
  rollerON();
  pumpON();
  mopON();
  dryON();
  digitalWrite(L1, LOW); 
  analogWrite(L2, motor_speed);
  analogWrite(R1, motor_speed);
  digitalWrite(R2, LOW);   
}

void move_b() {
  Serial.println("Backward");
  digitalWrite(L2, LOW); 
  analogWrite(L1, motor_speed);
  analogWrite(R2, motor_speed);
  digitalWrite(R1, LOW);   
}

void move_oneblock() {
    rollerON();
    pumpON();
    mopON();
    dryON();
    Serial.println("OneBlock");
    digitalWrite(L1, LOW); 
    analogWrite(L2, motor_speed);
    analogWrite(R1, motor_speed);
    digitalWrite(R2, LOW);      
    delay(unit_TV);
    move_s();
}

void turn_r() {
  rollerOFF();
  pumpOFF();
  mopOFF();
  dryOFF();
  Serial.println("Right");
  digitalWrite(L1, LOW); 
  analogWrite(L2, motor_speed);
  digitalWrite(R1, LOW);
  analogWrite(R2, motor_speed);   
  delay(2000);
} 

void turn_l() {
  rollerOFF();
  pumpOFF();
  mopOFF();
  dryOFF();
  Serial.println("Left");
  analogWrite(L1, motor_speed); 
  digitalWrite(L2, LOW);
  analogWrite(R1, motor_speed);
  digitalWrite(R2, LOW); 
  delay(2000);
}

void initializeUS(){
  pinMode(Usf_Trig, OUTPUT); 
  pinMode(Usf_Echo, INPUT);

  pinMode(Usl_Trig, OUTPUT); 
  pinMode(Usl_Echo, INPUT);

  pinMode(Usr_Trig, OUTPUT); 
  pinMode(Usr_Echo, INPUT);

  Serial.begin(9600);
  look_f();
}

int readUsf() {
  digitalWrite(Usf_Trig, LOW);
  delayMicroseconds(200);
  digitalWrite(Usf_Trig, HIGH);
  delayMicroseconds(1000);
  digitalWrite(Usf_Trig, LOW);
//  distance= duration*0.034/2;
  int distance = microsecondsToCentimeters(pulseIn(Usf_Echo, HIGH));
  if(distance == 0)Serial.println(distance);
  return distance;
}

int readUsr() {
  digitalWrite(Usr_Trig, LOW);
  delayMicroseconds(200);
  digitalWrite(Usr_Trig, HIGH);
  delayMicroseconds(1000);
  digitalWrite(Usr_Trig, LOW);
//  distance= duration*0.034/2;
  int distance = microsecondsToCentimeters(pulseIn(Usr_Echo, HIGH));
  if(distance == 0)Serial.println(distance);
  return distance;
}

int readUsl() {
  digitalWrite(Usl_Trig, LOW);
  delayMicroseconds(200);
  digitalWrite(Usl_Trig, HIGH);
  delayMicroseconds(1000);
  digitalWrite(Usl_Trig, LOW);
//  distance= duration*0.034/2;
  int distance = microsecondsToCentimeters(pulseIn(Usl_Echo, HIGH));
  if(distance == 0)Serial.println(distance);
  return distance;
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}

void look_f(){
 
}

int look_r(){

  int d = readUsr();
  delay(1000);
  return d;
}

int look_l(){
  int d = readUsl();
  delay(1000);
  return d;
}

void re_Readings() {
  byte i = sizeof(readings);
  int nreadings[2*i];
  while ( i-- ) *( nreadings + i ) = *( readings + i );
  int *readings = new int[2*i];
  readings = nreadings;
  mapsize = 2*mapsize;
}

void addReading(int i){
  int index = sizeof(readings)/sizeof(readings[0]) - 1;
  if(mapsize == index){
    re_Readings();
  }
  if(i % 2 == 0) {
    if ((i & 3) == 0){
      readings[index+1] = l;
    } else {
      readings[index+1] = r;
    }
  }else {
    if(i == 1){
      readings[0] = 0;
      readings[2] = 0;
      readings[1] = (curr_t - prev_t)/unit_TH;
      base_size = readings[1];
    } else {
      readings[index+1] = b;
    }
  }
}

void writeMap(){
  FILE *f = fopen("map.data", "wb");
  fwrite(readings, sizeof(int), sizeof(readings), f);
  fclose(f);
}

void checkautomode(){
  
  incomingByte = Serial.read();   //read incoming data

  if (incomingByte == 'X')
  {
    autoModeOn = 0;
    incomingByte='*';
  } else if (incomingByte == 'A'){
    autoModeOn = 1;
    incomingByte='*';
  }
}

void pumpON(){
  pumpstate = 1;
  while (pumpstate==1){
    digitalWrite(Pump, HIGH);
    delay(1000);
    digitalWrite(Pump, LOW);
    delay(3000);
  }
}

void pumpOFF(){
  pumpstate = 0;
}


void rollerON(){
  digitalWrite(Roller, HIGH);
}

void rollerOFF(){
  digitalWrite(Roller, LOW);
}


void mopON(){
  digitalWrite(MOP, HIGH);
}

void mopOFF(){
  digitalWrite(MOP, LOW);
}


void dryON(){
  digitalWrite(Dry, HIGH);
}

void dryOFF(){
  digitalWrite(Dry, LOW);
}

void readMap(){
//  #define fmap "C:\\tempExtract\\fmap.data"
//  FILE *fp;
//  int newMap[100];
//  fopen(fmap, "rb");
//  fread(newMap, sizeof(char), 1,fp);
//  fclose(fp);
}
