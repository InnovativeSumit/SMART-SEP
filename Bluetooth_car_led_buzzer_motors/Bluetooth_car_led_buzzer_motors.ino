#define in1 7 //Right
#define in2 6 //Right
#define en1 3 //right speed

#define in3 10   //Left
#define in4 11   //Left
#define en2 5   //Left speed

#define LED 4          // Front lights  pin
#define LED2 A1        // Back lights pin
const int buzzer = A0; //Buzzer to arduino pin A0 (Horn)


int command; //Int to store app command state.

int pwm_speedA = 255;  //forward speed
int pwm_speedB = 48;  //turn speed

void setup() {

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);  
  pinMode(en1, OUTPUT);
   
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(en2, OUTPUT);
  
  pinMode(LED, OUTPUT);   //Set the Front Lights pin.
  pinMode(LED2, OUTPUT); //Set the Back lights.

  pinMode(buzzer, OUTPUT);  //buzzer to arduino pin A0 (Horn)
  
  Serial.begin(9600);  //Set the baud rate to your Bluetooth module.
}

void loop() {
  
  if (Serial.available() > 0) {
    command = Serial.read();
    Stop(); //Initialize with motors stoped.
    switch (command) {
     
///////////////////////////////////// Turn on/off Front lights
     case 'W':
        digitalWrite(LED, HIGH);
         break;
      
     case 'w':
        digitalWrite(LED, LOW);
         break;
//////////////////////////////////////   Turn on/off Back lights
      case 'U':
        digitalWrite(LED2, HIGH);
        break;
      
      case 'u':
        digitalWrite(LED2, LOW);
        break;
/////////////////////////////////////// Toggle on/off Horn
      case 'V':
        tone(buzzer, 2500); // Send 1KHz sound signal...
        break;
      
      case 'v':
          noTone(buzzer);// Stop sound...
        break;
////////////////////////////////////////
      
      case 'F':
        forward();
        break;
      case 'B':
        back();
        break;
      case 'L':
        left();
        break;
      case 'R':
        right();
        break;
      case 'G':
        forwardleft();
        break;
      case 'I':
        forwardright();
        break;
      case 'H':
        backleft();
        break;
      case 'J':
        backright();
        break;
      case '0':
        pwm_speedA = 100;
        break;
      case '1':
        pwm_speedA = 140;
        break;
      case '2':
        pwm_speedA = 153;
        break;
      case '3':
        pwm_speedA = 165;
        break;
      case '4':
        pwm_speedA = 178;
        break;
      case '5':
        pwm_speedA = 191;
         break;
      case '6':
        pwm_speedA = 204;
        break;
      case '7':
        pwm_speedA=216;
        
        break;
      case '8':
        pwm_speedA = 229;
        break;
      case '9':
        pwm_speedA = 242;
        break;
      case 'q':
        pwm_speedA = 255;      
        break;
    }
    
  }
}

void forward() {
  digitalWrite(in3, HIGH);  
  analogWrite(en2,pwm_speedA); 
  
  digitalWrite(in2, HIGH);  
  analogWrite(en1,pwm_speedA);

}

void back() {
  digitalWrite(in1, HIGH);  
  analogWrite(en1,pwm_speedA); 
  
  digitalWrite(in4, HIGH);  
  analogWrite(en2,pwm_speedA);
}

void left() {
  digitalWrite(in3, HIGH);
  analogWrite(en2,pwm_speedA);
  
  digitalWrite(in1, HIGH);
  analogWrite(en1,pwm_speedA);
}

void right() {
  digitalWrite(in2, HIGH);
analogWrite(en1,pwm_speedA);

digitalWrite(in4, HIGH);
  analogWrite(en2,pwm_speedA);
  }
void forwardleft() {
  digitalWrite(in3, HIGH);  
  analogWrite(en2,pwm_speedA); 
  
  digitalWrite(in2, HIGH);  
  analogWrite(en1,pwm_speedB);
  
}
void forwardright() {
  digitalWrite(in3, HIGH);  
  analogWrite(en2,pwm_speedB); 
  
  digitalWrite(in2, HIGH);  
  analogWrite(en1,pwm_speedA);
}
void backright() {
  digitalWrite(in1, HIGH);
  analogWrite(en1,pwm_speedA);
    
    digitalWrite(in4, HIGH);
analogWrite(en2,pwm_speedB);
}
void backleft() {
  digitalWrite(in1, HIGH);
  analogWrite(en1,pwm_speedB);
      
  digitalWrite(in4, HIGH);
analogWrite(en2,pwm_speedA);
}

void Stop() {
  analogWrite(in1, 0);
  analogWrite(in2, 0);
  analogWrite(in3, 0);
  analogWrite(in4, 0);
}