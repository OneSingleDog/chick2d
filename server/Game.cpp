#include "Game.h"
#include <cstdlib>
#include <cstdio>
#include <cmath>

Game::Game(){
	FILE*config=NULL;
	config = fopen( "/Users/mac/Desktop/Test/Test/config.txt", "r");
	if (config==NULL)
		{
		printf("Open config failed\n");
		system("pause");
		exit(0);
		}
	else printf("Open config succeeded\n");
	int tmp;
	wall = new Wall;
	for (int i = 0;i<MAP_WIDTH;++i)
		for (int j = 0;j<MAP_WIDTH;++j)
			{
			fscanf(config, "%d", &tmp);
			wall->Set(i, j, tmp);
			}
	for (int i = 0;i<BEGINBOX;++i)
		fscanf(config, "%d%d", box_X+i, box_Y+i);
	for (int i = 0;i<MAXLEVEL;++i)
		fscanf(config, "%lf%d%d", poison_DMG+i, poison_TIME+i,poison_SIZE+i);
	fclose(config);
}

Game::~Game()
	{
	EndGame();
	delete wall;
	}

void Game::InitGame(){
	BoxNumber = BEGINBOX;
	poison_X = 0;
	poison_Y = 0;
	poison_LEVEL = 0;
	started = false;
	connected = 0;
	for (int i = 0;i<BEGINBOX;++i)
		{
		box[i] = new Box(box_X[i], box_Y[i], i);
		box[i]->InitBoxByRandom();
		}
	for(int i = 0;i < MAXPLAYER;++ i){
		ShootSuccess[i] = false;
	}
    Gamebegintime = clock();
	srand(time(0));
}

void Game::EndGame(){
	if (!started)return;
	for (int i = 0;i<MAXPLAYER;++i)if (player[i])delete(player[i]);
	for (int i = 0;i<BoxNumber;++i)if (box[i])delete(box[i]);
	BoxNumber = 0;
	poison_LEVEL = 0;
	started = false;
	connected = 0;
}

bool Game::alive(int player_id){
	if (living_count<=1)return false;//��Ϸ����
	return player[player_id]->JudgeDead();
}

string Game::login(const c_s_msg&msg, int player_id){
	if (msg.type)return "NULL";//����
	player[player_id] = new Player(msg.x, msg.y);
	string user_name = string(msg.remark);
	player[player_id]->InitalPlayer(player_id, user_name);
	++connected;
	if (connected==MAXPLAYER)
		{
		started = true;
		living_count = MAXPLAYER;
		}
	return user_name;
}

s_c_msg&Game::info(int player_id){
	if (!started)
		{
		output.type = 0;
		output.infox = connected;
		}
	else if (player[player_id]->JudgeDead())
		{
		output.type = 2;
		output.infox = player[player_id]->GetKillAmount();
		output.infoy = player[player_id]->GetKillerId();
		}
	else if (living_count==1)
		{
		output.type = 3;
		output.infox = player[player_id]->GetKillAmount();
		}
	else
		{
		output.type = 1;
		output.Poison_X = poison_X;
		output.Poison_Y = poison_Y;
		output.Poison_Size = poison_SIZE[poison_LEVEL];
		for(int i = 0;i < BoxNumber;++ i){
			output.Boxes[i].PillAmount[0] = box[i] -> GetPillOneAmount();
			output.Boxes[i].PillAmount[1] = box[i] -> GetPillTwoAmount();
			output.Boxes[i].PillAmount[2] = box[i] -> GetPillThreeAmount();
			output.Boxes[i].PillAmount[3] = box[i] -> GetPillFourAmount();
			output.Boxes[i].Armor = box[i] -> GetArmorNaijiu();
			output.Boxes[i].Wp1Type = box[i] -> GetWeaponOne() -> GetType();
			output.Boxes[i].Wp2Type = box[i] -> GetWeaponTwo() -> GetType();
			output.Boxes[i].Wp1Bullets = box[i] -> GetWeaponOne() -> GetTotalBullet();
			output.Boxes[i].Wp2Bullets = box[i] -> GetWeaponTwo() -> GetTotalBullet();
		}
		output.currenthp = player[player_id] -> GetPlayerCurrentHP();
		output.Armornaijiu = player[player_id] -> GetArmorNaijiu();
		output.PillAmount[0] = player[player_id] -> GetPillOneAmount();
		output.PillAmount[1] = player[player_id] -> GetPillTwoAmount();
		output.PillAmount[2] = player[player_id] -> GetPillThreeAmount();
		output.PillAmount[3] = player[player_id] -> GetPillFourAmount();
		output.SubWeaponType = player[player_id] -> GetSubWeapon() -> GetType();
		output.MainWeaponCurBullet = player[player_id] -> GetMainWeapon() -> GetCurBullet();
		output.MainWeaponBackupBullet = player[player_id] -> GetMainWeapon() -> GetBackupBullet();
		output.SubWeaponCurBullet = player[player_id] -> GetSubWeapon() -> GetCurBullet();
		output.SubWeaponBackupBullet = player[player_id]  -> GetSubWeapon() -> GetBackupBullet();
		for(int i = 0;i < MAXPLAYER;++ i){
			output.user_name[i] = player[i] -> GetUserName();
			output.x[i] = player[i] -> GetX();
			output.y[i] = player[i] -> GetY();
			output.IsCuring[i] = player[i] -> IsCuringNow();
			output.IsLoading[i] = player[i] -> GetMainWeapon() -> IsLoadingBullet();
			output.Firing[i] = ShootSuccess[i];
			ShootSuccess[i] = false;
			output.MainWeaponType[i] = player[i] -> GetMainWeapon() -> GetType();
			output.Isdead[i] = player[i] -> JudgeDead();
			output.BeKilledByPlayerId[i] = player[i] -> GetKillerId();
		}

	}
	return output;
}

