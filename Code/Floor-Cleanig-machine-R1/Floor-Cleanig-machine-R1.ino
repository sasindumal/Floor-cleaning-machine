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

#define Usf_Echo A0
#define Usf_Trig A1

#define Usl_Echo A2
#define Usl_Trig A3

#define Usr_Echo A4
#define Usr_Trig A5

int motor_speed = 255;
int autoModeOn = 1;
char incomingByte;
bool pumpState = false;
const long pumpOnTime = 500; 
const long pumpOffTime = 2000; 
int obstacledistance = 30;


float pred[3];

void setup() {
  pinMode(Pump, OUTPUT);
  pinMode(MOP, OUTPUT);
  pinMode(Roller, OUTPUT);
  pinMode(Dry, OUTPUT);
  pinMode(Usf_Trig, OUTPUT); 
  pinMode(Usf_Echo, INPUT);

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

  if (Serial.available() > 0)   //check if any data is available
  {
    incomingByte = Serial.read();   //read incoming data
    Serial.println(incomingByte);

    if (incomingByte == 'X'){
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
  if (Serial.available() > 0)   //check if any data is available
  {
    incomingByte = Serial.read();   //read incoming data
    Serial.println(incomingByte);
  }
  if (pumpState){
    pump();
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
    break;

    case 'f':
    Serial.println("Stop");
    digitalWrite(L1, LOW); 
    digitalWrite(L2, LOW);
    digitalWrite(R1, LOW);
    digitalWrite(R2, LOW);
    break;
    
    case 'B':
    Serial.println("Backward");
    digitalWrite(L2, LOW); 
    analogWrite(L1, motor_speed);
    analogWrite(R2, motor_speed);
    digitalWrite(R1, LOW);
    break;
    
    case 'b':
    Serial.println("Stop");
    digitalWrite(L1, LOW); 
    digitalWrite(L2, LOW);
    digitalWrite(R1, LOW);
    digitalWrite(R2, LOW);
    break;
    
    case 'L':
    Serial.println("Left");
    analogWrite(L1, motor_speed); 
    digitalWrite(L2, LOW);
    analogWrite(R1, motor_speed);
    digitalWrite(R2, LOW);
    break;
    
    case 'l':
    Serial.println("Stop");
    digitalWrite(L1, LOW); 
    digitalWrite(L2, LOW);
    digitalWrite(R1, LOW);
    digitalWrite(R2, LOW);
    break;

    case 'R':
    Serial.println("Right");
    digitalWrite(L1, LOW); 
    analogWrite(L2, motor_speed);
    digitalWrite(R1, LOW);
    analogWrite(R2, motor_speed);
    break;
    
    case 'r':
    Serial.println("Stop");
    digitalWrite(L1, LOW); 
    digitalWrite(L2, LOW);
    digitalWrite(R1, LOW);
    digitalWrite(R2, LOW);
    break;

    case 'S':
    Serial.println("Stop");
    digitalWrite(L1, LOW); 
    digitalWrite(L2, LOW);
    digitalWrite(R1, LOW);
    digitalWrite(R2, LOW);
    break;

    case 'N':
    pumpState = true;
    break;
    
    case 'n':
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
  digitalWrite(L2, LOW); 
  analogWrite(L1, motor_speed);
  analogWrite(R2, motor_speed);
  digitalWrite(R1, LOW); 
  digitalWrite(Roller, LOW);
  digitalWrite(MOP, LOW);
  digitalWrite(Dry, LOW);
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
  digitalWrite(Usf_Trig, LOW);
  delayMicroseconds(20);
  digitalWrite(Usf_Trig, HIGH);
  delayMicroseconds(100);
  digitalWrite(Usf_Trig, LOW);
//  distance= duration*0.034/2;
  int distance = microsecondsToCentimeters(pulseIn(Usf_Echo, HIGH));
  if(distance == 0)Serial.println(distance);
  int reading;
  if(distance < obstacledistance){
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
  digitalWrite(Pump, HIGH);
  delay(pumpOnTime);
  digitalWrite(Pump, LOW);
  delay (pumpOffTime);
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




// This is the code for our floor cleaning machine. Which build for Desig protype project. This code fully owned by Group R1, Sasindu malhara (2022/E/126)