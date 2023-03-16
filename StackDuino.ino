/*
 * Stack-Duino Korean version by "Argasi" (Ares-Gabriel-Asier) 2ºSMR 
 * Based in Stack mobile game
 */
#include <LiquidCrystal.h>
// LCD class definition
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);
// Box charset (1 = void, 2 = full)
byte boxVoid[] = {
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000};

byte boxFull[] = {
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111};
// Game params and constrains
int line = 16;
int boxAnimationId = 0;
int height = 15;
int remain = 2;
int last = 0;
int stat = 0;
int delayTime = 500; // Default delay time
int points = 2500;   // Default points
void setup()
{
  // Lcd definition
  lcd.begin(16, 2);
  lcd.clear();
  // Pin definition
  pinMode(7, INPUT);
  pinMode(6, OUTPUT);
  // Char definition
  lcd.createChar(1, boxVoid);
  lcd.createChar(2, boxFull);
  // Startup credits
  lcd.setCursor(0, 0);
  lcd.print("Stack-duino K.V");
  lcd.setCursor(0, 1);
  lcd.print("Hecho por argasi");
  // Startup waitoff
  delay(3000);
  lcd.clear();
}
// Gameloop
void loop()
{
  if (remain > 0) // If player can put box (2x1 or 1x1)
  {
    renderLine();
    // Update tower status (button pressed)
    if (checkButton())
    {
      tone(6, 350, 100);
      if (height > 0)
        height--;
      else if(remain != 0)
        remain = -1;
      delayTime -= 32;
      if (remain != -1)
        switch (stat)
        {
        case 0:
          remain = 0;
          break;
        case 1:
          if (last == 0 || last == 1)
          {
            last = 1;
            remain = 1;
          }
          else
            remain = 0;
          break;
        case 3:
          if (last == 0 || last == 3)
          {
            last = 3;
            remain = 1;
          }
          else
            remain = 0;
          break;
        }
        stat = rand() % 3;
        if(remain > 0)
          delay(125);
    }
          
    stat++;
    // Update or reset box status
    if (remain == 1 && stat == 2)
      stat++;
    if (stat > 3)
      stat = 0;
    if (stat != 1)
      delay(delayTime);
    else
      delay(delayTime + 250);
    // Update points
    if (points > 100)
      points -= 7;
  }
  else if (remain == 0)
  {
    // GG (Game lost)
    lcd.clear();
    loseScreen();
    // Sound
    tone(6, 200, 1000);
    delay(500);
    noTone(6);
    tone(6, 10, 750);
    // Wait and game reset
    delay(5000);
    reset();
  }
  else
  {
    // Win
     lcd.clear();
    // Point calculation
    if (remain == 1)
      points = points / 2;
    // Show win screen
    winScreen();
    // Sound
    tone(6, 800, 750);
    delay(500);
    noTone(6);
    tone(6, 900, 750);
    delay(600);
    noTone(6);
    tone(6, 1200, 1100);
    delay(1000);
    noTone(6);
    // Wait and game reset
    delay(5000);
    reset();
  }
}

// Reset game vars and reload it
void reset()
{
  height = 15;
  stat = 0;
  last = 0;
  remain = 2;
  delayTime = 500;
  lcd.clear();
}

// Render current boxline of tower
void renderLine()
{
  switch (stat)
  {
  case 0:
    lcd.setCursor(height, 0);
    lcd.write(1);
    lcd.setCursor(height, 1);
    lcd.write(1);
    break;
  case 1:
    lcd.setCursor(height, 0);
    lcd.write(1);
    lcd.setCursor(height, 1);
    lcd.write(2);
    break;
  case 2:
    lcd.setCursor(height, 0);
    lcd.write(2);
    lcd.setCursor(height, 1);
    lcd.write(2);
    break;
  case 3:
    lcd.setCursor(height, 0);
    lcd.write(2);
    lcd.setCursor(height, 1);
    lcd.write(1);
    break;
  }
}

// Returns button press value (T/F)
bool checkButton()
{
  return digitalRead(7);
}

// Render lose screen
void loseScreen()
{
  lcd.setCursor(0, 0);
  lcd.print("Fin de juego!");
  lcd.setCursor(0, 1);
  lcd.print("Altura: " + String(14 - height) + "m");
}

// Render win screen
void winScreen()
{
  lcd.setCursor(0, 0);
  lcd.print("Felicidades!");
  lcd.setCursor(0, 1);
  lcd.print("Puntos: " + String(points));
}
