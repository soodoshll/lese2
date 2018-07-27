#ifndef GLOBAL_H
#define GLOBAL_H

#include <bits/stdc++.h>
#include "object.h"
#include "script.h"
#include "record.h"
#include "
const double pi = 3.1415926;

const int MaxObj = 10000;
//���Ķ�������

const int CleanPeriod  = 100;
//��������б������

const int MaxEnemy = 1000;
//���ĵл�����

const int BulletTypeMax = 10;
//�����ӵ�������

const int BIwidth = 25, BIheight = 25;
//�ӵ�ͼ��ĳߴ�

//const double BULLET_SPEED = 5;

const int MissileTypeMax = 10;
//���ĵ�������

const int SPACE_WIDTH	= 500;
const int SPACE_HEIGHT	= 600;
//ս����С

const int SuperWeaponAttack = 20;
//��������һ����ɵ��˺�


const int FighterUpgradeMax = 5;
//���ս������


const int FighterTypeMax = 10;
//����ս����������

const int FighterUpgrade[FighterUpgradeMax][2] = {
    //ս������·��
    //����Ҫ�� ս������
    {0,   0},
    {10,  1},
    {25,  2},
    {50,  3},
    {100, 4},
};

const double BonusSpeed[3] = {4, 6, 8};
//����������ٶ�

const double BonusRange = 80;
//�������ֵ�λ�÷�Χ

const int    BonusImageSize[2] = {16,16};
//����ͼ��ĳߴ�


const int RecoverTimeMax = 3000;
//�޵�ʱ��

const int LifeCost        = 10;
//�ָ�һ��������Ҫ���ĵķ���

const int SuperWeaponCost = 10;
//���һ������������Ҫ���ĵķ���

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
