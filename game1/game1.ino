
#include <UTFT.h>
#include <URTouch.h>
#include <EEPROM.h>
//подключение монитора
UTFT myGLCD(TFT01_24SP, 5, 4, 8, 7, 6); //?????
URTouch  myTouch( 6, 5, 4, 3, 2);
//подключение шрифтов из библиотеки
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];
//переменные для игры
int x, y;
int xP = 319;
int yP = 100;
int yB = 50;
int movingRate = 3;
int fallRateInt = 0;
float fallRate = 0;
int score = 0;
int lastSpeedUpScore = 0;
int highestScore;
boolean screenPressed = false;
boolean gameStarted = false;

void setup() {
  // инициализация дисплея
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);
  drawHomeScreen();  
  currentPage = '0'; // индикатор того, что мы находимся на начальной странице
 
}

void loop() {
  // проверяем касание кнопки
  if (currentPage == '0') {
    if (myTouch.dataAvailable()) {
      myTouch.read();
      x=myTouch.getX(); 
      y=myTouch.getY(); 
      if ((x>=35) && (x<=285) && (y>=140) && (y<=180)) {
        drawFrame(35, 140, 285, 180);
        currentPage = '1';
        myGLCD.clrScr();
      }
    }
  }
  if (currentPage == '1'){
    initiateGame();
  }
  xP = xP - movingRate;
  drawHoop(xP,yP);
  // управление героем
   if (myTouch.dataAvailable()&& !screenPressed) {
      fallRate=-6; // Setting the fallRate negative will make the bird jump
      screenPressed = true;
    }
   else if ( !myTouch.dataAvailable() && screenPressed){
     screenPressed = false;
    }
  //нужные функции
  void drawHomeScreen() {
    //  заголовок
    myGLCD.setBackColor(0,0,0); // делаем фон черным
    myGLCD.setColor(255, 255, 255); // меняем цвет на белый
    myGLCD.setFont(BigFont); // выбираем один из шрифтов
    myGLCD.print("Circus Charlie", CENTER, 64);
    // кнопка
    myGLCD.setColor(16, 167, 103);
    myGLCD.fillRoundRect (35, 140, 285, 180);
    myGLCD.setColor(255, 255, 255);
    myGLCD.drawRoundRect (35, 140, 285, 180);
    myGLCD.setFont(BigFont);
    myGLCD.setBackColor(16, 167, 103);
    myGLCD.print("PLAY", CENTER, 152);

    }
  void initiateGame(){
    myGLCD.clrScr();
    myGLCD.setColor(0, 0, 0);
    myGLCD.getBackColor();
    myGLCD.setColor(255, 255, 255);
    myGLCD.setFont(BigFont);
    myGLCD.print("STAGE 1", CENTER, 110);
    delay(2000);
    myGLCD.clrScr();
    myGLCD.setColor(0, 0, 0);
    myGLCD.getBackColor();
    myGLCD.setColor(16, 167, 103);
    myGLCD.fillRect(0, 100, 319, 239);
    //дорисовать фон

  void drawHoop(int x, int y){
    if (x>=300){
    myGLCD.setColor(235, 30, 1);
    myGLCD.drawRect(319, 120, x, y);
    }
    else if (x<=299){
      
    }
  }
  }
}  
