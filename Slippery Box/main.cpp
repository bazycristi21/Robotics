#include <Wire.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <TFT_ILI9163C.h>
#include <EEPROM.h>

// Color definitions
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0  
#define WHITE   0xFFFF

/*
Teensy3.x and Arduino's
You are using 4 wire SPI here, so:
 MOSI:  11//Teensy3.x/Arduino UNO (for MEGA/DUE refere to arduino site)
 MISO:  12//Teensy3.x/Arduino UNO (for MEGA/DUE refere to arduino site)
 SCK:   13//Teensy3.x/Arduino UNO (for MEGA/DUE refere to arduino site)
 the rest of pin below:
 */
#define __CS 10
#define __DC 9

LiquidCrystal lcd(12,11,5,4,3,2);
float gyroRoll, gyroPitch;
float elapsedTime, currentTime, previousTime;
int c = 0;
bool chosenMode = false;
int modeChoice = -1;
const int pinSW = A2;
const int pinX = A0;
const int pinY = A1;
const int intensityLcdPin = 6;
int gameState = 0;
const int nrOptions = 4;
char *firstMenu[nrOptions] = {"Start","HighScore","About"};
int startPositionX[3]={1,1,11};
int startPositionY[3]={0,1,1};
int PositionX[nrOptions]={1,7,3};
int PositionY[nrOptions]={0,0,1};
int modeX[2] = {1,1};
int modeY[2] = {0,1};
char *modeOptions[2] = {"Campaign","Infinity"};
bool buttonPressed = false;
bool xAxisTilted = false;
bool yAxisTilted = false;
int buttonMax = 0;
int lcdCleared = 0;
int buttonNr = 0;
const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.
int16_t accelerometerX, accelerometerY, accelerometerZ; // variables for accelerometer raw data
int typingName = false;
bool gameStarted = false;
int timeLeft = 0;
byte gameScore = -1;
int newRecord = 0;
int gameResult = 1;
long totalRoll = 0;
long totalPitch = 0;
int ballSize = 11;
int obstacleSize = 5;
int exitSize = 6;
int ballX = 61,ballY = 61;
int exitX,exitY;
bool ballMoved = false;
int loseGame = 0;
int ballDirection = -1;
int levelTime = 30;
int currentChar = 0;
int charValue[5];
int xValue;
int yValue;
int swValue;
int gameSetted = 0;
long timeStart = 0;
byte maxScore = 0;
int lastTimeUpdate = 0;
int obstacleX,obstacleY;
bool obstacleActive = false;
bool exitActive = false;
int leftMargin = 5;
int rightMargin = 122;
int maxDisplayWidth = 128;

TFT_ILI9163C display = TFT_ILI9163C(__CS, 8, __DC);

void setup() {
  display.begin();
  display.clearScreen();
  lcd.begin(16,2);
  pinMode(pinX,INPUT);
  pinMode(pinY,INPUT);
  pinMode(pinSW,INPUT_PULLUP);
  pinMode(intensityLcdPin,INPUT);
  analogWrite(intensityLcdPin,100);
  Serial.begin(19200);
  
  Wire.begin();                      // Initialize comunication
  Wire.beginTransmission(MPU_ADDR);       // Start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);                  // Talk to the register 6B
  Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
  Wire.endTransmission(true);        //end the transmission
}

void clearSection(int x1,int y1,int x2,int y2)
{
  for(int i = y1; i < y2; i ++)
  {
    display.drawLine(x1,i,x2,i,BLACK);     // Clear the rectangle between x1,y1 and x2,y2
  }
}
void drawBall()
{
  if(ballDirection == 1) //moving to the right
  {
    display.fillRect(ballX - 12,ballY - 12, 12, ballSize + 34, BLACK);
  }
  if(ballDirection == 2)  // down
  {
    display.fillRect(ballX - 12, ballY - 12, ballSize + 34, 12, BLACK);
  }
  if(ballDirection == 3) // left
  {
    display.fillRect(ballX + ballSize, ballY - 12, 12, ballSize + 34, BLACK);
  }
  if(ballDirection == 4) // up
  {
    display.fillRect(ballX - 12,ballY + ballSize, ballSize + 34 , 12, BLACK);
  }
  display.fillRect(ballX,ballY,ballSize,ballSize,YELLOW);
}

