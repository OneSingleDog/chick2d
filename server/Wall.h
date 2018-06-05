#ifndef __WALL_H__
#define __WALL_H__

#define MAP_WIDTH 800
#define MAP_LENGTH 800

class Wall{
private:
	char wall[MAP_WIDTH][MAP_LENGTH];
public:
	Wall(char**);
	bool IsWall(int x,int y);
};

#endif