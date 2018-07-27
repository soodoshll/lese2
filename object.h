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
//���Ķ�������

const int CleanPeriod  = 1;
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


const int RecoverTimeMax = 3000;
//�޵�ʱ��

const int LifeCost        = 10;
//�ָ�һ��������Ҫ���ĵķ���

const int SuperWeaponCost = 10;
//���һ������������Ҫ���ĵķ���

const int InitLife = 3;
const int InitSuperWeapon = 2;

enum ObjectType {
    t_fighter,
    t_bullet ,   // Constant Velocity
    t_missile,   // Guided
    t_bonus ,
};

/* bonus �����֣�
   0 ��������
   1 ����
   2 ��������
*/

const int FighterUpgradeMax = 5;
//���ս������

const int FighterUpgrade[FighterUpgradeMax][2] = {
    //ս������·��
    //����Ҫ�� ս������
    {0,   0},
    {10,  1},
    {50,  2},
    {100, 3},
    {200, 4},
};

const double BonusSpeed[3] = {3, 4, 4};
//����������ٶ�

const double BonusRange = 80;
//�������ֵ�λ�÷�Χ

const int    BonusImageSize[2] = {16,16};
//����ͼ��ĳߴ�

struct Object {
    ObjectType type;
    int        type_id;   //�����е����� (��
    Image      *img;
    int        team;      //���� 0 �Ի� 1 �л�
    PointD     pos;       //λ��
    PointD     vel;       //�ٶ�ʸ��
    double     speed;     //�������
    double     r;         //�ж�Ȧ�뾶
    int        hp;        //ʣ������
    bool       valid;     //�Ƿ���Ч
    unsigned int last_shoot = 0; //�ϴη����ӵ���ʱ��
    unsigned int interval;     //�����ӵ���ʱ����
    bool       shoot = false;  //�Ƿ����ڷ����ӵ�
    int        bullettype = 0; //�ӵ�����
    Object *   target;         //����Ŀ��
    int        recovery_time = -1; //ʣ���޵�ʱ��
    int        dietime = -1;    //����ʱ��
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
    double a = 0;        // acceleration ���ļ��ٶ�
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
    int     gun;    //��������
};

const int FighterTypeMax = 10;
//����ս����������


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
//�����ӵ���ͼƬ

inline void AddEnemy(Object * e);
//��e��ӵ��л��б���

inline double ObjDistance(Object *a, Object * b);
//�������������ľ���

Object * ColDetect(Object * x);
//��ײ��� ���ض����б�����x��ײ�ĵ�һ������

int ColDetect2(Object * x,int start);
//��ײ��� ���ض����б����±�start���Ժ�Ķ�������x��ײ�ĵ�һ������

void ColDetectEnemy();
//�����ел�������ײ��� �������˺�

void ColDetectPlayer();
//���Ի�������ײ��� �������˺��ͽ���

void UpdatePos(Object * obj);
//ͨ���ٶ�ʸ������obj�����λ��

void UpdateAllPos();
//�������ж����λ��

void CleanList();
//��������б�

Object * AddMissile(int missiletype, int team, PointD pos, PointD vel, Object * target);
//���һ������������vel���ٶȷ���ʸ�� ֻ��ʾ���� ��ģ��û��ʲô��

void LaunchMissileF(Object * obj, Object * target, int missiletype, PointD vel);
//��obj������һ������������vel���ٶȷ���ʸ�� ֻ��ʾ���� ��ģ��û��ʲô��

void UpdateMissile(Object * obj);
//����һ���������ٶ�ʸ��

void UpdateMissiles();
//�������е������ٶ�ʸ��

Object * AddBullet(int bullettype, int team, PointD pos, PointD vel);
//���һ���ӵ� *���vel��ʾ����ٶ�ʸ�� (��

int ShootBullet(Object * obj, PointD vel);
//��obj����һ������vel���ӵ� velֻ��ʾ����

int ShootBulletF(Object * obj, int bullettype, PointD vel);
//����ʱ�����Ƶķ����ӵ� velֻ��ʾ����

void EnemyAttack();
//����ÿ�غϵĵл�������Ϊ

void RadiateShoot(Object * obj, int bullettype, int num);
//���䵯Ļ

int ReadIns(const Instruct & ins);
//���ض��ű�ָ��������Ӧ

void DealIns();
//���ݴ���ű�

void EnemyCrush(Object * obj);
//�л���ը֮��Ĵ���

void SuperAttack();
//������������

void UpdateBulletVel(Object * obj);
//��ʵ�֣������ӵ����ٶ�

void AimedBullet  (Object * obj, Object * target, int n, int bullettype, double theta);
//�����Ի��ѵ�Ļ����ʵ����������ż������

void FighterShoot(Object *obj);
#endif // OBJECT_H