void drawMargins()   // using this function we'll draw the game margins
{
  display.drawLine(leftMargin,leftMargin,leftMargin,rightMargin,BLUE);
  display.drawLine(leftMargin,rightMargin,rightMargin,rightMargin,BLUE);
  display.drawLine(rightMargin,rightMargin,rightMargin,leftMargin,BLUE);
  display.drawLine(rightMargin,leftMargin,leftMargin,leftMargin,BLUE);
}

void drawObstacles()  // for drawing obstacles
{
  if(obstacleActive == false)
  {
    obstacleActive = true;
    obstacleX = random(( min(ballX,exitX)+20) % 100 + 10 ,(max(ballX,exitX)-20) % 100 + 10);     // we use %100 and 10px delay to be sure that the obstacle is not spawning outside the margins
    obstacleY = random((min(ballY,exitY)+20) % 100 + 10, (max(ballY,exitY)-20) % 100 + 10);      // we use 20px delay to be sure that the obstacle is not overlaping the ball or the exit 
  }
  display.drawRect(obstacleX,obstacleY,obstacleSize,obstacleSize,RED);
}

void drawExit()  // for drawing exit / the points you need to take to increase your score.
{
  if(exitActive == false)   
  {
    exitActive = true;
    if(ballX < 61)          // anytime we check where the ball is located and we spawn the next exit in the oposite corner in order to make the game harder.
    {
      if(ballY < 61)
      {
        exitX = random(ballX+40,rightMargin - 20);   //   we use the 40px delay from the ball coords because we'll spawn the obstacles between the exit and the ball and we need to have enough space
        exitY = random(ballY+40,rightMargin - 20);
      }
      else
      {
        exitX = random(ballX+40,rightMargin - 20);
        exitY = random(10,ballY-40);
      }
    }
    else
    {
      if(ballY < 61)
      {
        exitX = random(10,ballX-40);
        exitY = random(ballY+40,rightMargin - 20);
      }
      else
      {
        exitX = random(10,ballX-40);
        exitY = random(10,ballY-40);
      }
    } 
  }
  display.fillRect(exitX,exitY,exitSize,exitSize,GREEN);
}
int detectCollision()
{
  if((ballX + ballSize >= exitX && ballX <= exitX + exitSize && ballY + ballSize >= exitY && ballY <= exitY + exitSize)) 
    return 1;
  if((ballX <= leftMargin || ballX + ballSize >= rightMargin || ballY <= leftMargin || ballY + ballSize >= rightMargin) || 
  (ballX + ballSize >= obstacleX && ballX <= obstacleX + obstacleSize && ballY + ballSize >= obstacleY  && ballY <= obstacleY + obstacleSize))
  {
    loseGame = 1;
    return 2;
  }
  return 3;
}

void clearObstacle()
{
  clearSection(obstacleX,obstacleY,obstacleX+obstacleSize,obstacleY+obstacleSize);
}
void clearExit()
{
  clearSection(exitX,exitY,exitX+exitSize+1,exitY+exitSize+1);
  clearObstacle();
}
void readGyro2()
{
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU_ADDR, 7*2, true); // request a total of 7*2=14 registers
  
  // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
  accelerometerX = Wire.read()<<8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
  accelerometerY = Wire.read()<<8 | Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
  accelerometerZ = Wire.read()<<8 | Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
  // This part ^ was took from the MPU6050 documentation
  
  gyroRoll = accelerometerX / 1000;
  gyroPitch = accelerometerY / 1000;
 
  if(gyroRoll < 7  && loseGame == 0)   // check if it's sloping right
  {
    ballX += 1;
    ballDirection = 1;
    ballMoved = true;
  }
  else if(gyroRoll > 13  && loseGame == 0)  // left
  {
    ballX -= 1;
    ballMoved = true;
    ballDirection = 3;
  }
  if(gyroPitch > -13 && loseGame == 0)   // up
  {
    ballY += 1;
    ballDirection = 2;
    ballMoved = true;
  }
  else if(gyroPitch < -20 && loseGame == 0)  // down
  {
    ballY -= 1;
    ballDirection = 4;
    ballMoved = true;
  }
}

