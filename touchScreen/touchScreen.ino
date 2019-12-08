#include "UTFT.h"                                      // Подключаем библиотеку для работы с дисплеем
#include "TouchScreen.h"                               // Подключаем библиотеку для работы с TouchScreen
#include <EEPROM.h>

//extern uint8_t charlie[0x46A];
const uint8_t RS   = A2;                               
const uint8_t WR   = A1;                               
const uint8_t CS   = A3;                               
const uint8_t RST  = A4;                               
const uint8_t SER  = A0;
const uint8_t YP   = A2;                               
const uint8_t XM   = A3;                               
const uint8_t YM   = 8;                                
const uint8_t XP   = 9;                              
//  Определяем экстремумы для значений считываемых с аналоговых входов при определении точек нажатия на TouchScreen:
const int tsMinX   = 140;                              // соответствующий точке начала координат по оси X
const int tsMinY   = 110;                              // соответствующий точке начала координат по оси Y
const int tsMaxX   = 955;                              // соответствующий максимальной точке координат по оси X
const int tsMaxY   = 910;                              // соответствующий максимальной точке координат по оси Y
const int mipPress = 10;                               // соответствующий минимальной степени нажатия на TouchScreen
const int maxPress = 1000;                             // соответствующий максимальной степени нажатия на TouchScreen
extern uint8_t SmallFont[];                            //подключение шрифтов из библиотеки
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];
// нужные цвета
//(20, 115, 40) - зеленый
//(13, 56, 163) - синий
//(240, 115, 26) - оранжевый
//(240, 115, 26) - бежевый
//
UTFT myGLCD(TFT28UNO, RS, WR, CS, RST, SER);           // Создаём объект для работы с дисплеем
TouchScreen ts = TouchScreen(XP, YP, XM, YM);          // Создаём объект для работы с TouchScreenUTFT 
int score = 0;
int lastSpeedUpScore = 0;
int highestScore;
int speedOfGame = 1;
char name[3] = "KSU";
bool gameStarted = false;
int currentPage = 0;
bool playerIsInAir = false;
int air = 0;
int obstaclesSequence1[7]= { 1, 5, 10, 15, 20, 25, 30 };
int sizeObstacles1 = 7;
int spawnedObstacles[2];
int sizeSpawnedObstacles = 0;
int timer= 0;
int charlieX = 60;
int xO = 319;
int movingRate = 10;
enum gameState {
  inMenu,
  inSettings,
  waitingInput,
  inputLocked
  };
gameState state = inMenu;

void setup() {
  Serial.begin(9600);                                    // Инициируем передачу данных в монитор последовательного порта на скорости 9600 бит/сек
  myGLCD.InitLCD();                                      // Инициируем работу с TFT дисплеем
  highestScore = EEPROM.read(0);
  myGLCD.clrScr();  
  drawHomeScreen();
}

