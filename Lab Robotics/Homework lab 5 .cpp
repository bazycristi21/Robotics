#include <LiquidCrystal.h>
#include <EEPROM.h>
const int pinSW = 10;
const int pinX = A0;
const int pinY = A1;


LiquidCrystal lcd(12,11,5,4,3,2);

void setup(){
  Serial.begin(9600);
  pinMode(pinX,INPUT);
  pinMode(pinY,INPUT);
  pinMode(pinSW,INPUT_PULLUP);
  lcd.begin(16,2);
  lcd.clear();
}
int gameState = 0;
const int nrOptions = 4;
char *Menu[nrOptions] = {"My game","Start","Score","Settings"};
char *Start[nrOptions] = {"Lives:3","Level:", "Score:","Exit"};
char *Score[2] = {"Score:","Exit"};
char *Settings[2] ={"Level:","Exit"};
int settingsPositionX[2]={4,4};
int settingsPositionY[2]={0,1};
int scorePositionX[2]={4,4};
int scorePositionY[2]={0,1};
int startPositionX[4]={1,9,1,11};
int startPositionY[4]={0,0,1,1};
int PositionX[nrOptions]={1,9,1,8};
int PositionY[nrOptions]={0,0,1,1};
bool buttonPressed = false;
bool xAxisTilted = false;
bool yAxisTilted = false;
int maxScore = 0;
int score = 0;
int buttonMax = 0;
int cleared = 0;
int buttonNr = 1;
int startingLevel = 0;
int Level = 0;
int setLevel = 0;
int lastUpdate = 0;
int started = 0;
int adr =0;
int val;  


void displayMenu(int pos, int positionX[],int positionY[],char *menu[])
{
  
  lcd.setCursor(positionX[pos],positionY[pos]);
  lcd.print(menu[pos]);
  
}
int timpStart;
byte value;
void displayFlag(int pos, int dimension, int positionX[],int positionY[])
{
  for(int i =0; i < dimension; i++)
  {
    lcd.setCursor(positionX[i] - 1,positionY[i]);
    lcd.print(" ");
  }
  if(buttonNr != 0)
  {
    lcd.setCursor(positionX[pos]-1,positionY[pos]);
    lcd.print(">");
  }
}
int timpUpdate = 0;
int timpUpdate2 = 0;
int firstLevel;

void loop(){
  maxScore = EEPROM.read(adr);
  Serial.println("gamestate:");
  Serial.print(gameState);
  int yValue = analogRead(pinX);
  int xValue = analogRead(pinY);
 
  int swValue = digitalRead(pinSW);
  if (swValue == 0 and buttonPressed == false)
  {
    if((gameState == 1 and buttonNr == 3) or (gameState == 2 and buttonNr ==1) or (gameState == 3 and buttonNr == 1))
    {
      buttonPressed = true;
      gameState = 0;
      cleared = 0;   
      buttonNr = 1;
    }
    else{
      gameState = buttonNr;
      buttonPressed = true;
      cleared = 0;  
      buttonNr = 1;
    }
    
  }
  if(swValue == 1)
  {
    buttonPressed = false;
  }
  if(xValue > 700 && buttonNr < buttonMax-1  && xAxisTilted == false)
    {
      buttonNr ++;
      xAxisTilted = true;
    }
    if(xValue < 300 && buttonNr > 1 && xAxisTilted == false)
    {
      buttonNr --;
      xAxisTilted = true;
    }
    if(xValue > 300 && xValue < 700)
    {
      xAxisTilted = false;
    }
   if(yValue > 700  && yAxisTilted == false)
    {
      
      if(gameState == 3)
      {
        startingLevel++;
        setLevel = 0;
      }
      else
      {
        cleared = 0;
        if(gameState == 0)
          gameState = buttonNr;
        else
          gameState = 0;
      }
      yAxisTilted = true;
    }
   if(xValue < 500 && yAxisTilted == false)
    {
      if(gameState == 3 && startingLevel > 0)
      {
        startingLevel--;
        setLevel = 0;
      }
      else
      {
        cleared = 0;
        if(gameState == 0)
          gameState = buttonNr;
        else
          gameState = 0;
      }
      yAxisTilted = true;
    }
   if(yValue > 300 && yValue < 700)
    {
      yAxisTilted = false;
    }
  if(gameState == 0)
  {
    buttonMax = 4;
    if(cleared == 0)
    {
      lcd.clear();
      cleared = 1;
    }
    for (int i=0;i<nrOptions;i++)
    {
      displayMenu(i,PositionX,PositionY,Menu);
    }
    displayFlag(buttonNr,nrOptions,PositionX,PositionY);
  }
  Serial.println(buttonNr);
  if(gameState == 1)
  {
    
    started = 1;
   
    if(cleared == 0)
    {
      Level = startingLevel;
      score = 3*Level;
      lcd.clear();
      cleared = 1;
      timpStart = millis();
      timpUpdate2 = 0;
      firstLevel = Level;
    }
    
   
    if(Level -firstLevel == 2)
    {
      delay(1000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Congratulations");
      lcd.setCursor(0,1);
      lcd.print("you finished the");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("you finished the");
      lcd.setCursor(0,1);
      lcd.print("game.Press the");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("game.Press the");
      lcd.setCursor(0,1);
      lcd.print("button to exit");
      while(gameState == 1)
      {
        swValue = digitalRead(pinSW);
        buttonPressed = false;
        if (swValue == 0 and buttonPressed == false)
        {
            gameState = 0;
            cleared = 0;   
            buttonNr = 1;
            buttonPressed =true;        
        }
        if(swValue == 1)
        {
          buttonPressed =false;
        }
      }
    }
      else
      {
        
        buttonMax = 4;
        for (int i=0;i<nrOptions;i++)
        {
          displayMenu(i,startPositionX,startPositionY,Start);
        }
        if(millis()-timpStart - timpUpdate>= 5000)
        {
          Level ++;
          timpUpdate = millis() ;
          timpStart = 0;
        }
        lcd.setCursor(startPositionX[1]+6,startPositionY[1]);
        lcd.print(Level);
        lcd.setCursor(startPositionX[2]+6,startPositionY[2]);
        score = 3 *Level;
        lcd.print(score);
        displayFlag(3,4,startPositionX,startPositionY);
        if(score>maxScore)
        {  
          maxScore = score;
          EEPROM.write(adr, maxScore);
          adr++;
          if(adr == EEPROM.length())
              adr = 0;
        }
      }
  }
  if(gameState == 2)
  {
    
    buttonMax = 2;
    if(cleared == 0)
    {
      lcd.clear();
      cleared = 1;
    }
    for(int i=0;i<nrOptions;i++)
    {
      displayMenu(i,scorePositionX,scorePositionY,Score);
    }
    lcd.setCursor(scorePositionX[0]+6,scorePositionY[0]);
    
    lcd.print(maxScore);
    displayFlag(1,2,scorePositionX,scorePositionY);
    
  }
  if(gameState == 3)
  {
    buttonMax = 2;
    if(cleared == 0)
    {
      lcd.clear();
      cleared = 1;
    }
    for (int i=0;i<2;i++)
    {
      displayMenu(i,settingsPositionX,settingsPositionY,Settings);
    }
    lcd.setCursor(settingsPositionX[0]+6,settingsPositionY[0]);
    lcd.print(startingLevel);
    displayFlag(1,2,settingsPositionX,settingsPositionY);
    
  }
  } 
