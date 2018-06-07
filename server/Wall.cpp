#include "Wall.h"

Wall::Wall(char** _wall){
    for(int i = 0;i < MAP_WIDTH;++ i){
        for(int j = 0;j < MAP_LENGTH;++ j){
            wall[i][j] = _wall[i][j];
        }
    }
}

bool IsWall(int x,int y){
    return wall[x][y] == 1 ? true:false;
}
