#include <Wire.h>
#include <LiquidCrystal_I2C.h>


#define one 2
#define two 3
#define three 4
#define four 5


struct PredJmena{
  String predjmeno;
  int zivotyNavic;
  int poskozeniNavic;
};
struct Nepratele {
  String jmeno;
  int zivoty;
  int poskozeni;
};

PredJmena dataPredJmena[] = {
  {"Huge",3,0},
  {"Small",-1,0},
  {"Violent",2,2},  
  {"Rude",0,3},
  {"Stupid",-1,-1},
  {"Angry",0,2},
  {"Smart",-2,3},
  {"Hungry",0,2}

};

Nepratele dataNepratelePlain[] = {
  {"Slime", 3,1},
  {"Skeleton", 2,2},
  {"Wolf", 3,3},
  {"Zombie",4,1},
  {"Bear",10,2},
  {"Bandit",3,5}
};

Nepratele dataNeprateleDarkForest[] = {
  {"Knight", 10,4},
  {"Archer", 7,6},
  {"Goblin", 5,5},
  {"Orge", 15,3}
};

const int dataPredJmenaSize = sizeof(dataPredJmena) / sizeof(dataPredJmena[0]);
const int dataNepratelePlainSize = sizeof(dataNepratelePlain) / sizeof(dataNepratelePlain[0]);
const int dataNeprateleDarkForestSize = sizeof(dataNeprateleDarkForest) / sizeof(dataNeprateleDarkForest[0]);


// STATS
  int maxHealth = 5;
  int health = 5;
  int level = 1;
  int expirience = 1;
  int attack = 2;
  int coins = 1;
  int potions = 0;

int enemyHealth;
int enemyDamage;
int enemyExpirence;

  enum LoactionScreen{
    home,
    exporing,
    fighting,
    village,
    locations,
    work
  };
  enum Loaction{
    Plain,
    DarkForest,
    Desert
  };

// CHARACTERS
  byte heartChar[] = {
    B00000,
    B01010,
    B10101,
    B10001,
    B10001,
    B01010,
    B00100,
    B00000
  };
  byte arrowChar[] = {
    B00100,
    B01110,
    B11111,
    B01110,
    B01110,
    B01110,
    B01110,
    B00000
  };
  byte swordChar[] = {
    B00000,
    B00000,
    B00011,
    B00111,
    B01110,
    B01100,
    B10000,
    B00000
  };
  byte checkmarkChar[] = {
    B00000,
    B00000,
    B00000,
    B00001,
    B00011,
    B10110,
    B11100,
    B01000
  };
  byte crossChar[] = {
    B00000,
    B10001,
    B11011,
    B01110,
    B00100,
    B01110,
    B11011,
    B10001
  };
  byte arrowsChar[] = {
    B00100,
    B01010,
    B10001,
    B00000,
    B00000,
    B10001,
    B01010,
    B00100
  };
  byte coinChar[] = {
    B00100,
    B01010,
    B10001,
    B10001,
    B10001,
    B01010,
    B00100,
    B00000
  };
  byte lockChar[] = {
    B00000,
    B00000,
    B00100,
    B01010,
    B01010,
    B11111,
    B11111,
    B11111
  };
  byte potionChar[] = {
    B11111,
    B11111,
    B01010,
    B01010,
    B10001,
    B10001,
    B10001,
    B11111
  };


int menuSelection = 1;
int villageSelection = 1;
int locationSelection = 1;
int poolSelected = 1;

float time;
LoactionScreen location;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() 
{
  randomSeed(analogRead(A0));
  Serial.begin(9600);
  // Inputs creation
  pinMode(one, INPUT_PULLUP);
  pinMode(two, INPUT_PULLUP);
  pinMode(three, INPUT_PULLUP);
  pinMode(four, INPUT_PULLUP);
  
  lcd.init();
  lcd.backlight();

  // Symbols creation
  lcd.createChar(0,heartChar);
  lcd.createChar(1,arrowChar);
  lcd.createChar(2,swordChar);
  lcd.createChar(3,checkmarkChar);
  lcd.createChar(4,crossChar);
  lcd.createChar(5,arrowsChar);
  lcd.createChar(6,coinChar);
  lcd.createChar(7,potionChar);

  location = home;
  StartScreen();
  MainScreen();
}

