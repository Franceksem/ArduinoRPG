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
  {"Rude",0,3}
};

Nepratele dataNepratele[] = {
  {"Slime", 3,1},
  {"Skeleton", 2,2},
  {"Wolf", 3,3}
};

const int dataPredJmenaSize = sizeof(dataPredJmena) / sizeof(dataPredJmena[0]);
const int dataNeprateleSize = sizeof(dataNepratele) / sizeof(dataNepratele[0]);


// STATS
  int maxHealth = 10;
  int health = 10;
  int level = 1;
  int expirience = 1;
  int attack = 2;
  int coins = 0;

int enemyHealth;
int enemyDamage;
int enemyExpirence;

  enum Loaction{
    home,
    exporing,
    fighting
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


int menuSelection;
float time;
Loaction location;
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

  location = home;
  MainScreen();

}

void loop() 
{

  
  // HOME ONE INPUT
  if (digitalRead(one) == 0 && location == home) 
  {
    FindEnemy();
  }
  // EXPLORING ONE INPUT
  if(digitalRead(one) == 0 && location == exporing)
  {
    FightScene();
  }
  // EXPLORING TWO INPUT
  if(digitalRead(two) == 0 && location == exporing)
  {
    MainScreen();
  }
  
  if(digitalRead(three) == 0 && location == home)
  {
    menuSelection++;
    MainScreen();
  }

  if(digitalRead(four) == 0 && location == home)
  {
    menuSelection--;
    MainScreen();
  }
  



  Serial.println(digitalRead(one));
  Serial.println(digitalRead(two));
  Serial.println(digitalRead(three));
  Serial.println(digitalRead(four));

  
  

  delay(100);
}

void FindEnemy()
{
  location = exporing;
  lcd.clear();
  int enemyIndex = 0;
  int atributeIndex = 0;

  enemyIndex = random(0,3);
  atributeIndex = random(0,3);

  // ZADANI ATRIBUTU
   enemyHealth = dataNepratele[enemyIndex].zivoty + dataPredJmena[atributeIndex].zivotyNavic;
   enemyDamage = dataNepratele[enemyIndex].poskozeni + dataPredJmena[atributeIndex].poskozeniNavic;
   enemyExpirence = enemyDamage + enemyHealth;
  // VYKRESLENI JMEN
    lcd.setCursor(0, 0);
    lcd.print(dataPredJmena[atributeIndex].predjmeno);
    lcd.setCursor(0, 1);
    lcd.print(dataNepratele[enemyIndex].jmeno);
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
  lcd.setCursor(2, 1);
  lcd.write(2);
  lcd.setCursor(3, 1);
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

  if (enemyHealth >= 0 && health >= 0)
    {
    FightScene();
    }
  else
  {
    if (health <= 0)
    {
      health = 1;
      MainScreen();
    }else {
    
    health = maxHealth;
    WinScreen();
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
        lcd.print("Barn");
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
