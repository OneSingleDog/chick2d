#include "Wall.h"
#include <cstring>

Wall::Wall(){
	memset(wall, 0, sizeof(wall));
}

bool Wall::IsWall(int x,int y){
    return wall[x][y] == 1 ? true:false;
}

void Wall::Set(int x, int y, int value){
	wall[x][y] = char(value);
}
