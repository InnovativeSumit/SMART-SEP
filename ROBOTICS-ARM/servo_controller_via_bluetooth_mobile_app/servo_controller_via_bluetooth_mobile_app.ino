#include <Servo.h>
Servo Servo1, Servo2, Servo3, Servo4, Servo5, Servo6;
unsigned int temp_value1;
unsigned int temp_value2;

void set_allservo(int value) {
  Servo1.write(value);
  Servo2.write(value);
  Servo3.write(value);
  Servo4.write(value);
  Servo5.write(value);
  Servo6.write(value);
  delay(2);
}

void set_servo(int values) {
  if (values >= 0 && values <= 180) {
    int myservo1 = values;
    Servo1.write(myservo1);
    delay(2);

  }

  else if (values >= 181 && values <= 361) {
    int myservo2 = values;
    myservo2 = map(myservo2, 181, 361, 0, 180);
    Servo2.write(myservo2);
    delay(2);

  }

  else if (values >= 362 && values <= 542) {
    int myservo3 = values;
    myservo3 = map(myservo3, 362, 542, 0, 180);
    Servo3.write(myservo3);
    delay(2);

  }

  else if (values >= 543 && values <= 723) {
    int myservo4 = values;
    myservo4 = map(myservo4, 543, 723, 0, 180);
    Servo4.write(myservo4);
    delay(2);

  }

  else if (values >= 724 && values <= 904) {
    int myservo5 = values;
    myservo5 = map(myservo5, 724, 904, 0, 180);
    Servo5.write(myservo5);
    delay(2);

  }

  else if (values >= 905 && values <= 1085) {
    int myservo6 = values;
    myservo6 = map(myservo6, 905, 1085, 0, 180);
    Servo6.write(myservo6);
    delay(2);

  }

  else if (values >= 11000 && values <= 11180) {
    set_allservo(values - 11000);
  }
}

void setup() {
  Serial.begin(9600);

  Servo1.attach(6);
  Servo2.attach(8);
  Servo3.attach(9);
  Servo4.attach(12);
  Servo5.attach(13);
  Servo6.attach(14);
  delay(5);

  Servo1.write(0);
  Servo2.write(0);
  Servo3.write(0);
  Servo4.write(0);
  Servo5.write(0);
  Servo6.write(0);
  delay(5);
}

void loop() {
  if (Serial.available() > 1) {
    temp_value1 = Serial.read();
    temp_value2 = Serial.read();
    set_servo((temp_value2 * 256) + temp_value1);
  }
}