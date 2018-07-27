#ifndef GLOBAL_H
#define GLOBAL_H

#include <bits/stdc++.h>
#include "object.h"
#include "script.h"
#include "record.h"
#include "
const double pi = 3.1415926;

const int MaxObj = 10000;
//最大的对象数量

const int CleanPeriod  = 100;
//清理对象列表的周期

const int MaxEnemy = 1000;
//最大的敌机数量

const int BulletTypeMax = 10;
//最大的子弹种类数

const int BIwidth = 25, BIheight = 25;
//子弹图像的尺寸

//const double BULLET_SPEED = 5;

const int MissileTypeMax = 10;
//最大的导弹数量

const int SPACE_WIDTH	= 500;
const int SPACE_HEIGHT	= 600;
//战场大小

const int SuperWeaponAttack = 20;
//超级武器一次造成的伤害


const int FighterUpgradeMax = 5;
//最大战机级别


const int FighterTypeMax = 10;
//最大的战斗机类型数

const int FighterUpgrade[FighterUpgradeMax][2] = {
    //战机升级路线
    //分数要求， 战机类型
    {0,   0},
    {10,  1},
    {25,  2},
    {50,  3},
    {100, 4},
};

const double BonusSpeed[3] = {4, 6, 8};
//奖励掉落的速度

const double BonusRange = 80;
//奖励出现的位置范围

const int    BonusImageSize[2] = {16,16};
//奖励图像的尺寸


const int RecoverTimeMax = 3000;
//无敌时间

const int LifeCost        = 10;
//恢复一次生命需要消耗的分数

const int SuperWeaponCost = 10;
//获得一个超级武器需要消耗的分数

const int InitLife = 5;
const int InitSuperWeapon = 3;


extern string RES_PATH_SCRIPT;

extern Image *imagePlayer, *imageBullet, *imageFighter, *imageCircle, *imageMissile, *imageBonus, *imageProtect, *imageBackground,  *imageButton, *imageRBackground;

extern unsigned int BonusScore[3];

extern unsigned int Score;
extern unsigned int SuperWeapon;

extern Object * ObjList[MaxObj];
extern int ObjNumber ;

extern Object * ObjPlayer;
extern bool ListNeedClean;
extern const int CleanPeriod;
extern unsigned int LastClean;
extern unsigned int LastTime;
extern unsigned int GameTime;
extern bool Suspend;
extern int Life;

extern int RecoverTime;

extern Object * EnemyList[MaxEnemy];
extern int EnemyNumber;


extern BulletType BulletTypeList[BulletTypeMax];
extern FighterType FighterTypeList[FighterTypeMax];



extern Instruct InsList[InsMax];
extern int InsNumber;
extern int InsPtr;

#endif // GLOBAL_H
