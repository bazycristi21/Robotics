const int speakerPin = A0;
const int ledPin = 13;
const int buttonPin = 2;
const int threshold = 700;

int speakerValue = 0;
int buttonValue = 0;
int ledValue = 0;
int knockTime =-1;
int time1 = -1;
int flag =0;


void setup() {
  pinMode(speakerPin,INPUT);
  pinMode(buttonPin,INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);  
}
void loop() {
  time1 = millis();
  speakerValue = analogRead(speakerPin);
  int buttonState = digitalRead(buttonPin);
  buttonValue = !buttonState;
  if(speakerValue > threshold && flag ==0)
  {
      knockTime = millis();
      Serial.println(speakerValue);
      flag = 1;
  }
  if(time1 - knockTime > 5000 && knockTime != -1)
  {
      analogWrite(ledPin, speakerValue);
  }
  if(buttonValue == HIGH && time1- knockTime>5000)
  {
      analogWrite(ledPin,0);
      knockTime=-1;
      flag =0;
  }
  
        
}
