// This is the code for our floor cleaning machine. Which build for Desig protype project. This code fully owned by Group R1, Sasindu malhara (2022/E/126)



// Define pins
#define FL1 2
#define FL2 4
#define FLENB 3

#define BL1 14
#define BL2 15
#define BLENB 16

#define FR1 17
#define FR2 18
#define FRENB 19

#define BR1 5
#define BR2 7
#define BRENB 6

#define RO1 20
#define RO2 21
#define ROENB 22

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
const long pumpOnTime = 500; 
const long pumpOffTime = 2000; 
int obstacledistance = 30;


float pred[3];

void hardware_setup(){
	new DCMotor_Hbridge(FL1, FL2, FLENB, "FL", 0.1176798, 20, 1);
  new DCMotor_Hbridge(BL1, BL2, BLENB, "BL", 0.1176798, 20, 1);
  new DCMotor_Hbridge(FR1, FR2, FRENB, "FR", 0.1176798, 20, 1);
  new DCMotor_Hbridge(BR1, BR2, BRENB, "BR", 0.1176798, 20, 1);
  new DCMotor_Hbridge(RO1, RO2, ROENB, "Roller", 0.1176798, 20, 1);



}


void setup() {

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
  Serial.println("Forward");

  analogWrite(RO1, motor_speed);
  digitalWrite(RO2, LOW);   
   
}

