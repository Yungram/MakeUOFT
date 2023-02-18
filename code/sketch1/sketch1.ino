const int SensorCenter = 0;

const int UltraSensorTrig = 4;  //out
const int UltraSensorEcho = 5;  //in
const int InfraredSensorPin = 8;
const int buzzPin = 9;
const int UltraSensorTrig2 = 10;
const int UltraSensorEcho2 = 11;
const int buzzerPinLeft = 12;
const int buzzerPinRight = 13;

double power_right = 0;
double power_left = 0;

void setup() {
  // put your setup code here, to run once:

  pinMode(InfraredSensorPin, INPUT);

  pinMode(UltraSensorTrig, OUTPUT);
  pinMode(UltraSensorEcho, INPUT);

  pinMode(UltraSensorTrig2, OUTPUT);
  pinMode(UltraSensorEcho2, INPUT);

  pinMode(buzzPin, OUTPUT);
  pinMode(buzzerPinLeft, OUTPUT);

  // Setup camera

  Serial.begin(9600);
  //Sensor down initial value if detect big change then trigger, if reset to around initial
  //Accelarometer for falling detection
}

void loop() {

  // power values reset
  power_right = 0;
  power_left = 0;

  // get sensor data
  digitalWrite(UltraSensorTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(UltraSensorTrig, HIGH);
  delayMicroseconds(2);
  digitalWrite(UltraSensorTrig, LOW);

  double duration_right = pulseIn(UltraSensorEcho, HIGH);
  double distance_right = (duration_right / 2) * 0.343;
  Serial.print("Right Sensor Value: ");
  Serial.println(distance_right);  //in centimeters 0-15000

  digitalWrite(UltraSensorTrig2, LOW);
  delayMicroseconds(2);
  digitalWrite(UltraSensorTrig2, HIGH);
  delayMicroseconds(2);
  digitalWrite(UltraSensorTrig2, LOW);

  double duration_left = pulseIn(UltraSensorEcho2, HIGH);
  double distance_left = (duration_left / 2) * 0.343;
  Serial.print("Left Sensor Value: ");
  Serial.println(distance_left);  //in centimeters 0-15000

  // Right USS trip
  if (distance_right < 2000) {
    power_right = 1 - (distance_right / 2000);
    Serial.print("Right Buzzer Warning Intensity: ");
    Serial.println(power_right);
  }

  // Left USS trip
  if (distance_left < 2000) {
    power_left = 1 - (distance_left / 2000);
    Serial.print("Left Buzzer Warning Intensity: ");
    Serial.println(power_left);
  }

  // Get IR sensor value
  double distanceCenter = 0;

  distanceCenter = digitalRead(InfraredSensorPin);
  Serial.print("IR Sensor Value: ");
  Serial.println(distanceCenter);

  //check for downward buzzer trip
  if (distanceCenter == 0) {
    Serial.println("IR Value Tripped");
    // Downward sensor tripped
  }

  Serial.print("Sensor value:");
  Serial.println(SensorCenter);

  // set center buzzer intensity
  double centerBuzzer = (power_left * .50) + (power_right * .50);
  Serial.println(centerBuzzer);

  // set right and left buzzer intensity
  double leftBuzzer = power_left;
  double rightBuzzer = power_right;

  // trigger center buzzer
  if (centerBuzzer > 0) {
    analogWrite(buzzPin, 3 * centerBuzzer);
    delay(10);
    analogWrite(buzzPin, 0);
  }

  //trigger left buzzer
  if (leftBuzzer > 0) {
    tone(buzzerPinLeft, 2500);
    delay(15);
    noTone(buzzerPinLeft);
  }

  // trigger right buzzer
  if (rightBuzzer > 0) {
    tone(buzzPinRight, 2500);
    delay(15);
    noTone(buzzerPinRight);
  }

  // get vision detection response
  // if confidence rate works out to confident object
  // buzzer will have certain patterns for different objects

  if (camera == "STAIRSUP") {
    //Tone Ascending infront buzzer
    for (int i = 0; i < 10; i++) {
      tone(buzzPin, i * centerBuzzer);
    }
    noTone(buzzPin);
  }

  if (camera == "STAIRSDOWN") {
    //Tone Descending infront buzzer
    for (int i = 10; i > 0; i--) {
      tone(buzzPin, i * centerBuzzer);
    }
    noTone(buzzPin);
  }

  if (camera == "CHAIR") {
    analogWrite(buzzPin, 3 * centerBuzzer);
    delay(25);
    analogWrite(buzzPin, 3 * centerBuzzer);
    delay(25);
    analogWrite(buzzPin, 0);
    //Long Same Tone infront buzzer
  }
  
  if (camera == "TABLE") {
    //Double Medium Length Tone infront buzzer
    analogWrite(buzzPin, 3 * centerBuzzer);
    delay(50);
    analogWrite(buzzPin, 0);
  }

  // get accelerometer data
  // detect large change, turn fall mode on, every cycle while on will add to counter
  // set buzzer
  // if detect movement again end fall mode, reset counter
  // if fall mode cycles up to 20, then SOS

  // wait 500ms
  Serial.println("================");
  delay(500);
}
