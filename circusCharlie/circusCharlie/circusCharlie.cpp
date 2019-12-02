//
//  main.cpp
//  circusCharlie
//
//  Created by Ксения on 29/10/2019.
//  Copyright © 2019 Амельченкова Ксения. All rights reserved.
//



#include <string>
#include "circusCharlie.h"

// Render
//
// 01234...
// ########################## 0
// endl
// endl
// endl
// if player is in air cout << "  X"
//   X       &                    <- player static pos [4, 2] & obstacle
// ##########################    <- ground
// when obstacle reaches y = 2 we should check if player is in air
const int SceneWidth = 10;
int air = 0;
    bool CircusCharlieGame::Update(char input)
    {
        ReadInput(input); // read input & updare player state (position, etc.)
        
        UpdateObstacles(); // update obstacles
        
        if (IsEnd())    // check for endgame
            return false;
        
        Render(); // render game scene
        
        return true; // return true which means we continue game
    }

    void CircusCharlieGame::ReadInput(char input)
    {
        // check if input is valid & player should do something
        // if valid => do something
        
        if (input){
            playerIsInAir = true;
            air = 2;
        }
        if (air == 0)
            playerIsInAir = false;
        
    }
    void CircusCharlieGame::UpdateObstacles()
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
    bool CircusCharlieGame::IsEnd() // check if player is inside one of the obstacles
    {
        if ((!playerIsInAir)&&(spawnedObstacles[0] == 2))
            return true;
        else
            return false;
    }
    void CircusCharlieGame::Render()
    {
        // display game
        std::cout.clear();
        std::cout << std::endl << std::endl << std::endl;
        if (playerIsInAir)
        {
            std::cout << "  X" << std::endl;
            int j = 0;
            for (int i = 0; i < SceneWidth; ++i)
            {
                if (i == spawnedObstacles[j])
                {
                    std::cout << "&";
                    j = (j + 1) % spawnedObstacles.size();
                }
                else std::cout << " ";
            }
            if (air!=0)
                air-=1;
        }
        else
        {
            std::cout << std::endl;
            int j = 0;
            for (int i = 0; i < SceneWidth; ++i)
            {
                if (i == 2) std::cout << "X";
                // if player bumbed into obsacle than we will display obstacle not a player
                if (i == spawnedObstacles[j])
                {
                    std::cout << "&";
                    j = (j + 1) % spawnedObstacles.size();
                }
                else std::cout << " ";
            }
        }
        std::cout << std::endl;
        // print ground
        for (int i = 0; i < SceneWidth; ++i)
            std::cout << "#";
        std::cout << std::endl;
    }

int main()
{
    char input;
    CircusCharlieGame game;
    do {
        std::cin >> input;
    } while (game.Update(input));
    std::cout << "You loosed!" << std::endl;
    return 0;
}
