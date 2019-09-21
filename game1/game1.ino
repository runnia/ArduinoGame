
#include <UTFT.h>
#include <URTouch.h>
UTFT myGLCD(CTE32HR,25,26,27,28); //?????
URTouch  myTouch( 6, 5, 4, 3, 2);
//подключение шрифтов из библиотеки
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];

void setup() {
  // инициализация дисплея
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);
  drawHomeScreen();  
  currentPage = '0'; // индикатор того, что мы находимся на начальнойй странице
 
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
  }
}  
