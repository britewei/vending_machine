#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);
Servo servos[6];

int productamount = 6;
int IR[3][4] = {0};
int but[3][2] = {0};
int money = 0;
int productselect = 0;
int showquantity = 0;

struct Product
{
  char name[16];
  int price;
};

Product products[] =
{
  {"Product 1 ", 10},
  {"Product 2 ", 20},
  {"Product 3 ", 30},
  {"Product 4 ", 40},
  {"Product 5 ", 50},
  {"Product 6 ", 60}
};

void setup()
{
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  servos[0].attach(9);
  servos[1].attach(10);
  servos[2].attach(11);
  servos[3].attach(12);
  servos[4].attach(13);
  servos[5].attach(14);
  
  IR[0][0]=1;
  IR[0][1]=5;
  IR[0][2]=10;
  IR[0][3]=50;
}

void loop()
{
  insertmoney();
  productselecting();
  showproducts();
  productselected();
}

void insertmoney()  
{
  for (int a = 0; a <= 3; a++)
  {
    IR[1][a] = digitalRead(a + 2);
    if (IR[1][a] == 1 and IR[2][a] == 0)
    {
      money+=IR[0][a];
    }
    if (IR[1][a] != IR[2][a])
    {
      delay(10);
    }
    IR[2][a] = IR[1][a];
  }
  
  lcd.setCursor(15,0);
  lcd.print("$:");
  lcd.print(money);
  lcd.print(" ");
}

void showproducts()   
{
  String prshown;
  
  if(productselect<=3)   
  {
    for (int b=0; b<=3; b++)
    {
      if(b!=productselect)
      {
        prshown = String("")+products[b].name+products[b].price+String(" ");
        lcd.setCursor(0, b); 
        lcd.print(prshown);
      } 
    }
    prshown = String("-")+products[productselect].name+products[productselect].price;
    lcd.setCursor(0,productselect);
    lcd.print(prshown);
  }
  else
  {
    showquantity=productselect-3;
    for (int b=0; b<=2; b++) 
    {
      prshown = String("")+products[showquantity].name+products[showquantity].price+String(" ");
      lcd.setCursor(0, b);
      lcd.print(prshown);
      showquantity++;
    }
    prshown = String("-")+products[productselect].name+products[productselect].price;
    lcd.setCursor(0,3);
    lcd.print(prshown);
  }
}

void productselecting()
{
  but[1][0]=digitalRead(6);
  if(but[1][0]==1 and but[2][0]==0)
  {
    but[0][0]++;
  }
  if(but[1][0] != but[2][0])
  {
    delay(20);
  }
  but[2][0]=but[1][0];
  productselect=but[0][0] % productamount;
  delay(1);

  but[1][1]=digitalRead(7);
  if(but[1][1]==1 and but[2][1]==0)
  {
    but[0][1]++;
  }
  if(but[1][1] != but[2][1])
  {
    delay(20);
  }
  but[2][1]=but[1][1];
  delay(1);
}

void productselected()
{
  if(but[0][1]==1 and money>=products[productselect].price )
  {
    lcd.setCursor(14,1);
    lcd.print("   ");
    lcd.setCursor(14,2);
    lcd.print("export");
    lcd.setCursor(14,3);
    lcd.print("     ");
    money-=products[productselect].price;
    servos[productselect].write(45);
    delay(1800);
    servos[productselect].write(90);
    but[0][1]=0;
    lcd.setCursor(14,2);
    lcd.print("  Hi  ");
  }
  else if(but[0][1]==1 and money<products[productselect].price )
  {
    but[0][1]=0;
    lcd.setCursor(14,1);
    lcd.print("not");
    lcd.setCursor(14,2);
    lcd.print("enough");
    lcd.setCursor(14,3);
    lcd.print("money");
  }
}
