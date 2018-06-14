#include "Game.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>

#ifdef MAC
Game::Game(){
	FILE*config=NULL;
	config = fopen( "/Users/mac/Desktop/学习/面向对象/finalprogram/chick2d/server/config.txt", "r");
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
	for (int i = 0;i<MAXLEVEL;++i)
		fscanf(config, "%lf%d%d", poison_DMG+i, poison_TIME+i,poison_SIZE+i);
	fclose(config);
}
#else
Game::Game(){
	FILE*config = NULL;
	fopen_s(&config, "E:\\VS2017\\Source\\chick2d-server\\x64\\Debug\\config.txt", "r");
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
		for (int j = 0;j<MAP_LENGTH;++j)
			{
			fscanf_s(config, "%d", &tmp);
			wall->Set(i, j, tmp);
			}
	for (int i = 0;i<MAXLEVEL;++i)
		fscanf_s(config, "%lf%d%d", poison_DMG+i, poison_TIME+i, poison_SIZE+i);
	fclose(config);
	}
#endif

Game::~Game()
	{
	EndGame();
	delete wall;
	}

void Game::InitGame(int p_num, int b_num){
	srand(time(0));
	player_num = p_num;
	BoxNumber = b_num;
	poison_X = 0;
	poison_Y = 0;
	poison_LEVEL = 0;
	started = false;
	connected = 0;
	living_count = 0;
	for (int i = 0;i<b_num;++i)
		{
		int xx = rand()%((MAP_RBOUND-MAP_LBOUND+1)*BOXSIZE)+MAP_LBOUND*BOXSIZE;
		int yy = rand()%((MAP_UBOUND-MAP_DBOUND+1)*BOXSIZE)+MAP_DBOUND*BOXSIZE;
		while (wall->IsWall(xx-BOXSIZE, yy-BOXSIZE)||wall->IsWall(xx-BOXSIZE, yy+BOXSIZE)||wall->IsWall(xx+BOXSIZE, yy-BOXSIZE)||wall->IsWall(xx+BOXSIZE, yy+BOXSIZE)||wall->IsWall(xx, yy+BOXSIZE)||wall->IsWall(xx, yy-BOXSIZE)||wall->IsWall(xx+BOXSIZE, yy)||wall->IsWall(xx-BOXSIZE, yy)||wall->IsWall(xx, yy))
			{
			xx = rand()%((MAP_RBOUND-MAP_LBOUND+1)*BOXSIZE)+MAP_LBOUND*BOXSIZE;
			yy = rand()%((MAP_UBOUND-MAP_DBOUND+1)*BOXSIZE)+MAP_DBOUND*BOXSIZE;
			}
		box[i] = new Box(xx, yy, i);
		box[i]->InitBoxByRandom();
		}
	memset(ShootSuccess, 0, sizeof(ShootSuccess));
	for (int i = 0;i<MAXPLAYER;++i)
		dead[i] = false;
}

void Game::EndGame(){
	if (!started)return;
	for (int i = 0;i<MAXPLAYER;++i)if (player[i])delete(player[i]);
	for (int i = 0;i<BoxNumber;++i)if (box[i])delete(box[i]);
	BoxNumber = 0;
	player_num = 0;
	poison_LEVEL = 0;
	started = false;
	connected = 0;
}

bool Game::alive(int player_id){
	return !dead[player_id];
}

void Game::disconnect(int player_id){
	if (dead[player_id])return;
	dead[player_id] = true;
	player[player_id]->LossHp(100, 1);
	if (started)Die(player_id);
}

string Game::login(const c_s_msg&msg, int&player_id){
	player_id = connected;
	if (msg.type)return "NULL";//ERROR
	player[player_id] = new Player(msg.x, msg.y);
	string user_name = string(msg.remark);
	player[player_id]->InitalPlayer(player_id, user_name);
	++connected;
	if (connected==player_num)//game begin
		{
		started = true;
		for (int i = 0;i<player_num;++i)
			if (!player[i]->JudgeDead())++living_count;
		Gamebegintime = clock();
		}
	return user_name;
}