void Game::change_poison(){
	if (poison_LEVEL>=MAXLEVEL)return;
	++poison_LEVEL;
	int t = poison_SIZE[poison_LEVEL-1]-poison_SIZE[poison_LEVEL]+1;
	poison_X += rand()%t;
	poison_Y += rand()%t;
}

bool Game::Die(int player_id){
	if (!player[player_id]->JudgeDead())return false;
	box[BoxNumber] = new Box(player[player_id]->GetX(), player[player_id]->GetY(),BoxNumber);
	box[BoxNumber]->InitBoxByPlayer(player[player_id]);
	++BoxNumber;
	return true;
}

void Game::merge(const c_s_msg&msg, int player_id){
	if (msg.type!=1)return;
	unsigned nowtime = clock()-Gamebegintime;
	if (poison_LEVEL<MAXLEVEL-1&&nowtime>=poison_TIME[poison_LEVEL+1])change_poison();
	player[player_id]->ChangePosition(msg.x, msg.y);
	if (msg.x>=poison_X&&msg.x<=poison_X+poison_SIZE[poison_LEVEL]&&msg.y>=poison_Y&&msg.y<=poison_Y+poison_SIZE[poison_LEVEL])
		{
		player[player_id]->InPoison(nowtime, poison_DMG[poison_LEVEL]);
		Die(player_id);
		}
	else player[player_id]->OutPoison();
	if (~msg.BoxId)
		{
		if (msg.PickPill[0]&&msg.PickPill[0]<=box[msg.BoxId]->GetPillOneAmount())
			{
			box[msg.BoxId]->TakePillOne(msg.PickPill[0]);
			player[player_id]->PickPillOne(msg.PickPill[0]);
			}
		if (msg.PickPill[1]&&msg.PickPill[1]<=box[msg.BoxId]->GetPillTwoAmount())
			{
			box[msg.BoxId]->TakePillTwo(msg.PickPill[1]);
			player[player_id]->PickPillTwo(msg.PickPill[1]);
			}
		if (msg.PickPill[2]&&msg.PickPill[2]<=box[msg.BoxId]->GetPillThreeAmount())
			{
			box[msg.BoxId]->TakePillThree(msg.PickPill[2]);
			player[player_id]->PickPillThree(msg.PickPill[2]);
			}
		if (msg.PickPill[3]&&msg.PickPill[3]<=box[msg.BoxId]->GetPillFourAmount())
			{
			box[msg.BoxId]->TakePillFour(msg.PickPill[3]);
			player[player_id]->PickPillFour(msg.PickPill[3]);
			}
		if (msg.PickArmor)
			{
			double tmp = box[msg.BoxId]->GetArmorNaijiu();
			box[msg.BoxId]->TakeArmor(player[player_id]->GetArmorNaijiu());
			player[player_id]->PickArmor(tmp);
			}
		if (msg.PickBl1&&box[msg.BoxId]->GetWeaponOne()&&msg.PickBl1<=box[msg.BoxId]->GetWeaponOne()->GetTotalBullet())
			{
			int type = box[msg.BoxId]->GetWeaponOne()->GetType();
			if(player[player_id]->PickBullet(type, msg.PickBl1))box[msg.BoxId]->TakeWeaponOneBul(msg.PickBl1);
			}
		if (msg.PickBl2&&box[msg.BoxId]->GetWeaponTwo()&&msg.PickBl2<=box[msg.BoxId]->GetWeaponTwo()->GetTotalBullet())
			{
			int type = box[msg.BoxId]->GetWeaponTwo()->GetType();
			if(player[player_id]->PickBullet(type, msg.PickBl2))box[msg.BoxId]->TakeWeaponTwoBul(msg.PickBl2);
			}
		if (msg.PickWp1)
			{
			Weapon*tmp = box[msg.BoxId]->GetWeaponOne();
			if (tmp)
				{
				tmp = player[player_id]->PickWeapon(tmp);
				box[msg.BoxId]->TakeWeaponOne(tmp);
				}
			}
		if (msg.PickWp2)
			{
			Weapon*tmp = box[msg.BoxId]->GetWeaponTwo();
			if (tmp)
				{
				tmp = player[player_id]->PickWeapon(tmp);
				box[msg.BoxId]->TakeWeaponTwo(tmp);
				}
			}
		}
	if (msg.Exchange)player[player_id]->ExchangeWeapon();
	if (msg.Load)player[player_id]->LoadBullet(nowtime);
	player[player_id]->GetMainWeapon()->LoadEnd(nowtime);
	if (msg.ShootAngle>=0)
		Shoot(player_id, msg.ShootAngle,nowtime);
	player[player_id]->CureStart(msg.type, nowtime);
	player[player_id]->CureEnd(nowtime);
}

