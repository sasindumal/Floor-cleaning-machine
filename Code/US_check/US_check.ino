#define Usfl_Echo A0
#define Usfl_Trig A1

#define Usfr_Echo 10
#define Usfr_Trig 13

#define Usl_Echo A2
#define Usl_Trig A3

#define Usr_Echo A4
#define Usr_Trig A5

void setup() {
  Serial.begin(9600);

  pinMode(Usfl_Trig, OUTPUT);
  pinMode(Usfl_Echo, INPUT);
  pinMode(Usfr_Trig, OUTPUT);
  pinMode(Usfr_Echo, INPUT);
  pinMode(Usl_Trig, OUTPUT);
  pinMode(Usl_Echo, INPUT);
  pinMode(Usr_Trig, OUTPUT);
  pinMode(Usr_Echo, INPUT);
}

void loop() {
  float distanceFL = measureDistance(Usfl_Trig, Usfl_Echo);
  float distanceFR = measureDistance(Usfr_Trig, Usfr_Echo);
  float distanceL = measureDistance(Usl_Trig, Usl_Echo);
  float distanceR = measureDistance(Usr_Trig, Usr_Echo);

  Serial.print("Front Left Distance: ");
  Serial.print(distanceFL);
  Serial.println(" cm");
  Serial.print("Front Right Distance: ");
  Serial.print(distanceFR);
  Serial.println(" cm");
  Serial.print("Left Distance: ");
  Serial.print(distanceL);
  Serial.println(" cm");
  Serial.print("Right Distance: ");
  Serial.print(distanceR);
  Serial.println(" cm");

  delay(1000);
}

float measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(20);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(30);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2;

  return distance;
}
