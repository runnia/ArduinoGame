
#include <Adafruit_GFX.h>    
#include <Adafruit_TFTLCD.h>
#include <TouchScreen.h>

#define YP A1 // must be an analog pin, use "An" notation! 
#define XM A2 // must be an analog pin, use "An" notation! 
#define YM 7 // can be a digital pin 
#define XP 6 // can be a digital pin 

//максимальное и миниальное напряжение по x и по y 
#define TS_MINX 150 
#define TS_MINY 120 
#define TS_MAXX 920 
#define TS_MAXY 940 

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF


class CircusCharlieGame
{
public:
    bool Update(char input)
    {
        // read input & updare player state (position, etc.)
        ReadInput(input);
        // update obstacles
        UpdateObstacles();
        // check for endgame
        if (IsEnd())
            return false;
        // render game scene
        Render();
        // return true which means we continue game
        return true;
    }
private:
    //std::vector<int> obstaclesSequence = { 1, 5, 10, 15, 20, 25, 30 };    // time between creating obstacles
    int timer = 0;                                                        // timer for checking if we need to generate new obstacle
    bool playerIsInAir = false;                                            // determines if player is in air (for 3 updates)
    //std::vector<int> spawnedObstacles;
    void ReadInput(char input)
    {
        // check if input is valid & player should do something
        // if valid => do something
        
        if (input == 'w'){
            playerIsInAir = true;
            air = 2;
        }
        if (air == 0)
            playerIsInAir = false;
        
    }
    void UpdateObstacles()
    {
        timer++;
        if (spawnedObstacles.size() > 0)
        {
            // move all existing obsacles to the left
            for (int i = 0; i < spawnedObstacles.size(); ++i)
                spawnedObstacles[i]--;
            // delete obstacles that are out of the scene
            if (spawnedObstacles[0] == 0)
                spawnedObstacles.erase(spawnedObstacles.begin());
        }
        // create new obstacles if time has come
        if (timer == obstaclesSequence[0])
        {
            spawnedObstacles.push_back(SceneWidth - 1);
            obstaclesSequence.erase(obstaclesSequence.begin());
        }
    }
    bool IsEnd() // check if player is inside one of the obstacles
    {
        if ((!playerIsInAir)&&(spawnedObstacles[0]==2))
            return true;
        else
            return false;
    }
//    void Render1()
//    {
//        // display game
//        std::cout.clear();
//        std::cout << std::endl << std::endl << std::endl;
//        if (playerIsInAir)
//        {
//            std::cout << "  X" << std::endl;
//            int j = 0;
//            for (int i = 0; i < SceneWidth; ++i)
//            {
//                if (i == spawnedObstacles[j])
//                {
//                    std::cout << "&";
//                    j = (j + 1) % spawnedObstacles.size();
//                }
//                else std::cout << " ";
//            }
//            if (air!=0)
//                air-=1;
//        }
//        else
//        {
//            std::cout << std::endl;
//            int j = 0;
//            for (int i = 0; i < SceneWidth; ++i)
//            {
//                if (i == 2) std::cout << "X";
//                // if player bumbed into obsacle than we will display obstacle not a player
//                if (i == spawnedObstacles[j])
//                {
//                    std::cout << "&";
//                    j = (j + 1) % spawnedObstacles.size();
//                }
//                else std::cout << " ";
//            }
//        }
//        std::cout << std::endl;
//        // print ground
//        for (int i = 0; i < SceneWidth; ++i)
//            std::cout << "#";
//        std::cout << std::endl;
//    }
    void Render()
    {
      
      
    }
};

void setup() {
  
  Serial.begin(115200);
  const int SceneWidth = 10;
  int air = 0;
  CircusCharlieGame game;
  tft.reset();
  
  uint16_t identifier = tft.readID();

  //Serial.print(F("LCD driver chip: "));
  //Serial.println(identifier, HEX);
    

  tft.begin(identifier);
  tft.fillScreen(BLUE);
  
}

void loop() {
  if (game.Update(input))
      char input  = Serial.read();
}
