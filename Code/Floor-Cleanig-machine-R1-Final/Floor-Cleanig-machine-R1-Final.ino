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
#define Pump 12
#define Dry 11

#define Usfl_Echo A0
#define Usfl_Trig A1

#define Usfr_Echo 10
#define Usfr_Trig 13

#define Usl_Echo A2
#define Usl_Trig A3

#define Usr_Echo A4
#define Usr_Trig A5

int motor_speed = 255;
int autoModeOn = 0;
char incomingByte;
bool pumpState = false;
const long pumpOnTime = 1000; 
const long pumpOffTime = 2000; 
int obstacledistance = 30;
bool FpumpState = false;


float pred[3];

void setup() {
  pinMode(Pump, OUTPUT);
  pinMode(MOP, OUTPUT);
  pinMode(Roller, OUTPUT);
  pinMode(Dry, OUTPUT);

  pinMode(Usfl_Trig, OUTPUT); 
  pinMode(Usfl_Echo, INPUT);

  pinMode(Usfr_Trig, OUTPUT); 
  pinMode(Usfr_Echo, INPUT);

  pinMode(Usl_Trig, OUTPUT); 
  pinMode(Usl_Echo, INPUT);

  pinMode(Usr_Trig, OUTPUT); 
  pinMode(Usr_Echo, INPUT);

  Serial.begin(9600); 
}


void loop()
{

  switch(autoModeOn)
  {
    case 0:
    manualMode();   
    break;

    case 1 :
    automaticMode();     
    break;
  }
}


void automaticMode() {

  motor_speed = 255;

  if (Serial.available() > 0)   //check if any data is available
  {
    incomingByte = Serial.read();   //read incoming data
    Serial.println(incomingByte);

    if (incomingByte == 'X'){
      Serial.println("Stop");
      digitalWrite(L1, LOW); 
      digitalWrite(L2, LOW);
      digitalWrite(R1, LOW);
      digitalWrite(R2, LOW);
      autoModeOn = 0;

    }
  }

  pred[0]= readUsf();
  pred[1]= readUsl();
  pred[2]= readUsr();
   for (int i = 0; i < 3; i++) {
        Serial.println(pred[i]);
    }
  int prediction = predict(pred);
  Serial.println(prediction);
  switch(prediction){

    case 0:
    turn_l();
    break;

    case 1:
    turn_r();
    break;

    case 2:
    move_b();
    break;

    case 3:
    move_f();
    break;
  }
  
}