void StartScreen()
{
  lcd.setCursor(2, 0);
  lcd.print("Arduino RPG");
  lcd.setCursor(3, 1);
  lcd.print("By Francek");
  delay(2000);
  return;
}

void loop() 
{

  
  // HOME ONE INPUT
  if (digitalRead(one) == 0 && location == home) 
  {
    switch (menuSelection) {
      case 1:
        FindEnemy();
        break;
      case 2:
        VillageScreen();
        break;
      case 3:
        LocationScreen();
        break;
      case 4:
        WorkScreen();
        break;

    }
  }
  // AWAY ONE INPUT
  if(digitalRead(one) == 0 && location != home)
  {
    switch (location) {
      case village:
        if(coins > 0)
        {
          SleepingScreen();
          coins--;
          health = maxHealth;
          MainScreen();
        }else {
        NoCoinsScreen();
        }
        break;
      case exporing:
        FightScene();
        break;
      case locations:
        switch(locationSelection){
        case 1:
          poolSelected = 1;
          Serial.println(poolSelected);
          TravelScreen("Plains");
          break;
        case 2:
          if(level >= 5){
          poolSelected = 2;
          Serial.println(poolSelected);
          TravelScreen("Dark Forest");
          }
          else{
          CantGoScreen();
          }
          break;
        case 3:
          if(level >= 10){
          poolSelected = 3;
          TravelScreen("Desert");
          }
          else{
          CantGoScreen();
          }
          break;
        case 4:
          if(level >= 20){
          poolSelected = 4;
          TravelScreen("Volcano");
          }
          else{
          CantGoScreen();
          }
          break;
        }
        break;
      case work:
        WorkingScreen();
        break;
    }
  }
  // TWO INPUT
  if(digitalRead(two) == 0)
  {
    MainScreen();
  }
  
  
  if(digitalRead(three) == 0)
  {
    switch(location){
      case home:
        if(menuSelection < 4)
        {
          menuSelection++;
          MainScreen();
        }
        break;
      case village:
        if(villageSelection < 4)
        {
          villageSelection++;
          VillageScreen();
        }
        break;
      case locations:
        if(locationSelection < 4)
        {
          locationSelection++;
          LocationScreen();
        }
        break;
        

    }
  }

  if(digitalRead(four) == 0)
  {
    switch(location){
      case home:
        if(menuSelection > 1)
        {
          menuSelection--;
          MainScreen();
        }
        break;
      case village:
        if(villageSelection > 1)
        {
          villageSelection--;
          VillageScreen();
        }
        break;
      case locations:
        if(locationSelection > 1)
        {
          locationSelection--;
          LocationScreen();
        }
        break;
    }
  }
  


  /*
  Serial.println(digitalRead(one));
  Serial.println(digitalRead(two));
  Serial.println(digitalRead(three));
  Serial.println(digitalRead(four));
  */
  
  

  delay(100);
}

void MainScreen()
{
  
  location = home;
  lcd.clear();
  // HEART / HEALTH
  lcd.setCursor(0, 0);
  lcd.write(0);
  lcd.setCursor(1, 0);
  lcd.print(health);
  // ARROW / LEVEL
  lcd.setCursor(4, 0);
  lcd.write(1);
  lcd.setCursor(5, 0);
  lcd.print(level);
  // SWORD / ATTACK
  lcd.setCursor(7, 0);
  lcd.write(2);
  lcd.setCursor(8, 0);
  lcd.print(attack);
  // COIN / COINS
  lcd.setCursor(10, 0);
  lcd.write(6);
  lcd.setCursor(11, 0);
  lcd.print(coins);

  lcd.setCursor(13, 0);
  lcd.write(7);
  lcd.setCursor(14, 0);
  lcd.print(potions);


  // Lower panel emojis
  lcd.setCursor(0, 1);
  lcd.write(3);
  lcd.setCursor(15, 1);
  lcd.write(5);
  lcd.setCursor(1, 1);
  // Selection of activities
  switch(menuSelection)
  {
      case 1:{
        lcd.print("Explore");
        break;
      }
      case 2:{
        lcd.print("Village");
        break;
      }
      case 3:{
        lcd.print("Locations");
        break;
      }
      case 4:{
        lcd.print("Work");
        break;
      }
  }

  delay(200);
  
}

