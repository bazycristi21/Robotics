#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "LedControl.h"

LiquidCrystal lcd(8, 9, 5, 4, 3, 2);
LedControl lc = LedControl(12, 11, 10, 1);

const int pinSW = A2;
const int pinX = A0;
const int pinY = A1;
int adr = 0;
int val;
bool winGame = false;
int timePassed = 0;
int score = 0;
int levelNr = 0;
const int nrLevels = 4;
int updateTime = 0;
const byte Levels[nrLevels][16][16] = {
  {

    {0, 0, 1, 1, 1, 1, 1, 1},

    {0, 0, 0, 1, 1, 1, 1, 1},

    {1, 0, 0, 0, 1, 1, 1, 1},

    {1, 1, 0, 0, 0, 1, 1, 1},

    {1, 1, 1, 0, 0, 0, 1, 1},

    {1, 1, 1, 1, 0, 0, 0, 1},

    {1, 1, 1, 1, 1, 0, 0, 0},

    {1, 1, 1, 1, 1, 1, 0, 0},



  },

  {

    {0, 0, 0, 1, 1, 1, 1, 1},

    {0, 0, 0, 0, 0, 0, 0, 1},

    {1, 0, 0, 0, 0, 0, 0, 1},

    {1, 1, 1, 0, 1, 0, 0, 1},

    {1, 1, 1, 1, 0, 0, 0, 1},

    {1, 1, 0, 0, 0, 0, 1, 1},

    {1, 0, 0, 1, 0, 0, 0, 0},

    {1, 0, 1, 1, 1, 1, 0, 0},



  },
  {

    {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1},
    {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0}

  },





  {
    {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
    {0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
    {1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1},
    {1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1},
    {1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0}

  }

};
int sfert = 0;
int Exit[nrLevels][3] = {{7, 7, 0}, {7, 7, 0}, {7, 7, 3}, {7, 7, 3}};
int Start2[nrLevels][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
int Points[nrLevels][2][4] = {
  {{2, 2, 0, 0}, {5, 5, 0, 0}},
  {{2, 2, 0, 0}, {5, 5, 0, 0}},
  {{2, 2, 0, 0}, {5, 5, 0, 0}},
  {{1,1,1,0},{3,3,0,0}}
};
int playerX = 0;
int playerY = 0;
int xValue = 0;
int yValue = 0;
int xAxisTilted = false;
int yAxisTilted = false;
int pointsUpdate = 0;
int pointsValue = false;
int loseGame = false;
bool setPlayer = false;
int startTime = 0;
bool setNewLevel = false;
int gameState = 0;
const int nrOptions = 4;
char *Menu[nrOptions] = {"Maze", "Start", "Score", "Settings"};
char *Score[2] = {"Score:", "Exit"};
char *Settings[2] = {"Level:", "Exit"};
int settingsPositionX[2] = {4, 4};
int settingsPositionY[2] = {0, 1};
int scorePositionX[2] = {4, 4};
int scorePositionY[2] = {0, 1};
int startPositionX[4] = {1, 9, 1, 11};
int startPositionY[4] = {0, 0, 1, 1};
int PositionX[nrOptions] = {1, 9, 1, 8};
int PositionY[nrOptions] = {0, 0, 1, 1};
bool buttonPressed = false;
int maxScore = 0;
int buttonMax = 0;
int cleared = 0;
int buttonNr = 1;
int startingLevel = 0;
int Level = 0;
int lastUpdate = 0;
bool started = false;
int currentLevelTime = 0;
byte winMatrix[8][27] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,0,1,1,1,0,1,0,1,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,1,1,0,1,1,1,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};




void setup() {
  lc.shutdown(0, false);

  lc.setIntensity(0, 2);

  lc.clearDisplay(0);

  Serial.begin(9600);

  for (int i = 0; i < 8 ; i++)

    for (int j = 0; j < 8; j++)

      lc.setLed(0, j, i, true);
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
  pinMode(pinSW, INPUT_PULLUP);
  lcd.begin(16, 2);
  lcd.clear();
}



void displayPoints(int level, int sfert)

{

  if (millis() - pointsUpdate > 200)

  {

    for (auto i : Points[level])

      if (sfert == i[2] and i[3] == 0)

      {

        lc.setLed(0, i[0], i[1], pointsValue);

      }

    pointsValue = !pointsValue;

    pointsUpdate = millis();

  }

  for (auto i : Points[level])

    if (sfert == i[2] and i[3] == 1)

    {

      lc.setLed(0, i[0], i[1], false);

    }

}

void displayLevel(int level, int sfert)

{

  if (sfert == 0)

  {

    for ( int i = 0; i < 8; i ++)

      for ( int j = 0; j < 8; j ++)

        lc.setLed(0, i, j, Levels[level][i][j]);

  }

  if (sfert == 1)

  {

    for ( int i = 0; i < 8; i++)

      for (int j = 0; j < 8; j++)

      {

        lc.setLed(0, i, j, Levels[level][i][j + 8]);

      }

  }

  if (sfert == 2)

  {

    for ( int i = 0; i < 8; i++)

      for (int j = 0; j < 8; j++)

      {

        lc.setLed(0, i, j, Levels[level][i + 8][j]);

      }

  }

  if (sfert == 3)

  {

    for ( int i = 0; i < 8; i++)

      for (int j = 0; j < 8; j++)

      {

        lc.setLed(0, i, j, Levels[level][i + 8][j + 8]);

      }

  }

}

void resetLevel(int level , int lose) {

  int nrDots = 0;

  playerX = Start2[level][0];

  playerY = Start2[level][1];

  sfert = Start2[level][2];

  for ( int i = 0; i < 8; i++)

  {

    if (Points[level][i][3] == 1)

      nrDots ++;

    Points[level][i][3] = 0;

  }

  if (lose == 1)
  {
    if(score == 100)
    {
      lcd.setCursor(9,1);
      lcd.print(" ");
    }
    score = score - 20 * nrDots;
    if(score < 0)
      score = 0;
  }
}

  void displayWin()
  {
   
    for(int i = 0; i < 8; i++)
    {
      for(int j = 0; j < 8; j++)
      {
        lc.setLed(0,i,j,false);
      }
    }
    for(int k =0; k < 19; k++){
      for(int i = 0; i < 8; i++)
      {
        for(int j = 0; j < 8; j++)
        {
          lc.setLed(0,i,j,winMatrix[i][j+k]);
        }
      }
      delay(330);
    }
  }

bool checkColision(int levelNr, int playerX, int playerY, int sfert) {

  if (sfert == 0)
  {
    if(levelNr == 1)
    {
      if(playerX == 1 && playerY == 7)
        score += 300;
    }
    if (Levels[levelNr][playerX][playerY] == 1)

      return true;

    return false;
    
  }

  if (sfert == 1)

  {

    if (Levels[levelNr][playerX][playerY + 8] == 1)

      return true;

    return false;

  }

  if (sfert == 2)

  {

    if (Levels[levelNr][playerX + 8][playerY] == 1)

      return true;

    return false;

  }

  if (sfert == 3)

  {

    if (Levels[levelNr][playerX + 8][playerY + 8] == 1)

      return true;

    return false;

  }

}

bool checkWin(int levelNr, int playerX, int playerY, int sfert) {
  
  if (playerX == Exit[levelNr][0])
    if (playerY == Exit[levelNr][1])
      if (sfert == Exit[levelNr][2])
      {
        return true;
      }
  return false;
}

void pointsColision(int level, int playerX, int playerY, int sfert)
{
  for (int i = 0 ; i < 2; i++) {
    if (Points[level][i][2] == sfert)
      if (Points[level][i][0] == playerX)
        if (Points[level][i][1] == playerY)
          if (Points[level][i][3] == 0)
          {
            Points[level][i][3] = 1;
            score += 20;
          }
  }
}

int play()
{

  int levelTime[nrLevels] ={21000,21000,31000,31000};
  
  if( !setNewLevel)
  {
    loseGame = false;
    winGame = false;
    started = true;
    startTime = millis();
    timePassed = 0;
    currentLevelTime = levelTime[levelNr];
    setNewLevel = true;
  }
  if (!setPlayer)
  {
    levelNr = startingLevel;
    playerX = Start2[levelNr][0];
    playerY = Start2[levelNr][1];
    sfert = Start2[levelNr][2];
    setPlayer = true;
    score = 0;
  }
  displayLevel(levelNr, sfert);
  displayPoints(levelNr, sfert);

  lc.setLed(0,playerX,playerY,true);
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  yValue = 1023 - yValue;
  if (xValue > 900 and xAxisTilted == false)
  {
    xAxisTilted = true;
    if (levelNr < 2)
    {
      if (playerX < 7)
        playerX ++;
    }
    else
    {
      if (playerX == 7)
      {
        if (sfert == 0 or sfert == 1)
        {
          if (sfert == 0)
            sfert = 2;
          if (sfert == 1)
            sfert = 3;
          playerX = 0;
        }
      }
      else if (playerX != 7)
      {
        playerX ++;
      }
    }
  }
  if (xValue < 100 and xAxisTilted == false)
  {
    xAxisTilted = true;
    if (levelNr < 2)
    {
      if (playerX > 0)
        playerX --;
    }
    else
    {
      if (playerX == 0)
      {
        if (sfert == 2 or sfert == 3)
        {
          playerX = 7;
          if (sfert == 2)
            sfert = 0;
          if (sfert == 3)
            sfert = 0;
        }
      }
      else if (playerX != 0)
      {
        playerX --;
      }
    }
  }
  if (xValue > 100 and xValue < 900)
    xAxisTilted = false;
  if (yValue > 900 and yAxisTilted == false)
  {
    yAxisTilted = true;
    if (levelNr < 2)
    {
      if (playerY < 7)
        playerY ++;
    }
    else
    {
      if (playerY == 7)
      {
        if (sfert == 0 or sfert == 2)
        {
          playerY = 0;
          if (sfert == 0)
            sfert = 1;
          if (sfert == 2)
            sfert = 3;
        }
      }
      else if (playerY != 7)
      {
        playerY ++;
      }
    }
  }
  if (yValue < 100 and yAxisTilted == false)
  {
    yAxisTilted = true;
    if (levelNr < 2)
    {
      if (playerY > 0)
        playerY --;
    }
    else
    {
      if (playerY == 0)
      {
        if (sfert == 1 or sfert == 3)
        {
          playerY = 7;
          if (sfert == 1)
            sfert = 0;
          if (sfert == 3)
            sfert = 2;
        }
      }
      else if (playerY != 0)
     { 
       playerY --;
      }
    }
  }
  if (yValue > 100 and yValue < 900)
    yAxisTilted = false;
  if (checkColision(levelNr, playerX, playerY, sfert))
  {
    resetLevel(levelNr, 1);
  }
  timePassed = millis() - startTime;
  pointsColision(levelNr, playerX, playerY, sfert);
  lcd.setCursor(14,0);
  if(levelTime[levelNr] - timePassed < 0)
  {
    loseGame = true;
    timePassed = 0;
   
    levelNr = 0;
    setPlayer = false;
    setNewLevel = false;
    started = false;
    for(int i = 0; i < nrLevels; i ++)
      resetLevel(i,0);
    return false;
    
    
  }
  lcd.print((levelTime[levelNr] - timePassed)/1000);  
  if((levelTime[levelNr] - timePassed)/1000 < 10)
  {
    
    lcd.setCursor(15,0);
    lcd.print(" ");
  }
 
  if (checkWin(levelNr, playerX, playerY, sfert))  {
    if(levelNr == nrLevels - 1){
      displayWin();
      winGame = true;
      started = false;
      setPlayer = false;
      setNewLevel = false;
      
      return;
     }
    else{
      setNewLevel = false;
      levelNr++;
      resetLevel(levelNr, 0);
    }
  }
}


void displayMenu(int pos, int positionX[], int positionY[], char *menu[])
{

  lcd.setCursor(positionX[pos], positionY[pos]);
  lcd.print(menu[pos]);

}
int timpStart;
byte value;
void displayFlag(int pos, int dimension, int positionX[], int positionY[])
{
  for (int i = 0; i < dimension; i++)
  {
    lcd.setCursor(positionX[i] - 1, positionY[i]);
    lcd.print(" ");
  }
  if (buttonNr != 0)
  {
    lcd.setCursor(positionX[pos] - 1, positionY[pos]);
    lcd.print(">");
  }
}
int timpUpdate = 0;
int timpUpdate2 = 0;
int firstLevel;

void loop() {

  if(started == false)
  {
    for(int i = 0; i < 7; i++)
      for(int j = 0; j < 7; j++)
        lc.setLed(1,i,j,true);
  }
  maxScore = EEPROM.read(adr);
  int yValue = analogRead(pinX);
  int xValue = analogRead(pinY);
  xValue = 1023 - xValue;


  int swValue = digitalRead(pinSW);
  
  if (swValue == 0 and buttonPressed == false)
  {
    if ((gameState == 1 and buttonNr == 3) or (gameState == 2 and buttonNr == 1) or (gameState == 3 and buttonNr == 1))
    {
      buttonPressed = true;
      gameState = 0;
      cleared = 0;
      buttonNr = 1;
    }
    else {
      gameState = buttonNr;
      buttonPressed = true;
      cleared = 0;
      buttonNr = 1;
    }

  }
  if (swValue == 1)
  {
    buttonPressed = false;
  }
  if (xValue > 700 && buttonNr < buttonMax - 1  && xAxisTilted == false)
  {
    buttonNr ++;
    xAxisTilted = true;
  }
  if (xValue < 300 && buttonNr > 1 && xAxisTilted == false)
  {
    buttonNr --;
    xAxisTilted = true;
  }
  if (xValue > 300 && xValue < 700)
  {
    xAxisTilted = false;
  }
  if (yValue > 700  && yAxisTilted == false)
  {

    if (gameState == 3)
    {
      if(startingLevel < 3)
        startingLevel++;
    }
    yAxisTilted = true;
  }
  if (yValue < 300 && yAxisTilted == false)
  {
    if (gameState == 3 && startingLevel > 0)
    {
      startingLevel--;
    }
    yAxisTilted = true;
  }
  if (yValue > 300 && yValue < 700)
  {
    yAxisTilted = false;
  }
  if (gameState == 0)
  {
    buttonMax = 4;
    if (cleared == 0)
    {
      lcd.clear();
      cleared = 1;
    }
    for (int i = 0; i < nrOptions; i++)
    {
      Serial.println(i);
      displayMenu(i, PositionX, PositionY, Menu);
    }
    lcd.setCursor(8,1);
    lcd.print("Settings");
    displayFlag(buttonNr, nrOptions, PositionX, PositionY);
  }
 
  if (gameState == 1)
  {
    started = true;
    
    if (cleared == 0)
    {
      Level = startingLevel;
      lcd.clear();
      cleared = 1;
      timpStart = millis();
      timpUpdate2 = 0;
      firstLevel = Level;
     }
    while(started == true)
    {
      play();
      lcd.setCursor(0,0);
      lcd.print("Level:");
      lcd.setCursor(7,0);
      lcd.print(levelNr);
      lcd.setCursor(0,1);
      lcd.print("Score:");
      lcd.setCursor(7,1);
      lcd.print(score);
    }
    if (score > maxScore)
      {
        maxScore = score;
        EEPROM.write(adr, maxScore);
      }
    if (winGame == true)
    {
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Congratulations");
      lcd.setCursor(0, 1);
      lcd.print("you finished the");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("you finished the");
      lcd.setCursor(0, 1);
      lcd.print("game.Press the");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("game.Press the");
      lcd.setCursor(0, 1);
      lcd.print("button to exit");
      while (gameState == 1)
      {
        swValue = digitalRead(pinSW);
        buttonPressed = false;
        if (swValue == 0 and buttonPressed == false)
        {
          gameState = 0;
          cleared = 0;
          buttonNr = 1;
          buttonPressed = true;
        }
        if (swValue == 1)
        {
          buttonPressed = false;
        }
      }
    }
    if(loseGame == true)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Time is up");
      lcd.setCursor(0, 1);
      lcd.print("Your score:");
      lcd.setCursor(12,1);
      lcd.print(score);
      delay(3000);
      lcd.clear();
      gameState = 0;
      cleared = 0;
      buttonNr = 1;
    }
  }
  if (gameState == 2)
  {
    buttonMax = 2;
    if (cleared == 0)
    {
      lcd.clear();
      cleared = 1;
    }
    for (int i = 0; i < nrOptions; i++)
    {
      displayMenu(i, scorePositionX, scorePositionY, Score);
    }
    lcd.setCursor(scorePositionX[0] + 6, scorePositionY[0]);

    lcd.print(EEPROM.read(adr));
    displayFlag(1, 2, scorePositionX, scorePositionY);

  }
  if (gameState == 3)
  {
    buttonMax = 2;
    if (cleared == 0)
    {
      lcd.clear();
      cleared = 1;
    }
    for (int i = 0; i < 2; i++)
    {
      displayMenu(i, settingsPositionX, settingsPositionY, Settings);
    }
    lcd.setCursor(settingsPositionX[0] + 6, settingsPositionY[0]);
    lcd.print(startingLevel);
    displayFlag(1, 2, settingsPositionX, settingsPositionY);

  }
}