void manualMode()
{ 
  if (pumpState == true && FpumpState == true){
    pump();
    }
    

  if (Serial.available() > 0)   //check if any data is available
  {
    incomingByte = Serial.read();   //read incoming data
    Serial.println(incomingByte);
  }

  switch(incomingByte)    //based on received character execute respective commands
  {
    case 'A':
    autoModeOn = 1;
    incomingByte='*';
    break;

    case 'F':
    Serial.println("Forward");
    digitalWrite(L1, LOW); 
    analogWrite(L2, motor_speed);
    analogWrite(R1, motor_speed);
    digitalWrite(R2, LOW);
    FpumpState = true;
    break;

    case 'f':
    Serial.println("Stop");
    digitalWrite(L1, LOW); 
    digitalWrite(L2, LOW);
    digitalWrite(R1, LOW);
    digitalWrite(R2, LOW);
    pumpState = false;
    break;
    
    case 'B':
    Serial.println("Backward");
    digitalWrite(L2, LOW); 
    analogWrite(L1, motor_speed);
    analogWrite(R2, motor_speed);
    digitalWrite(R1, LOW);
    pumpState = false;
    break;
    
    case 'b':                                                                                                          
    Serial.println("Stop");
    digitalWrite(L1, LOW); 
    digitalWrite(L2, LOW);
    digitalWrite(R1, LOW);
    digitalWrite(R2, LOW);
    pumpState = false;
    break;
    
    case 'L':
    Serial.println("Left");
    analogWrite(L1, motor_speed); 
    digitalWrite(L2, LOW);
    analogWrite(R1, motor_speed);
    digitalWrite(R2, LOW);
    pumpState = false;
    break;
    
    case 'l':
    Serial.println("Stop");
    digitalWrite(L1, LOW); 
    digitalWrite(L2, LOW);
    digitalWrite(R1, LOW);
    digitalWrite(R2, LOW);
    pumpState = false;
    break;

    case 'R':
    Serial.println("Right");
    digitalWrite(L1, LOW); 
    analogWrite(L2, motor_speed);
    digitalWrite(R1, LOW);
    analogWrite(R2, motor_speed);
    pumpState = false;
    break;
    
    case 'r':
    Serial.println("Stop");
    digitalWrite(L1, LOW); 
    digitalWrite(L2, LOW);
    digitalWrite(R1, LOW);
    digitalWrite(R2, LOW);
    pumpState = false;
    break;

    case 'S':
    Serial.println("Stop");
    digitalWrite(L1, LOW); 
    digitalWrite(L2, LOW);
    digitalWrite(R1, LOW);
    digitalWrite(R2, LOW);
    pumpState = false;
    break;

    case 'N':
    Serial.println("Pump ON");
    pumpState = true;
    break;
    
    case 'n':
    Serial.println("Pump OFF");
    pumpState = false;
    digitalWrite(Pump, LOW);
    break;

    case 'M':
    digitalWrite(MOP, HIGH);
    break;
    
    case 'm':
    digitalWrite(MOP, LOW); 
    break;

    case 'K':
    digitalWrite(Roller, HIGH);
    break;
    
    case 'k':
    digitalWrite(Roller, LOW); 
    break;

    case 'Q':
    digitalWrite(Dry, HIGH);
    break;
    
    case 'q':
    digitalWrite(Dry, LOW); 
    break;

    case '1':
    motor_speed = 155;
    break;
    
    case '2':
    motor_speed = 205;
    break;
    
    case '3':
    motor_speed = 255;
    break;
    
    delay(500);
  }
}



// Auto mode functions
  
void move_f() {
  Serial.println("Forward");
  digitalWrite(L1, LOW); 
  analogWrite(L2, motor_speed);
  analogWrite(R1, motor_speed);
  digitalWrite(R2, LOW);
  digitalWrite(Roller, HIGH);
  digitalWrite(MOP, HIGH);
  digitalWrite(Dry, HIGH);
  pump();                                                                                                                                                                            
}

void move_b() {
  Serial.println("Backward");
  digitalWrite(Roller, LOW);
  digitalWrite(MOP, LOW);
  digitalWrite(Dry, LOW);
  while (readUsl() == 1 && readUsr() ==1 ){
  digitalWrite(L2, LOW); 
  analogWrite(L1, motor_speed);
  analogWrite(R2, motor_speed);
  digitalWrite(R1, LOW);

  if (Serial.available() > 0)   //check if any data is available
  {
    incomingByte = Serial.read();   //read incoming data
    Serial.println(incomingByte);

    if (incomingByte == 'X'){
      autoModeOn = 0;
      break;
    }
  }

  }
  if (readUsl == 0){
    turn_l();
  }else{
    turn_r();
  }


}

void turn_r() {
  Serial.println("Right");
  digitalWrite(L1, LOW); 
  analogWrite(L2, motor_speed);
  digitalWrite(R1, LOW);
  analogWrite(R2, motor_speed);
  digitalWrite(Roller, LOW);
  digitalWrite(MOP, LOW);
  digitalWrite(Dry, LOW);
} 

void turn_l() {
  Serial.println("Left");
  analogWrite(L1, motor_speed); 
  digitalWrite(L2, LOW);
  analogWrite(R1, motor_speed);
  digitalWrite(R2, LOW); 
  digitalWrite(Roller, LOW);
  digitalWrite(MOP, LOW);
  digitalWrite(Dry, LOW);
}