void FindEnemy()
{
  location = exporing;
  lcd.clear();
  int enemyIndex = 0;
  int atributeIndex = 0;
  Nepratele* selectedPool;
  int selectedPollSize;
  switch(poolSelected)
  {
    case 1:
      selectedPool = dataNepratelePlain;
      selectedPollSize = dataNepratelePlainSize;
      break;
    case 2:
      selectedPool = dataNeprateleDarkForest;
      selectedPollSize = dataNeprateleDarkForestSize;
      break;
  }
  
  enemyIndex = random(0,selectedPollSize);
  atributeIndex = random(0,dataPredJmenaSize);

  // ZADANI ATRIBUTU
   enemyHealth = selectedPool[enemyIndex].zivoty + dataPredJmena[atributeIndex].zivotyNavic;
   enemyDamage = selectedPool[enemyIndex].poskozeni + dataPredJmena[atributeIndex].poskozeniNavic;
   enemyExpirence = enemyDamage + enemyHealth;
  // VYKRESLENI JMEN
    lcd.setCursor(0, 0);
    lcd.print(dataPredJmena[atributeIndex].predjmeno);
    lcd.setCursor(0, 1);
    lcd.print(selectedPool[enemyIndex].jmeno);
  // VYKRESLOVANI ATRIBUTU
    lcd.setCursor(7, 0);
    lcd.write(0);
    lcd.setCursor(8, 0);
    lcd.print(enemyHealth);
    lcd.setCursor(7, 1);
    lcd.write(2);
    lcd.setCursor(8, 1);
    lcd.print(enemyDamage);
  // FIGHT FLEE
    lcd.setCursor(10,0);
    lcd.write(3);
    lcd.setCursor(11,0);
    lcd.print("Fight");

    lcd.setCursor(10,1);
    lcd.write(4);
    lcd.setCursor(11,1);
    lcd.print("Flee");
  
  delay(1000);


}



void FightScene()
{

  location = fighting;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Foe");

  lcd.setCursor(0, 1);
  lcd.write(0);
  lcd.setCursor(1, 1);
  lcd.print(enemyHealth);
  lcd.setCursor(3, 1);
  lcd.write(2);
  lcd.setCursor(4, 1);
  lcd.print(enemyDamage);

  lcd.setCursor(13,0);
  lcd.print("You");

  lcd.setCursor(10, 1);
  lcd.write(0);
  lcd.setCursor(11, 1);
  lcd.print(health);
  lcd.setCursor(13, 1);
  lcd.write(2);
  lcd.setCursor(14, 1);
  lcd.print(attack);

  delay(1000);
  
  health -= enemyDamage;
  enemyHealth -= attack;

  if (enemyHealth > 0 && health > 0)
    {
    FightScene();
    }
  else
  {
    if (enemyHealth <= 0)
    {
      health = maxHealth;
      WinScreen(); 
    }else {
    
      health = 1;
      LooseScreen();
      MainScreen();
    }
  }
  

}

void WinScreen()
{
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("You won!");
  lcd.setCursor(3,1);
  lcd.print("Earned");
  lcd.setCursor(10,1);
  lcd.print(enemyExpirence);
  lcd.setCursor(12,1);
  lcd.print("xp");

  expirience += enemyExpirence;
  
  

  delay(3000);

  if(expirience >= 10)
  {
    LevelUp();
  }

  MainScreen();
}

void LooseScreen()
{
  lcd.clear();

  lcd.setCursor(2,0);
  lcd.print("You got hurt");
  lcd.setCursor(2,1);
  lcd.print("you have 1");
  lcd.setCursor(13,1);
  lcd.write(0);

  delay(5000);

  return;

}

void LevelUp()
{

  expirience = expirience - 10;
  level++;
  maxHealth++;
  attack++;

  health = maxHealth;
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("Level up!");
  lcd.setCursor(1,1);
  lcd.print("Stats icreased");
  delay(3000);
}