s_c_msg&Game::info(int player_id){
	if (!started)
		{
		output.type = 0;
		output.infox = connected;
		output.infoy = player_id;
		output.live_count = player_num;
		}
	else if (player[player_id]->JudgeDead())
		{
		output.type = 2;
		output.infox = player[player_id]->GetKillAmount();
		output.infoy = living_count+1;
		dead[player_id] = true;
		}
	else if (living_count==1)
		{
		output.type = 3;
		output.infox = player[player_id]->GetKillAmount();
		output.infoy = 1;
		dead[player_id] = true;
		}
	else
		{
		output.type = 1;
		output.Boxnum = BoxNumber;
		output.infoy = player_id;
		output.live_count = living_count;
		output.Poison_X = poison_X + poison_SIZE[poison_LEVEL]/2;
		output.Poison_Y = poison_Y + poison_SIZE[poison_LEVEL]/2;
		output.Poison_LEVEL = poison_LEVEL;
		output.inpoison = player[player_id] -> GetInPoison();
		for(int i = 0;i < BoxNumber;++ i){
			output.Boxes[i].type = box[i]->GetType();
			output.Boxes[i].x = box[i] -> GetX();
			output.Boxes[i].y = box[i] -> GetY();
			output.Boxes[i].PillAmount[0] = box[i] -> GetPillOneAmount();
			output.Boxes[i].PillAmount[1] = box[i] -> GetPillTwoAmount();
			output.Boxes[i].PillAmount[2] = box[i] -> GetPillThreeAmount();
			output.Boxes[i].PillAmount[3] = box[i] -> GetPillFourAmount();
			output.Boxes[i].Armor = box[i] -> GetArmorNaijiu();
			if(box[i] -> GetWeaponOne() == NULL || box[i] -> GetWeaponOne() -> GetType() == FIST)output.Boxes[i].Wp1Type = -1;
			else output.Boxes[i].Wp1Type = box[i] -> GetWeaponOne() -> GetType();
			if(box[i] -> GetWeaponTwo() == NULL || box[i] -> GetWeaponTwo() -> GetType() == FIST)output.Boxes[i].Wp2Type = -1;
			else output.Boxes[i].Wp2Type = box[i] -> GetWeaponTwo() -> GetType();
			if(box[i] -> GetWeaponOne() == NULL)output.Boxes[i].Wp1Bullets = 0;
			else output.Boxes[i].Wp1Bullets = box[i] -> GetWeaponOne() -> GetTotalBullet();
			if(box[i] -> GetWeaponTwo() == NULL)output.Boxes[i].Wp2Bullets = 0;
			else output.Boxes[i].Wp2Bullets = box[i] -> GetWeaponTwo() -> GetTotalBullet();
		}
		output.currenthp = player[player_id] -> GetPlayerCurrentHP();
		output.Armornaijiu = player[player_id] -> GetArmorNaijiu();
		output.PillAmount[0] = player[player_id] -> GetPillOneAmount();
		output.PillAmount[1] = player[player_id] -> GetPillTwoAmount();
		output.PillAmount[2] = player[player_id] -> GetPillThreeAmount();
		output.PillAmount[3] = player[player_id] -> GetPillFourAmount();
		if(player[player_id] -> GetSubWeapon() == NULL || player[player_id] -> GetSubWeapon() -> GetType() == FIST)output.SubWeaponType = -1;
		else output.SubWeaponType = player[player_id] -> GetSubWeapon() -> GetType();
		if(player[player_id] -> GetMainWeapon() == NULL)output.MainWeaponCurBullet = 0;
		else output.MainWeaponCurBullet = player[player_id] -> GetMainWeapon() -> GetCurBullet();
		if(player[player_id] -> GetMainWeapon() == NULL)output.MainWeaponBackupBullet = 0;
		else output.MainWeaponBackupBullet = player[player_id] -> GetMainWeapon() -> GetBackupBullet();
		if(player[player_id] -> GetSubWeapon() == NULL)output.SubWeaponCurBullet = 0;
		else output.SubWeaponCurBullet = player[player_id] -> GetSubWeapon() -> GetCurBullet();
		if(player[player_id] -> GetSubWeapon() == NULL)output.SubWeaponBackupBullet = 0;
		else output.SubWeaponBackupBullet = player[player_id]  -> GetSubWeapon() -> GetBackupBullet();
		output.IsCuring = player[player_id] -> IsCuringNow();
		for(int i = 0;i < player_num;++ i){
			#ifdef MAC
			strcpy(output.user_name[i],player[i] -> GetUserName().c_str());
			#else
			strcpy_s(output.user_name[i], 16, player[i]->GetUserName().c_str());
			#endif
			output.x[i] = player[i] -> GetX();
			output.y[i] = player[i] -> GetY();
			output.Firing[i] = ShootSuccess[player_id][i]>0;
			if(ShootSuccess[player_id][i]>0)--ShootSuccess[player_id][i];
			if(player[i] -> GetMainWeapon() == NULL || player[i] -> GetMainWeapon() -> GetType() == FIST) output.MainWeaponType[i] = -1;
			else output.MainWeaponType[i] = player[i] -> GetMainWeapon() -> GetType();
			output.Isdead[i] = player[i] -> JudgeDead();
			output.BeKilledByPlayerId[i] = player[i] -> GetKillerId();
			output.face_angle[i] = player[i]->getFaceAngle();
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
	int xx = poison_X+rand()%poison_SIZE[poison_LEVEL];
	int yy = poison_Y+rand()%poison_SIZE[poison_LEVEL];
	while (wall->IsWall(xx-BOXSIZE, yy-BOXSIZE)||wall->IsWall(xx-BOXSIZE, yy+BOXSIZE)||wall->IsWall(xx+BOXSIZE, yy-BOXSIZE)||wall->IsWall(xx+BOXSIZE, yy+BOXSIZE)||wall->IsWall(xx, yy+BOXSIZE)||wall->IsWall(xx, yy-BOXSIZE)||wall->IsWall(xx+BOXSIZE, yy)||wall->IsWall(xx-BOXSIZE, yy)||wall->IsWall(xx, yy))
		{
		xx = poison_X+rand()%poison_SIZE[poison_LEVEL];
		yy = poison_Y+rand()%poison_SIZE[poison_LEVEL];
		}
	box[BoxNumber] = new Box(xx, yy, BoxNumber);
	box[BoxNumber]->InitBoxByAirdrop();
	++BoxNumber;
}

bool Game::Die(int player_id){
	if (!player[player_id]->JudgeDead())return false;
	box[BoxNumber] = new Box(player[player_id]->GetX()+8, player[player_id]->GetY()+8,BoxNumber);
	box[BoxNumber]->InitBoxByPlayer(player[player_id]);
	++BoxNumber;
	--living_count;
	return true;
}

void Game::merge(const c_s_msg&msg, int player_id){
	if (living_count<=1)return;
	if (msg.type!=1)return;
	if (player[player_id]->JudgeDead())return;
	int nowtime = (clock()-Gamebegintime)*1000/CLOCKS_PER_SEC;
	if (poison_LEVEL<MAXLEVEL-1&&nowtime>=poison_TIME[poison_LEVEL+1])change_poison();
	player[player_id]->ChangePosition(msg.x, msg.y);
	player[player_id]->setFaceAngle(msg.face_angle);
	if (msg.x>=poison_X&&msg.x<=(poison_X+poison_SIZE[poison_LEVEL])&&msg.y>=poison_Y&&msg.y<=(poison_Y+poison_SIZE[poison_LEVEL]))
		player[player_id]->OutPoison();
	else
		{
		player[player_id]->InPoison(nowtime, poison_DMG[poison_LEVEL]);
		Die(player_id);
		}
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
		if (msg.PickBl1&&box[msg.BoxId]->GetWeaponOne())
			{
			int type = box[msg.BoxId]->GetWeaponOne()->GetType();
			if(player[player_id]->PickBullet(type, box[msg.BoxId] -> GetWeaponOne() -> GetTotalBullet()))box[msg.BoxId]->TakeWeaponOneBul(box[msg.BoxId]->GetWeaponOne()->GetTotalBullet());
			}
		if (msg.PickBl2&&box[msg.BoxId]->GetWeaponTwo())
			{
			int type = box[msg.BoxId]->GetWeaponTwo()->GetType();
			if(player[player_id]->PickBullet(type, box[msg.BoxId] -> GetWeaponTwo() -> GetTotalBullet()))box[msg.BoxId]->TakeWeaponTwoBul(box[msg.BoxId]->GetWeaponTwo() -> GetTotalBullet());
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
	if(player[player_id]->GetMainWeapon())
		player[player_id]->GetMainWeapon()->LoadEnd(nowtime);
	if (msg.ShootAngle>=0)
		Shoot(player_id, msg.ShootAngle,nowtime);
	player[player_id]->CureStart(msg.curetype, nowtime);
	player[player_id]->CureEnd(nowtime);
}

#define SGN(x) ((x)<0?-1:1)
#define DIST(xa,ya,xb,yb) (sqrt(((xa)-(xb))*((xa)-(xb))+((ya)-(yb))*((ya)-(yb))))
void Game::Shoot(int player_id, double angle,unsigned nowtime){
	if (!player[player_id]->Shoot(nowtime))return;
	for (int i = 0;i<player_num;++i)++ShootSuccess[i][player_id];
	int times = player[player_id]->GetMainWeapon()->GetType()==SHOTGUN ? SHOTGUN_TIMES : 1;

	while (times)
		{
		angle = player[player_id]->GetShootAngle(angle);
		double mindist = player[player_id]->GetMainWeapon()->GetDistance();
		int target = -1;
		int sourcex = player[player_id]->GetX()+PLAYERSIZE/2, sourcey = player[player_id]->GetY()+PLAYERSIZE/2;
		double A = sin(angle), B = -cos(angle);
		double C = -(A*sourcex+B*sourcey);

		for (int i = 0;i<player_num;++i)
			{
			if (i==player_id)continue;
			if (player[i]->JudgeDead())continue;

			bool hit = false;
			int tmpx = player[i]->GetX(), tmpy = player[i]->GetY();

			if (SGN(A)!=SGN(tmpy-sourcey)||SGN(B)!=SGN(sourcex-tmpx))continue;

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
		if (target==-1)
			{
			--times;
			continue;
			}
		for (double tested_dist = 0;tested_dist<mindist;tested_dist += WALLSIZE/2.0)
			{
			int xx = int(sourcex+tested_dist*cos(angle));
			int yy = int(sourcex+tested_dist*cos(angle));
			if (xx<0||xx>=MAP_WIDTH||yy<0||yy>=MAP_LENGTH)
				{
				target = -1;
				break;
				}
			if (wall->IsWall(xx, yy))
				{
				target = -1;
				break;
				}
			}
		if (target==-1)
			{
			--times;
			continue;
			}
		player[target]->BeAttack(player[player_id]->GetMainWeapon()->GetDamage(), player_id);
		if (Die(target))player[player_id]->KillOnePlayer();
		--times;
		}
}
