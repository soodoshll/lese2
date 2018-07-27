#ifndef OBJECT_H
#define OBJECT_H
#include "SDL2_header.h"
#include "script.h"
#include <iostream>
#include <cmath>
#include <SDL2/SDL_mixer.h>

using namespace Game;

using namespace std;

extern Mix_Chunk *soundExplode, *soundSuperWeapon, *soundDie;

const double pi = 3.1415926;

const int MaxObj = 10000;
//最大的对象数量

const int CleanPeriod  = 1;
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


const int RecoverTimeMax = 3000;
//无敌时间

const int LifeCost        = 10;
//恢复一次生命需要消耗的分数

const int SuperWeaponCost = 10;
//获得一个超级武器需要消耗的分数

const int InitLife = 3;
const int InitSuperWeapon = 2;

enum ObjectType {
    t_fighter,
    t_bullet ,   // Constant Velocity
    t_missile,   // Guided
    t_bonus ,
};

/* bonus 有三种：
   0 武器升级
   1 续命
   2 超级武器
*/

const int FighterUpgradeMax = 5;
//最大战机级别

const int FighterUpgrade[FighterUpgradeMax][2] = {
    //战机升级路线
    //分数要求， 战机类型
    {0,   0},
    {10,  1},
    {50,  2},
    {100, 3},
    {200, 4},
};

const double BonusSpeed[3] = {3, 4, 4};
//奖励掉落的速度

const double BonusRange = 80;
//奖励出现的位置范围

const int    BonusImageSize[2] = {16,16};
//奖励图像的尺寸

struct Object {
    ObjectType type;
    int        type_id;   //种类中的种类 (逃
    Image      *img;
    int        team;      //队伍 0 自机 1 敌机
    PointD     pos;       //位置
    PointD     vel;       //速度矢量
    double     speed;     //最大速率
    double     r;         //判定圈半径
    int        hp;        //剩余生命
    bool       valid;     //是否有效
    unsigned int last_shoot = 0; //上次发射子弹的时间
    unsigned int interval;     //发射子弹的时间间隔
    bool       shoot = false;  //是否正在发射子弹
    int        bullettype = 0; //子弹类型
    Object *   target;         //攻击目标
    int        recovery_time = -1; //剩余无敌时间
    int        dietime = -1;    //死亡时间
    Object  (ObjectType Type,
             int Typeid,
            Image * Img,
            int Team,
            PointD Pos = PointD(),
            PointD Vel = PointD(),
            double Speed = 0,
            double R = 1,
            int Hp = 10,
            unsigned int Interval = 0,
            bool Valid = true
            ):type(Type),type_id(Typeid),img(Img),pos(Pos),vel(Vel),speed(Speed),r(R),hp(Hp),team(Team),valid(Valid),
              interval(Interval){};
};


struct BulletType {
    double  speed  = 5;
    int     attack = 5;
    int     interval;
    double  r = 2;
    Rect    clip;
};

struct MissileType{
    int    attack;
    double speed = 0;
    double a = 0;        // acceleration 向心加速度
    double r;
    Rect   clip;
};

struct FighterType{
    int     bullettype;
    double  speed;
    int     hp;
    double  r = 5;
    Rect    clip;
    int     bonus[3];
    int     gun;    //机炮数量
};

const int FighterTypeMax = 10;
//最大的战斗机类型数


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

extern int LastSW;

extern bool Suspend;
extern int Life;

extern int RecoverTime;

extern Object * EnemyList[MaxEnemy];
extern int EnemyNumber;


extern BulletType BulletTypeList[BulletTypeMax];
extern FighterType FighterTypeList[FighterTypeMax];


extern int deltaInsTime;

extern Instruct InsList[InsMax];
extern int InsNumber;
extern int InsPtr;

extern int EnemyAlive;
extern bool GameEnding;


void LoadBullet();
//加载子弹的图片

inline void AddEnemy(Object * e);
//将e添加到敌机列表当中

inline double ObjDistance(Object *a, Object * b);
//计算两个对象间的距离

Object * ColDetect(Object * x);
//碰撞检测 返回对象列表中与x碰撞的第一个对象

int ColDetect2(Object * x,int start);
//碰撞检测 返回对象列表中下标start及以后的对象中与x碰撞的第一个对象

void ColDetectEnemy();
//对所有敌机进行碰撞检测 并计算伤害

void ColDetectPlayer();
//对自机进行碰撞检测 并计算伤害和奖励

void UpdatePos(Object * obj);
//通过速度矢量更新obj对象的位置

void UpdateAllPos();
//更新所有对象的位置

void CleanList();
//清理对象列表

Object * AddMissile(int missiletype, int team, PointD pos, PointD vel, Object * target);
//添加一个导弹，其中vel是速度方向矢量 只表示方向， 其模并没有什么用

void LaunchMissileF(Object * obj, Object * target, int missiletype, PointD vel);
//从obj对象发射一个导弹，其中vel是速度方向矢量 只表示方向， 其模并没有什么用

void UpdateMissile(Object * obj);
//更新一个导弹的速度矢量

void UpdateMissiles();
//更新所有导弹的速度矢量

Object * AddBullet(int bullettype, int team, PointD pos, PointD vel);
//添加一个子弹 *这个vel表示真的速度矢量 (逃

int ShootBullet(Object * obj, PointD vel);
//让obj发射一个方向vel的子弹 vel只表示方向

int ShootBulletF(Object * obj, int bullettype, PointD vel);
//不受时间限制的发射子弹 vel只表示方向

void EnemyAttack();
//处理每回合的敌机攻击行为

void RadiateShoot(Object * obj, int bullettype, int num);
//辐射弹幕

int ReadIns(const Instruct & ins);
//对特定脚本指令做出反应

void DealIns();
//根据处理脚本

void EnemyCrush(Object * obj);
//敌机爆炸之后的处理

void SuperAttack();
//发动超级攻击

void UpdateBulletVel(Object * obj);
//待实现：更新子弹的速度

void AimedBullet  (Object * obj, Object * target, int n, int bullettype, double theta);
//弧形自机狙弹幕（可实现奇数弹和偶数弹）

void FighterShoot(Object *obj);
#endif // OBJECT_H