void drawHomeScreen() { //вывод стартового экрана
    //  заголовок
    myGLCD.setBackColor(0,0,0); // делаем фон черным
    myGLCD.setColor(255, 255, 255); // меняем цвет на белый
    myGLCD.setFont(BigFont); // выбираем один из шрифтов
    myGLCD.print("Circus Charlie", CENTER, 64);
    // кнопка
    myGLCD.setColor(20, 115, 40);
    myGLCD.fillRoundRect (35, 140, 285, 180);
    myGLCD.setColor(255, 255, 255);
    myGLCD.drawRoundRect (35, 140, 285, 180);
    myGLCD.setFont(BigFont);
    myGLCD.setBackColor(20, 115, 40);
    myGLCD.print("PLAY", CENTER, 152);
    myGLCD.setColor(20, 115, 40);
    myGLCD.fillRoundRect (35, 190, 145, 220);
    myGLCD.fillRoundRect (175, 190, 285, 220);
    myGLCD.setFont(SmallFont);
    myGLCD.print("Settings", CENTER, 152);
    myGLCD.print("Scores", CENTER, 152);
    
}
void initiateGame(){ //функция, отвечающая за отрисовку фона
    currentPage = 1;
    myGLCD.clrScr(); 
    myGLCD.setColor(0, 0, 0);
    myGLCD.getBackColor();
    myGLCD.setColor(255, 255, 255);
    myGLCD.setFont(BigFont);
    myGLCD.print("STAGE 1", CENTER, 110);
    delay(2000);
    myGLCD.clrScr();
    myGLCD.setColor(20, 115, 40);
    myGLCD.fillRect(0, 120, 319, 239);
    myGLCD.setColor(13, 56, 163);
    myGLCD.fillRect(0, 80, 319, 120); 
    myGLCD.setColor(0,0,0);
//    myGLCD.drawLine(0,120,319,120);
//    myGLCD.drawLine(0,135,319,135);
    myGLCD.drawLine(0,165,319,165);
    myGLCD.drawLine(0,200,319,200);
    //myGLCD.setBackColor(16, 167, 103);
    myGLCD.setBackColor(0, 0, 0);
    myGLCD.setColor(255, 255, 255);
    myGLCD.setFont(SmallFont);
    myGLCD.print("Highest Score: ",5,5); //в углу экрана выводим рекорд
    myGLCD.printNumI(highestScore, 120, 6);
    myGLCD.print(">RESTART<",255,5); //кнопка чтобы перезапустить игру
    myGLCD.drawLine(0,23,319,23);
    myGLCD.setBackColor(20, 115, 40);
    myGLCD.print("Score:",5,220);
//    myGLCD.setBackColor(0,0,0);
//    myGLCD.setFont(BigFont);
//    myGLCD.print("TAP TO START",CENTER,100);
    //myGLCD.drawBitmap (50, 60, 20, 24, charlie);
    
}
//void Render(){
//  
//}
void gameOver(){ //функция, отвечающая за прекращение игры (при нажатии RESTART)
  delay(1000); 
  myGLCD.clrScr();
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setFont(BigFont);
  myGLCD.print("GAME OVER", CENTER, 40);
  myGLCD.print("Score:", 100, 80);
  myGLCD.printNumI(score,200, 80);
  myGLCD.print("Restarting...", CENTER, 120);
  myGLCD.setFont(SevenSegNumFont);
  myGLCD.printNumI(2,CENTER, 150);
  delay(1000);
  myGLCD.printNumI(1,CENTER, 150);
  delay(1000);
  if (score > highestScore) { //если счет больше рекорда, то он станосится новым рекордом
    highestScore = score;
    EEPROM.write(0,highestScore);
  }
  // Resets the variables to start position values
  score = 0;
  lastSpeedUpScore = 0;
  currentPage = 0; 
  initiateGame();
}
void drawObstacles(int x){
  myGLCD.setColor(240, 115, 26);
  myGLCD.drawRoundRect (x-10, 120, x-1, 160);
  myGLCD.setColor(20, 115, 40);
  myGLCD.fillRect (x, 120, x+10, 160);
}
void drawCharlie(int air){
  myGLCD.setColor(20, 115, 40);
  myGLCD.fillRect (30, 120, 40, 200);
  myGLCD.setColor(232, 194, 167);
  if (playerIsInAir){
  myGLCD.fillRect(30, 120, 40, 160);
  delay(200);
  }
  else{
  myGLCD.fillRect(30, 160, 40, 200);
  playerIsInAir = false;
  }
}

void loop() {
  TSPoint p = ts.getPoint();                             // Считываем координаты и интенсивность нажатия на TouchScreen в структуру p
  pinMode(XM, OUTPUT);                                   // Возвращаем режим работы вывода X- в значение «выход» для работы с дисплеем
  pinMode(YP, OUTPUT);                                   // Возвращаем режим работы вывода Y+ в значение «выход» для работы с дисплеем
  if (p.z > mipPress && p.z < maxPress) {                // Если степень нажатия достаточна для фиксации координат TouchScreen
                                                         // Преобразуем значения полученные с TouchScreen в координаты дисплея:
    p.x = map(p.x, tsMinX, tsMaxX, 0, 320);              // Преобразуем значение p.x от диапазона tsMinX...tsMaxX, к диапазону 0...320
    p.y = map(p.y, tsMinY, tsMaxY, 0, 240);              // Преобразуем значение p.y от диапазона tsMinY...tsMaxY, к диапазону 0...240
  }
  if ((p.x > 35 && p.x < 285) && (p.y > 140 && p.y < 180)){
    initiateGame();
    state = inputLocked;
  }
  
  if (gameStarted){
    air --;
    if (air == 0){
      playerIsInAir = false;
    }
    if ((p.x > 0 && p.x < 319) && (p.y > 30 && p.y < 239)){
      playerIsInAir = true;
      air = 2;
    }
    timer++; 
    drawCharlie(air);
    xO = xO - movingRate;
    drawObstacles(xO);
    delay(500);
  }
}  
  