void gameoverAnimation()
{
  for(int i = 0 ; i < 128; i++)
  {
    display.drawLine(i,i,128-i,i,RED);
    display.drawLine(128-i,i,128-i,128-i,RED);
    display.drawLine(128-i,128-i,i,128-i,RED);
    display.drawLine(i,128-i,i,i,RED);
    delay(12);
   }
}

void startAnimation(int color)
{
  for(int i = 127 ; i >= 0; i--)
  {
    display.drawLine(i,i,128-i,i,color);
    display.drawLine(128-i,i,128-i,128-i,color);
    display.drawLine(128-i,128-i,i,128-i,color);
    display.drawLine(i,128-i,i,i,color);
    delay(12);
  }
}


void readButtons()
{
  yValue = analogRead(pinX);
  xValue = analogRead(pinY);
  swValue = digitalRead(pinSW);
  if (swValue == 0 and buttonPressed == false)
  {
      if(typingName == true)
      {
        
        for(int i = 0 ; i < 5; i++)
        {
            EEPROM.write(i,char('A'+charValue[i]));
        }
        EEPROM.write(5,maxScore);
        gameState = 0;
      }
      else if(gameState == 0)
      {
        gameState = buttonNr + 1;
        lcdCleared = 0;
      }
      else
      {
        if(gameState == 1 && chosenMode == false)
        {
          modeChoice = buttonNr + 1;
          chosenMode = true;
        }
        else
          gameState = 0;
      }
      lcdCleared = 0;
      buttonPressed = true;
      buttonNr = 0;  
  }
  if(swValue == 1)
  {
    buttonPressed = false;
  }
  if(xValue > 700 && xAxisTilted == false)
    {
      if(buttonNr < buttonMax - 1)
        buttonNr ++;
      xAxisTilted = true;
      if(typingName == true && currentChar < 4)
      {
        currentChar ++; 
      }
    }
    if(xValue < 300 && xAxisTilted == false)
    {
      if(buttonNr > 0)
        buttonNr --;
      xAxisTilted = true;
      if(typingName == true && currentChar > 0)
        currentChar --;
    }
    if(xValue > 300 && xValue < 700)
    {
      xAxisTilted = false;
    }
    if(typingName == true)
    {
      if(yValue > 700 && yAxisTilted == false)
      {
        if('A' + charValue[currentChar] < 'Z')
          charValue[currentChar] ++;
        yAxisTilted = true;
      }
      if(yValue < 300 && yAxisTilted == false)
      {
        if(charValue[currentChar] > 0)
          charValue[currentChar] --;
        yAxisTilted = true;
      }
      if(yValue > 300 && yValue < 700)
      {
        yAxisTilted = false;
      }
    }
}
int playFree()
{
  if(gameStarted == false)
      return -1;   // if game is not started, exit
  if(gameSetted == 0)
  {
    lcd.clear();    // reset the values for the new game
    startAnimation(BLUE);
    newRecord = false;
    ballX = 61;
    ballY = 61;
    gameScore = -1;
    gameStarted = true;
    loseGame = 0;
    display.clearScreen();
    gameSetted = 1;
    timeStart = millis() / 1000;
    timeLeft = 30;
    gameResult = 1;
  }
  timeLeft = levelTime - (millis()/1000 - timeStart);
  
 
  drawMargins();
  drawBall();
  drawObstacles();
  drawExit();
  readGyro2();
  if(gameResult == 1)  // gameScore ++
  {
    clearExit();
    gameScore ++;
    exitActive = false;
    obstacleActive = false;
  }
  gameResult = 3;
  gameResult = detectCollision();
  lcd.setCursor(0,1);
  lcd.print("Score:");
  lcd.print(gameScore);
  lcd.setCursor(0,0);
  lcd.print("Time:");
  if(timeLeft < 0)
  {
    loseGame = 1;
    timeLeft = 0;
  }
  if(timeLeft > 0)
  {
    if(timeLeft >= 10)
      lcd.print(timeLeft);
    else
    {
       lcd.setCursor(6,0);
       lcd.print(" ");
       lcd.setCursor(5,0);
       lcd.print(timeLeft);
    }
  }
  else
  {
       lcd.setCursor(5,0);
       lcd.print("0");
       lcd.setCursor(6,0);
       lcd.print(" ");
       loseGame = 1; 
  }
  if(loseGame == 1)
  {
    gameStarted = false;
    timeLeft = 0;
    modeChoice = -1;
    chosenMode = false;
    maxScore = EEPROM.read(5);
    if(gameScore > maxScore)
    {
      newRecord = true;
      maxScore = gameScore;
      typingName = true;
      startAnimation(BLUE);
      lcd.setCursor(0,0);
      lcd.print("New record");
      delay(1500);
      lcd.clear();
      lcd.print("Enter your name:");
      while(true)      
      {
         readButtons();
         for(int i = 1; i < 10; i+= 2)
         {
            lcd.setCursor(i,1);
            lcd.print(char('A'+charValue[(i-1)/2]));   
         }
         lcd.setCursor(currentChar*2,1);
         lcd.print(">");
         for(int i = 0; i < 10; i+= 2)
         {
          if(i/2 != currentChar)
          { 
            lcd.setCursor(i,1);
            lcd.print(" ");
          }
         }
         if(gameState == 0)
          {
            lcdCleared = 0;
            typingName = false;
            break;
          }
       }
     }
     else
     {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Game over");
        lcd.setCursor(0,1);
        lcd.print("Score:");
        lcd.print(gameScore);
        gameoverAnimation();
        delay(1500);
        gameState = 0;
        lcdCleared = 0;
        loseGame = 0;
     }
  }
}

