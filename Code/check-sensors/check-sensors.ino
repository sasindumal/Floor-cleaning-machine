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

void setup() {
  // Initialize the serial communication
  Serial.begin(9600);

  // Initialize the sensor pins
  pinMode(Usf_Trig, OUTPUT);
  pinMode(Usf_Echo, INPUT);
  pinMode(Usl_Trig, OUTPUT);
  pinMode(Usl_Echo, INPUT);
  pinMode(Usr_Trig, OUTPUT);
  pinMode(Usr_Echo, INPUT);

  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(LENB, OUTPUT);
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(RENB, OUTPUT);
  pinMode(Roller, OUTPUT);
  pinMode(MOP, OUTPUT);
}

void loop() {

   long distanceLeft = readUltrasonicDistance(Usf_Trig, Usf_Echo);

  if (distanceLeft > 30) {
    // Move forward if no obstacle
    digitalWrite(L1, LOW); 
    analogWrite(L2, motor_speed);
    analogWrite(R1, motor_speed);
    digitalWrite(R2, LOW);   
    
  } else {
    // Stop the robot if an obstacle is detected
    digitalWrite(L1, LOW);
    digitalWrite(L2, LOW);
    
    digitalWrite(R1, LOW);
    digitalWrite(R2, LOW);
    
  }

    long distanceL = readUltrasonicDistance(Usl_Trig, Usl_Echo);

  if (distanceL > 30) {
    // Move forward if no obstacle
    digitalWrite(Roller, HIGH);   
    
  } else {
    // Stop the robot if an obstacle is detected
   digitalWrite(Roller, LOW);   
  }

  long distanceR = readUltrasonicDistance(Usr_Trig, Usr_Echo);

  if (distanceL > 30) {
    // Move forward if no obstacle
    digitalWrite(MOP, HIGH);   
    
  } else {
    // Stop the robot if an obstacle is detected
   digitalWrite(MOP, LOW);   
  }


/*
  // Read and print distance from left sensor
 

  if (distanceLeft > 30){
  analogWrite(L1, motor_speed); 
  digitalWrite(L2, LOW);
  analogWrite(R1, motor_speed);
  digitalWrite(R2, LOW); 
  Serial.print("Left Distance: ");
  Serial.print(distanceLeft);
  Serial.println(" cm");
  }

  // Read and print distance from right sensor
  long distanceRight = readUltrasonicDistance(Usr_Trig, Usr_Echo);

  if (distanceRight > 30){
  
  Serial.println("Right");
  digitalWrite(L1, LOW); 
  analogWrite(L2, motor_speed);
  digitalWrite(R1, LOW);
  analogWrite(R2, motor_speed);
  
  Serial.print("Right Distance: ");
  Serial.print(distanceRight);
  Serial.println(" cm");
  }
*/
  // Wait a bit before next measurement
  delay(500);
}

long readUltrasonicDistance(int trigPin, int echoPin) {
  // Send a 10us pulse to trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the time for the echo to return
  long duration = pulseIn(echoPin, HIGH);

  // Calculate the distance (duration / 2) / 29.1
  long distance = (duration / 2) / 29.1;

  return distance;
}
