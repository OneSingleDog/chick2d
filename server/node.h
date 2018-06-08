#ifndef __NODE_H__
#define __NODE_H__

class node{
private:
	unsigned x;
	unsigned y;
public:
	node(unsigned _x = 0,unsigned _y = 0):x(_x),y(_y){}
	unsigned GetX()const{return x;}
	unsigned GetY()const{return y;}
	void SetX(unsigned _x){x = _x;}
	void SetY(unsigned _y){y = _y;}
};

#endif