int readUsf() {
  digitalWrite(Usfl_Trig, LOW);
  delayMicroseconds(20);
  digitalWrite(Usfl_Trig, HIGH);
  delayMicroseconds(100);
  digitalWrite(Usfl_Trig, LOW);
//  distance= duration*0.034/2;
  int distancel = microsecondsToCentimeters(pulseIn(Usfl_Echo, HIGH));

  digitalWrite(Usfr_Trig, LOW);
  delayMicroseconds(20);
  digitalWrite(Usfr_Trig, HIGH);
  delayMicroseconds(100);
  digitalWrite(Usfr_Trig, LOW);
//  distance= duration*0.034/2;
  int distancer = microsecondsToCentimeters(pulseIn(Usfr_Echo, HIGH));

  if(distancel == 0)Serial.println(distancel);
  if(distancer == 0)Serial.println(distancer);

  int reading;
  if(distancel < obstacledistance || distancer < obstacledistance){
    reading = 1.0;
  }else{
    reading = 0;
  }
  return reading;
}

int readUsr() {
  digitalWrite(Usr_Trig, LOW);
  delayMicroseconds(20);
  digitalWrite(Usr_Trig, HIGH);
  delayMicroseconds(100);
  digitalWrite(Usr_Trig, LOW);
//  distance= duration*0.034/2;
  int distance = microsecondsToCentimeters(pulseIn(Usr_Echo, HIGH));
  if(distance == 0)Serial.println(distance);
  int reading;
  if(distance < obstacledistance){
    reading = 1.0;
  }else{
    reading = 0;
  }
  return reading;
}

int readUsl() {
  digitalWrite(Usl_Trig, LOW);
  delayMicroseconds(20);
  digitalWrite(Usl_Trig, HIGH);
  delayMicroseconds(100);
  digitalWrite(Usl_Trig, LOW);
//  distance= duration*0.034/2;
  int distance = microsecondsToCentimeters(pulseIn(Usl_Echo, HIGH));
  if(distance == 0)Serial.println(distance);
  int reading;
  if(distance < obstacledistance){
    reading = 1.0;
  }else{
    reading = 0;
  }
  return reading;
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}