void displayMenu(int pos, int positionX[],int positionY[],char *menu[])
{
  lcd.setCursor(positionX[pos],positionY[pos]);
  lcd.print(menu[pos]); 
}

void displayFlag(int pos, int dimension, int positionX[],int positionY[])
{
  for(int i =0; i < dimension; i++)
  {
    lcd.setCursor(positionX[i] - 1,positionY[i]);
    lcd.print(" ");
  }
   lcd.setCursor(positionX[pos]-1,positionY[pos]);
   lcd.print(">"); 
}
void clearLCD()
{
    lcd.clear();
    lcdCleared = 1;
}

void loop(){
  maxScore = 0;
  readButtons();
  if(gameState == 0)  // main menu
  {
    gameStarted = false;
    buttonMax = 3;
    if(lcdCleared == 0 )
    { 
      clearLCD();
    }
    for (int i=0;i<nrOptions;i++)
    {
      displayMenu(i,PositionX,PositionY,firstMenu);
    }
    displayFlag(buttonNr,nrOptions,PositionX,PositionY);
    display.clearScreen();
  }
  if(gameState == 1) // game started
  {
    buttonNr = 1;
    if(lcdCleared == 0)
    {
      clearLCD();
      gameSetted = 0; 
    }
    if(gameStarted == false)
    {
      for(int i = 0 ; i < 2; i++)
      {
        displayMenu(i,modeX,modeY,modeOptions);
      }
      displayFlag(buttonNr ,2,modeX,modeY);
    } 
    
    if(modeChoice == 1)
    {
      gameStarted = true;
      //campaign();
    }
    if(modeChoice == 2){
      gameStarted = true;
      playFree();
      
    }
    else{
      readButtons(); 
    }
  }
  if(gameState == 2)  // highscore
  {
    if(lcdCleared == 0)
    {
      clearLCD();
      lcdCleared = 1;
    }
    buttonMax = 2;
    lcd.setCursor(0,0);
    for(int i = 0 ; i < 5; i++)
    {
      lcd.print(char(EEPROM.read(i)));
    }
    lcd.print(" ");
    lcd.print(EEPROM.read(5));
  }
  if(gameState == 3) // about
  {
    if(lcdCleared == 0)
    {
      clearLCD();
    }
    buttonMax = 0;
    lcd.setCursor(0,0);
    lcd.print("SlipperyBox v1.0");
    lcd.setCursor(0,1);
    lcd.print("Made by Bazy");
  }
} 
