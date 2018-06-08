#ifndef __WALL_H__
#define __WALL_H__

#define MAP_WIDTH 8000
#define MAP_LENGTH 8000

class Wall{
private:
	char wall[MAP_WIDTH][MAP_LENGTH];
public:
	Wall();
	bool IsWall(int x,int y);
	void Set(int x, int y, int value);
};

#endif
