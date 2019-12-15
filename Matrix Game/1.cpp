#include "LedControl.h"
LedControl lc =LedControl(12,11,10,1);
int pinX = A0;
int pinY = A1;
int score = 0;
int levelNr = 0;
const int nrLevels = 4;
int updateTime = 0;
const byte Levels[nrLevels][16][16] ={
  {
  {0,0,1,1,1,1,1,1},
  {0,0,0,0,1,1,1,1},
  {1,0,0,0,0,1,1,1},
  {1,0,0,0,0,0,1,1},
  {1,1,0,0,0,0,0,1},
  {1,1,1,0,0,0,0,1},
  {1,1,1,1,0,0,0,0},
  {1,1,1,1,1,1,0,0},
  
  },
  {
    {0,0,0,1,1,1,1,1},
    {0,0,0,0,0,0,0,1},
    {0,0,0,0,0,0,0,1},
    {1,1,1,0,1,0,0,1},
    {1,1,1,1,0,0,0,1},
    {1,1,0,0,0,0,1,1},
    {1,0,0,1,0,0,0,0},
    {1,1,1,1,1,1,0,0},
    
    },

     {
    {0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,0,0,1,0,1,0,0,0,1},
    {1,0,0,0,0,0,1,0,0,1,0,1,0,0,0,1},
    {1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,1},
    {1,0,0,1,0,0,1,0,0,0,0,1,0,0,0,1},
    {1,0,0,1,0,0,1,1,1,1,1,1,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,1,1,1,1,1,1,1,1,1,0,0,0,0,1},
    {1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0}
  },

  
    {
    {0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1},
    {0,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1},
    {1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1},
    {1,1,0,1,1,1,1,1,1,1,0,1,1,1,0,1},
    {1,0,0,0,1,0,0,0,1,0,0,1,0,1,0,1},
    {1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,1},
    {1,0,1,0,1,0,1,0,0,0,0,1,1,0,1,1},
    {1,0,1,0,1,1,1,1,1,1,1,1,0,0,0,1},
    {1,0,0,0,0,0,0,1,0,1,0,0,0,1,0,1},
    {1,0,1,0,1,1,1,1,0,0,1,0,0,1,0,1},
    {1,0,1,0,1,0,0,0,0,0,1,1,1,1,0,1},
    {1,0,1,1,1,0,1,1,1,0,1,0,0,0,0,1},
    {1,0,0,0,1,0,1,0,0,0,0,1,1,1,1,1},
    {1,1,0,0,0,0,1,0,0,1,0,0,0,0,1,0},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0}
  }
};
int sfert = 0;
int Exit[nrLevels][3] = {{7,7,0},{7,7,0},{7,7,3},{7,7,3}};
int Start[nrLevels][3] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
int Points[nrLevels][2][4] = {
  {{2,2,0,0},{5,5,0,0}},
  {{2,2,0,0},{5,5,0,0}},
  {{2,2,0,0},{5,5,0,0}},
  
};

int playerX = 0;
int playerY = 0;
int xValue = 0;
int yValue = 0;
int xAxisTilted = false;
int yAxisTilted = false;
int pointsUpdate = 0;
int pointsValue = false;
void displayPoints(int level, int sfert)
{
  if(millis() - pointsUpdate > 200)
  {
    for(auto i : Points[level])
      if(sfert == i[2] and i[3] == 0)
      {  
        lc.setLed(0,i[0],i[1], pointsValue);
      }
    pointsValue = !pointsValue;
    pointsUpdate = millis();
  }
  for(auto i : Points[level])
    if(sfert == i[2] and i[3] == 1)
    {  
      lc.setLed(0,i[0],i[1], false);
    }
}
void displayLevel(int level, int sfert)
{
    if(sfert == 0)
    {
      for( int i = 0; i < 8; i ++)
        for( int j = 0; j < 8; j ++)
          lc.setLed(0,i,j,Levels[level][i][j]);
    }
    if(sfert == 1)
    {
      for( int i = 0; i< 8; i++)
        for(int j = 0; j < 8; j++)
        {
          lc.setLed(0,i,j,Levels[level][i][j+8]);        
        }
    }
    if(sfert == 2)
    {
      for( int i = 0; i< 8; i++)
        for(int j = 0; j < 8; j++)
        {
          lc.setLed(0,i,j,Levels[level][i+8][j]);        
        }
    }
    if(sfert == 3)
    {
      for( int i = 0; i< 8; i++)
        for(int j = 0; j < 8; j++)
        {
          lc.setLed(0,i,j,Levels[level][i+8][j+8]);        
        }
    }
}
void resetLevel(int level , int lose){
  int nrDots = 0;
  playerX = Start[level][0];
  playerY = Start[level][1];
  sfert = Start[level][2];
  for( int i = 0; i < 8; i++)
  {
    if(Points[level][i][3] == 1)
      nrDots ++;
    Points[level][i][3] = 0;
  }
  if(lose == 1)
    score = score - 100*nrDots;
}   
/*
void displayWin()
{
  for( int i = 0; i < 8; i++)
  {
    for( int j = 0; j < 8; j++)
    {
      lc.setLed(0,i,j,false); 
    }
  }
  for(int k =0;k < 33; k++){
    for( int i = 0; i < 8; i++)
    {
      for( int j = 0; j < 8; j++)
      {
        lc.setLed(0,i,j,winMatrix[i][j+k]);   
      }  
    }
    delay(330);
  }
  levelNr ++;
  resetLevel(levelNr,0);
}
*/
bool checkColision(int levelNr,int playerX, int playerY, int sfert){
    if(sfert == 0)
    {
      if(Levels[levelNr][playerX][playerY] == 1)
        return 1;
      return 0;
    }
    if(sfert == 1)
    {
      if(Levels[levelNr][playerX][playerY + 8] == 1)
        return 1;
      return 0;
    }
    if(sfert == 2)
    {
      if(Levels[levelNr][playerX + 8][playerY] == 1)
        return 1;
      return 0;
    }
    if(sfert == 3)
    {
      if(Levels[levelNr][playerX + 8][playerY + 8] == 1)
        return 1;
      return 0;
    }
}
bool checkWin(int levelNr, int playerX, int playerY, int sfert){
  if(playerX == Exit[levelNr][0] and playerY == Exit[levelNr][1] and sfert == Exit[levelNr][2])
    return 1;
  return 0;
}
void pointsColision(int level, int playerX, int playerY, int sfert)
{
      for(int i = 0 ; i < 2; i++){
        if(Points[level][i][2] == sfert and Points[level][i][0] == playerX and Points[level][i][1] == playerY and Points[level][i][3] == 0)
        {
          Points[level][i][3] = 1;      
          score += 100;
        }
      }
}

