const int pinRED = 9;
const int pinBLUE = 11;
const int pinGREEN = 10;
const int pinPOTred = A0;
const int pinPOTblue = A1;
const int pinPOTgreen = A2;
int valuePOTred= 0;
int valuePOTblue= 0;
int valuePOTgreen= 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(pinRED,OUTPUT);
  pinMode(pinBLUE, OUTPUT);
  pinMode(pinGREEN, OUTPUT);
  pinMode(pinPOTred, INPUT);
  pinMode(pinPOTblue, INPUT);
  pinMode(pinPOTgreen, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
    valuePOTred = analogRead(pinPOTred);
    valuePOTblue = analogRead(pinPOTblue);
    valuePOTgreen = analogRead(pinPOTgreen);
    analogWrite(pinRED,valuePOTred/4);
    analogWrite(pinBLUE,valuePOTblue/4);
    analogWrite(pinGREEN,valuePOTgreen/4);
    Serial.println(valuePOTblue);
}
