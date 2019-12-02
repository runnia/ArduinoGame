//
//  circusCharlie.h
//  circusCharlie
//
//  Created by Ксения on 19/11/2019.
//  Copyright © 2019 Амельченкова Ксения. All rights reserved.
//

#ifndef circusCharlie_h
#define circusCharlie_h

class CircusCharlieGame{
public:
    bool Update(char input);
private:
    bool playerIsInAir= false;
    int obstaclesSequence[7]= { 1, 5, 10, 15, 20, 25, 30 };
    int spawnedObstacles[100];
    int timer= 0;
    void ReadInput(char input);
    void UpdateObstacles();
    bool IsEnd();
    void Render();
};



#endif /* circusCharlie_h */