bool setPlayer = false;
void setup()
{
  pinMode(pinX,INPUT);
  pinMode(pinY,INPUT);
  lc.shutdown(0,false);
  lc.setIntensity(0,2);
  lc.clearDisplay(0);
  Serial.begin(9600);
  for(int i=0; i < 8 ; i++)
    for(int j = 0; j < 8; j++)
      lc.setLed(0,j,i,true);
}
void loop()
{
  if(!setPlayer)
  {
    playerX = Start[levelNr][0];
    playerY = Start[levelNr][1];
    sfert = Start[levelNr][2];
    setPlayer = true;
  }
  displayLevel(levelNr, sfert);
  displayPoints(levelNr, sfert);
  bool lastUpdate = true;
  Serial.println(score);
  if(millis() - updateTime > 1000) 
  {  
    lc.setLed(0,playerX,playerY,lastUpdate);
    updateTime = millis();
    lastUpdate = !lastUpdate;
  }
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  yValue = 1023 - yValue;
  if(xValue > 700 and xAxisTilted == false)
  {
    xAxisTilted = true;
    if(levelNr < 2)
    {
      if(playerX < 7)
        playerX ++;
    }
    else
    {
      if(playerX == 7)
      {
        if(sfert == 0 or sfert == 1)
        {
          if(sfert == 0)
            sfert = 2;
          if(sfert == 1)
            sfert = 3;
          playerX = 0;
        }
      }
      else if(playerX != 7)
      {
         playerX ++;
      }
    }
  }
  if(xValue < 300 and xAxisTilted == false)
  {
    xAxisTilted = true;
    if(levelNr < 2)
    {
      if(playerX > 0)
        playerX --;
    }
    else
    {
      if(playerX == 0)
      {
        if(sfert == 2 or sfert ==3)
        {
          playerX = 7;
          if(sfert == 2)
            sfert = 0;
          if(sfert == 3)
            sfert = 0;
        }
      }
      else if(playerX != 0)
      {
         playerX --;
      }
    }
  }
  if(xValue > 300 and xValue < 700)
    xAxisTilted = false;
    
  if(yValue > 700 and yAxisTilted == false)
  {
    yAxisTilted = true;
    if(levelNr < 2)
    {
      if(playerY < 7)
        playerY ++;
    }
    else
    {
      if(playerY == 7)
      {
        if(sfert == 0 or sfert == 2)
        {
          playerY = 0;
          if(sfert == 0)
            sfert = 1;
          if(sfert == 2)
            sfert = 3;
        }
      }
      else if(playerY != 7)
      {
         playerY ++;
      }
    }
  }
  if(yValue < 300 and yAxisTilted == false)
  {
    yAxisTilted = true;
    if(levelNr < 2)
    {
      if(playerY > 0)
        playerY --;
    }
    else
    {
      if(playerY == 0)
      {
        if(sfert == 1 or sfert == 3)
        {
          playerY = 7;
          if(sfert == 1)
            sfert = 0;
          if(sfert == 3)
            sfert = 2;
        }
      }
      else if(playerY != 0)
      {
         playerY --;
      }
    }
  }
  if(yValue > 300 and yValue < 700)
    yAxisTilted = false;
  if(checkColision(levelNr,playerX,playerY,sfert))
  {
    resetLevel(levelNr,1);
  }
  if(checkWin(levelNr,playerX,playerY,sfert))  {
    lc.clearDisplay(0);
    //displayWin();
    levelNr++;
    resetLevel(levelNr,0);
  }
  pointsColision(levelNr,playerX,playerY,sfert);
}