void VillageScreen()
{
  lcd.clear();
  location = village;
  
  lcd.setCursor(0,0);
  lcd.print("Village");

  // Lower panel emojis
  lcd.setCursor(0, 1);
  lcd.write(3);
  lcd.setCursor(15, 1);
  lcd.write(5);
  lcd.setCursor(1, 1);

  switch(villageSelection){
    case 1:
      lcd.print("Sleep");
      lcd.setCursor(12, 1);
      lcd.write(6);
      lcd.setCursor(13, 1);
      lcd.print("1");
      break;
    case 2:
      lcd.print("Play cards");
      lcd.setCursor(12, 1);
      lcd.write(6);
      lcd.setCursor(13, 1);
      lcd.print("?");
      break;
    case 3:
      lcd.print("Get quest");
      lcd.setCursor(12, 1);
      lcd.write(6);
      lcd.setCursor(13, 1);
      lcd.print("?");
      break;
    case 4:
      lcd.print("Rob a house");
      lcd.setCursor(12, 1);
      lcd.write(6);
      lcd.setCursor(13, 1);
      lcd.print("?");
      break;
  }
  
  

  delay(300);
}

void SleepingScreen()
{
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Sleeeping");
  lcd.setCursor(3, 1);
  lcd.print("ZZ");
  for (int i = 5; i < 12; i++) {
    lcd.setCursor(i, 1);
    lcd.print("z");
    delay(500);
  }
  return;
}

void LocationScreen()
{
  lcd.clear();
  location = locations;
  
  lcd.setCursor(0,0);
  lcd.print("Locations");
  lcd.setCursor(15, 1);
  lcd.write(5);

  lcd.setCursor(1, 1);
  switch(locationSelection){
    case 1:
      lcd.print("Plains");
      lcd.setCursor(12, 1);
      lcd.print("1");
      lcd.setCursor(13, 1);
      lcd.write(1);
      lcd.setCursor(0, 1);
      if(level >= 0){
        lcd.write(3);
      } else{
        lcd.write(4);
      }
      break;
    case 2:
      lcd.print("Dark Forest");
      lcd.setCursor(12, 1);
      lcd.print("5");
      lcd.setCursor(13, 1);
      lcd.write(1);
      lcd.setCursor(0, 1);
      if(level >= 5){
        lcd.write(3);
      } else{
        lcd.write(4);
      }
      break;
    case 3:
      lcd.print("Desert");
      lcd.setCursor(11, 1);
      lcd.print("10");
      lcd.setCursor(13, 1);
      lcd.write(1);
      lcd.setCursor(0, 1);
      if(level >= 10){
        lcd.write(3);
      } else{
        lcd.write(4);
      }
      break;
    case 4:
      lcd.print("Volcano");
      lcd.setCursor(11, 1);
      lcd.print("20");
      lcd.setCursor(13, 1);
      lcd.write(1);
      lcd.setCursor(0, 1);
      if(level >= 20){
        lcd.write(3);
      } else{
        lcd.write(4);
      }
      break;
  }




  delay(200);
}

void WorkScreen()
{
  lcd.clear();
  location = work;
  
  lcd.setCursor(0,0);
  lcd.print("Work");

  // Lower panel emojis
  lcd.setCursor(0, 1);
  lcd.write(3);
  lcd.setCursor(15, 1);
  lcd.write(5);
  lcd.setCursor(1, 1);
  lcd.print("Mine");

  delay(500);
}

void WorkingScreen()
{
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Mining");
  lcd.setCursor(0, 1);
  for (int i = 0; i < 16;) {
    if(digitalRead(one) == 0)
    {
      lcd.setCursor(i, 1);
      lcd.print("-");
      i++;
    }
    delay(200);
  }
  
  coins++;
  lcd.clear();
  
  lcd.setCursor(3, 0);
  lcd.print("You earned");
  lcd.setCursor(6, 1);
  lcd.print("1");
  lcd.setCursor(7, 1);
  lcd.write(6);

  delay(2000);
  MainScreen();
}

void NoCoinsScreen()
{
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Not enough");
  lcd.setCursor(5,1);
  lcd.print("coins!");
  
  delay(2000);
  MainScreen();
}

void CantGoScreen()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("You are too weak");
  lcd.setCursor(1,1);
  lcd.print("get some exp");
  
  delay(2000);
  MainScreen();
}

void TravelScreen(String locationName)
{
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("You are in");
  lcd.setCursor(3,1);
  lcd.print(locationName);
  
  delay(2000);
  MainScreen();
}
