const int pinA = 12;
const int pinB = 8;
const int pinC = 5;
const int pinD = 3;
const int pinE = 2;
const int pinF = 11;
const int pinG = 6;
const int pinDP = 4;
const int pinD1 = 7;
const int pinD2 = 9;
const int pinD3 = 10;
const int pinD4 = 13;
const int segSize = 8;
const int noOfDisplays = 4;
const int noOfDigits = 10;
const int pinX = A0;
const int pinY = A1;
const int pinSW = 3;

int numbers[4];
int xValue,yValue;
int lastDebounceTime = 0;
int debounceDelay =50;
int number=1;
int digit=0;
int readingX,readingY=0;
int xAxisTilted = false;
int yAxisTilted = false;
int dpState = LOW;
int currentNumber = 0;
unsigned long delayCounting = 50; // incrementing interval
unsigned long lastIncreasing = 0;
int swPressed = 0;
int readingSW,swValue=5;
int swBack=1;

// segments array, similar to before
int segments[segSize] = {
pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};
// digits array, to switch between them easily
int digits[noOfDisplays] = {
pinD1, pinD2, pinD3, pinD4
};
byte digitMatrix[noOfDigits][segSize - 1] = {
// a b c d e f g
{1, 1, 1, 1, 1, 1, 0}, // 0
{0, 1, 1, 0, 0, 0, 0}, // 1
{1, 1, 0, 1, 1, 0, 1}, // 2
{1, 1, 1, 1, 0, 0, 1}, // 3
{0, 1, 1, 0, 0, 1, 1}, // 4
{1, 0, 1, 1, 0, 1, 1}, // 5
{1, 0, 1, 1, 1, 1, 1}, // 6
{1, 1, 1, 0, 0, 0, 0}, // 7
{1, 1, 1, 1, 1, 1, 1}, // 8
{1, 1, 1, 1, 0, 1, 1} // 9
};

void displayNumber(byte digit, byte decimalPoint) {
  for (int i = 0; i < segSize - 1; i++) {
    digitalWrite(segments[i], digitMatrix[digit][i]);
  }
  // write the decimalPoint to DP pin
  for(int i=0;i<4;i++)
    if(i==digit)
      digitalWrite(pinDP,LOW);
  }
// activate the display no. received as param
void showDigit(int num) {
  for (int i = 0; i < noOfDisplays; i++) {
    digitalWrite(digits[i], HIGH);  
    digitalWrite(pinDP, HIGH);
  }
  digitalWrite(digits[num], LOW);
  
}

void setup() {
  
for (int i = 0; i < segSize - 1; i++)
{
pinMode(segments[i], OUTPUT);
}
for (int i = 0; i < noOfDisplays; i++)
{
pinMode(digits[i], OUTPUT);
}
pinMode(pinX,INPUT);
pinMode(pinY,INPUT);
pinMode(pinSW,INPUT);
Serial.begin(9600);
}


void loop() {
    digitalWrite(pinDP,LOW);
    for(int i = 0; i < 4; i++)
    {
      showDigit(i);
      displayNumber(numbers[i], LOW);
      delay(5);
    // increase this delay to see multiplexing in action. At about 100 it
    }   
    readingX = analogRead(pinY);
    xValue = readingX;
    readingSW = digitalRead(pinSW);
    Serial.println(readingSW);
    if(readingSW == 0 && swPressed == 0 && swBack == 1)
    {
      swPressed = 1;
      swBack = 0;
    }
    else if(swBack == 1 && swPressed == 1 && readingSW == 0)
    {
      swPressed = 0;
      swBack = 0;  
    }
    if(readingSW > 0)
    {
      swBack = 1;
    }
    
    
    if(xValue > 700 && digit > 0 && xAxisTilted == false && swPressed == 0)
    {
         digit--; // move to next display
         xAxisTilted = true;
    }
    if(xValue < 300 && digit < 4 && xAxisTilted == false && swPressed == 0)
    {
        digit++;
        xAxisTilted = true;
    }    
    if(xValue > 300 && xValue <700)
      xAxisTilted = false;
      
    readingY = analogRead(pinX);
    yValue = readingY;
    if(yValue > 700 and yAxisTilted == false and numbers[digit] < 9 && swPressed == 1){
        
        numbers[digit]++;
        yAxisTilted = true;
    }
    if(yValue < 300 and yAxisTilted == false and numbers[digit] > 0 && swPressed == 1)
    {
        numbers[digit]--;
        yAxisTilted = true;
    }
    if(yValue > 300 && yValue < 700)
    {
      yAxisTilted = false;
    }
    // increment the number
   }
  