#define SGN(x) ((x)<0?-1:1)
#define DIST(xa,ya,xb,yb) (sqrt(((xa)-(xb))*((xa)-(xb))+((ya)-(yb))*((ya)-(yb))))
void Game::Shoot(int player_id, double angle,unsigned nowtime){
	if (!player[player_id]->Shoot(nowtime))return;
	ShootSuccess[player_id] = true;
	int times = player[player_id]->GetMainWeapon()->GetType()==SHOTGUN ? 5 : 1;

	while (times)
		{
		angle = player[player_id]->GetShootAngle(angle);
		double mindist = player[player_id]->GetMainWeapon()->GetDistance();
		int target = -1;
		unsigned sourcex = player[player_id]->GetX()+PLAYERSIZE/2, sourcey = player[player_id]->GetY()+PLAYERSIZE/2;
		double A = cos(angle), B = -sin(angle);
		double C = -(A*sourcex+B*sourcey);

		for (int i = 0;i<MAXPLAYER;++i)
			{
			if (i==player_id)continue;
			if (player[i]->JudgeDead())continue;

			bool hit = false;
			unsigned tmpx = player[i]->GetX(), tmpy = player[i]->GetY();

			char sign = SGN(A*tmpx+B*tmpy+C);
			if (sign!=SGN(A*(tmpx+PLAYERSIZE)+B*tmpy+C))hit = true;
			if (sign!=SGN(A*(tmpx+PLAYERSIZE)+B*(tmpy+PLAYERSIZE)+C))hit = true;
			if (sign!=SGN(A*tmpx+B*(tmpy+PLAYERSIZE)+C))hit = true;

			if (hit)
				{
				double dist = DIST(sourcex, sourcey, tmpx, tmpy);
				if (dist<mindist)
					{
					mindist = dist;
					target = i;
					}
				dist = DIST(sourcex, sourcey, tmpx+PLAYERSIZE, tmpy);
				if (dist<mindist)
					{
					mindist = dist;
					target = i;
					}
				dist = DIST(sourcex, sourcey, tmpx, tmpy+PLAYERSIZE);
				if (dist<mindist)
					{
					mindist = dist;
					target = i;
					}
				dist = DIST(sourcex, sourcey, tmpx+PLAYERSIZE, tmpy+PLAYERSIZE);
				if (dist<mindist)
					{
					mindist = dist;
					target = i;
					}
				}
			}
		if (target==-1)continue;
		for (double tested_dist = 0;tested_dist<mindist;tested_dist += WALLSIZE/2.0)
			if (wall->IsWall(int(sourcex+tested_dist*cos(angle)), int(sourcey+tested_dist*sin(angle))))
				{
				target = -1;
				break;
				}
		if (target==-1)continue;
		player[target]->BeAttack(player[player_id]->GetMainWeapon()->GetDamage(), player_id);
		if (Die(target))player[player_id]->KillOnePlayer();
		--times;
		}
}