void pump(){
  Serial.println("PumpON");
  digitalWrite(Pump, HIGH);
  delay(pumpOnTime);
  digitalWrite(Pump, LOW);
  Serial.println("PumpOFF");
  // delay (pumpOffTime);
  }


                    int predict(float *x) {
                        uint8_t votes[4] = { 0 };
                        // tree #1
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[2] <= 0.5) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #2
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[2] <= 0.5) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #3
                        if (x[2] <= 0.5) {
                            if (x[1] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[1] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #4
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[1] <= 0.5) {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #5
                        if (x[1] <= 0.5) {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #6
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[2] <= 0.5) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #7
                        if (x[2] <= 0.5) {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #8
                        if (x[1] <= 0.5) {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #9
                        if (x[1] <= 0.5) {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        else {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #10
                        if (x[2] <= 0.5) {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[1] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #11
                        if (x[2] <= 0.5) {
                            if (x[1] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[1] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #12
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[2] <= 0.5) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #13
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[2] <= 0.5) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #14
                        if (x[2] <= 0.5) {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #15
                        if (x[2] <= 0.5) {
                            if (x[1] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #16
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[2] <= 0.5) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #17
                        if (x[1] <= 0.5) {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #18
                        if (x[1] <= 0.5) {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #19
                        if (x[1] <= 0.5) {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #20
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[2] <= 0.5) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #21
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[2] <= 0.5) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #22
                        if (x[2] <= 0.5) {
                            if (x[1] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #23
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[2] <= 0.5) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #24
                        if (x[1] <= 0.5) {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #25
                        if (x[1] <= 0.5) {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #26
                        if (x[1] <= 0.5) {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #27
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[2] <= 0.5) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #28
                        if (x[2] <= 0.5) {
                            if (x[1] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[1] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #29
                        if (x[1] <= 0.5) {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #30
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[1] <= 0.5) {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #31
                        if (x[1] <= 0.5) {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #32
                        if (x[2] <= 0.5) {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[1] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #33
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[2] <= 0.5) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #34
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[1] <= 0.5) {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #35
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[1] <= 0.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #36
                        if (x[2] <= 0.5) {
                            if (x[1] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[1] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #37
                        if (x[1] <= 0.5) {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #38
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[2] <= 0.5) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #39
                        if (x[2] <= 0.5) {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[1] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #40
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[2] <= 0.5) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #41
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[1] <= 0.5) {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #42
                        if (x[2] <= 0.5) {
                            if (x[1] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #43
                        if (x[2] <= 0.5) {
                            if (x[1] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #44
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[1] <= 0.5) {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #45
                        if (x[1] <= 0.5) {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #46
                        if (x[1] <= 0.5) {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #47
                        if (x[2] <= 0.5) {
                            if (x[1] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[1] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #48
                        if (x[2] <= 0.5) {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #49
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[2] <= 0.5) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #50
                        if (x[1] <= 0.5) {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #51
                        if (x[1] <= 0.5) {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #52
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[1] <= 0.5) {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #53
                        if (x[2] <= 0.5) {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #54
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[1] <= 0.5) {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #55
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[1] <= 0.5) {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #56
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[2] <= 0.5) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #57
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[2] <= 0.5) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #58
                        if (x[2] <= 0.5) {
                            if (x[1] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[1] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #59
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[1] <= 0.5) {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #60
                        if (x[2] <= 0.5) {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #61
                        if (x[2] <= 0.5) {
                            if (x[1] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #62
                        if (x[1] <= 0.5) {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #63
                        if (x[1] <= 0.5) {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #64
                        if (x[1] <= 0.5) {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        else {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #65
                        if (x[1] <= 0.5) {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #66
                        if (x[1] <= 0.5) {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #67
                        if (x[2] <= 0.5) {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[1] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #68
                        if (x[2] <= 0.5) {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #69
                        if (x[1] <= 0.5) {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #70
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[1] <= 0.5) {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #71
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[1] <= 0.5) {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #72
                        if (x[1] <= 0.5) {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #73
                        if (x[2] <= 0.5) {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[1] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #74
                        if (x[1] <= 0.5) {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #75
                        if (x[1] <= 0.5) {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #76
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[2] <= 0.5) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #77
                        if (x[2] <= 0.5) {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #78
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[2] <= 0.5) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #79
                        if (x[2] <= 0.5) {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #80
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[1] <= 0.5) {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #81
                        if (x[1] <= 0.5) {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #82
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[1] <= 0.5) {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #83
                        if (x[1] <= 0.5) {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #84
                        if (x[2] <= 0.5) {
                            if (x[1] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #85
                        if (x[1] <= 0.5) {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #86
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[1] <= 0.5) {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #87
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[2] <= 0.5) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #88
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[2] <= 0.5) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #89
                        if (x[1] <= 0.5) {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #90
                        if (x[1] <= 0.5) {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #91
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[2] <= 0.5) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #92
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[1] <= 0.5) {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #93
                        if (x[2] <= 0.5) {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #94
                        if (x[2] <= 0.5) {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[1] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #95
                        if (x[1] <= 0.5) {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #96
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[1] <= 0.5) {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #97
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[1] <= 0.5) {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #98
                        if (x[1] <= 0.5) {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[2] <= 0.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[2] <= 0.5) {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5) {
                                    votes[3] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #99
                        if (x[2] <= 0.5) {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[0] <= 0.5) {
                                votes[3] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // tree #100
                        if (x[0] <= 0.5) {
                            votes[3] += 1;
                        }

                        else {
                            if (x[2] <= 0.5) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[1] <= 0.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        // return argmax of votes
                        uint8_t classIdx = 0;
                        float maxVotes = votes[0];

                        for (uint8_t i = 1; i < 4; i++) {
                            if (votes[i] > maxVotes) {
                                classIdx = i;                 
                                maxVotes = votes[i];
                            }
                        }

                        return classIdx;
                    };




// This is the code for our floor cleaning machine. Which build for Desig protype project. This code fully owned by Group R1, Sasindu malhara (2022/E/126)void setup() {

