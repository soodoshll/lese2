#include "object.h"
#include <ctime>

Object * ObjList[MaxObj];
int ObjNumber = 0 ;
Object * ObjPlayer;
bool ListNeedClean     = false;

unsigned int LastClean = 0;
unsigned int LastTime  = 0;
unsigned int GameTime  = 0;
bool Suspend = false;
int Life = 5;
unsigned int Score = 0;
unsigned int SuperWeapon = 3;

 int RecoverTime = 0;

unsigned int BonusScore[3];

int LastSW = -10000;

//Image *imagePlayer, *imageBullet, *imageFighter, *imageCircle, *imageMissile, *imageBonus, *imageProtect, *imageBackground;
#include "draw.h"

Object * EnemyList[MaxEnemy];
int EnemyNumber = 0;

int EnemyAlive = 0;
bool GameEnding = false;

BulletType BulletTypeList[BulletTypeMax] =
//     Speed | Attack | Interval | R
    { {  5,       5,     200,        2},
      {  8,      10,     160,        2},
      {  12,      15,    120,        2},
      {  15,      20,    100,        2},
      {  18,      30,     80,        2},
      // 五种速度越来越快的子弹

      {  1.5,       5,     160,        4},
      {  2,     5,     400,        4},
      {  2.5,       5,     800,        4},
      // 三种缓慢的圆形子弹

      {  4,       5,     400,        2},
      {  8,       5,     400,        2},
      // 两种闪亮的子弹
};

FighterType FighterTypeList[FighterTypeMax] =
{
//Bullet | Speed | Hp | r |     Clip          |   Bonus   | Guns
    {0,      5,    5,   2,  {0, 0, 48, 48},      {0,0,0},     1},
    {1,      5,    5,   5,  {48, 0, 48, 48},     {0,0,0},     2},
    {2,      5,    5,   5,  {96, 0, 48, 48},     {0,0,0},     3},
    {3,      5,    5,   5,  {144, 0, 48, 48},    {0,0,0},     4},
    {4,      5,    5,   5,  {192, 0, 48, 48},    {0,0,0},     5},
    //五种飞机 速度不会变的 炮会越来越多

    {6,      5,   10,  20,  {0, 0, 48, 48},      {3,0,0},     1},
    {6,      6,   20,  20,  {0, 48, 64, 64},     {4,1,1},     1},
    {7,      7,   30,  20,  {64, 48, 64, 64},    {5,2,2},     1},
    {8,      8,   50,  20,  {128, 48, 64, 64},   {6,3,3},     1},
    {9,     10, 5000,  100,  {0, 112, 256, 256},  {60,0,0},     3},
    //敌机 攻击越来越强 最后一个是boss
};

void LoadBullet(){
    int i;
    for (i = 0 ; i < BulletTypeMax; i++)
        BulletTypeList[i].clip = {0, BIheight * i, BIwidth, BIheight};
}

inline void AddEnemy(Object * e){
    EnemyList[EnemyNumber++] = e;
    EnemyAlive ++;
}


inline double ObjDistance(Object *a, Object * b){
    return (a->pos - b->pos).length();
}

Object * ColDetect(Object * x){
    int i;
    Object ** ptr = ObjList;
    for (i = 0 ; i < ObjNumber ; ++i,++ptr)
        if (x != *ptr && (*ptr)->valid &&  (*ptr)->dietime == -1 &&x -> team != (*ptr) -> team && ObjDistance((*ptr), x) <= x->r + (*ptr)->r)
            return *ptr;
    return nullptr;
}

int ColDetect2(Object * x,int start){
    int i;
    Object * obj;
    for (i = start ; i < ObjNumber ; ++i){
        obj = ObjList[i];
        if (x != obj && obj->valid &&  obj->dietime == -1 && x -> team != obj -> team && ObjDistance(obj, x) <= x->r + obj->r)
            return i;
    }
    return -1;
}

void ColDetectEnemy(){
    int i;
    Object * cobj;
    for (i = 0 ; i < EnemyNumber; i++)
    if (EnemyList[i] -> valid && EnemyList[i] -> dietime == -1){
        cobj = ColDetect(EnemyList[i]);
        if (!cobj) continue;
        if (cobj -> type == t_fighter && cobj->recovery_time <= 0) EnemyCrush(EnemyList[i]);
        else if (cobj -> type == t_bullet) {
            cobj -> valid = false;
            ListNeedClean = true;
            //cout << (int)cobj <<endl;
            EnemyList[i]->hp -= BulletTypeList[cobj->type_id].attack;
            if (EnemyList[i]->hp <= 0 ) EnemyCrush(EnemyList[i]);
        }
    }
}

void BonusUpdate(){
    int cur_type = ObjPlayer -> type_id;
    if (cur_type < FighterUpgradeMax - 1 && BonusScore[0] >= FighterUpgrade[cur_type + 1][0]){
        ObjPlayer -> type_id    =  FighterUpgrade[cur_type + 1][1];
        ObjPlayer -> bullettype =  FighterTypeList[ObjPlayer -> type_id].bullettype;
        //BonusScore[0] -= BulletUpgrade[cur_type + 1][0];
    }
    if (BonusScore[1] >= LifeCost){
        BonusScore[1] -= LifeCost;
        Life++;
    }
    if (BonusScore[2] >= SuperWeaponCost){
        BonusScore[2] -= SuperWeaponCost;
        SuperWeapon++;
    }
}

void ColDetectPlayer(){
    int cobj_id= ColDetect2(ObjPlayer, 0);
    Object * cobj ;
    while (cobj_id > -1){
        cobj = ObjList[cobj_id];
        switch (cobj -> type){
        case t_bullet: case t_fighter: case t_missile:
            if (ObjPlayer -> recovery_time > 0){
                cobj_id = ColDetect2(ObjPlayer,cobj_id+1);
                continue;
            }
            if (Life > 0) Life--;
            Mix_PlayChannel(-1, soundDie, 0);
            if (cobj->type != t_fighter) cobj -> valid = false;
            ObjPlayer -> recovery_time = RecoverTimeMax;
            ListNeedClean = true;
            break;
        case t_bonus:
            BonusScore[cobj -> type_id] ++;
            Score ++;
            cobj -> valid = false;
            BonusUpdate();
            ListNeedClean = true;
            break;
        }
        cobj_id = ColDetect2(ObjPlayer,cobj_id+1);
    }
}

void EnemyCrush(Object * obj){
     //obj -> valid = false;
     ListNeedClean = true;
     if (! obj->valid || obj->dietime >=0) return;
     obj -> dietime = GameTime;
     EnemyAlive--;
     Mix_PlayChannel(-1, soundExplode, 0);
     int *b = FighterTypeList[obj -> type_id].bonus;
     srand(time(nullptr));
     int i, j;
     double x, y;
     for (i = 0 ; i < 3 ; i++)
     for (j = 0 ; j < b[i] ; j++){
        x = obj->pos.x + (rand() / double(RAND_MAX) - 0.5) * BonusRange;
        y = obj->pos.y + (rand() / double(RAND_MAX) - 0.5) * BonusRange;
        ObjList[ObjNumber++] = new Object(t_bonus, i, imageBonus, 1, PointD(x,y), PointD(0, 1) * BonusSpeed[i], 0, 10);
     }
}

void UpdatePos(Object * obj){
    obj -> pos = obj -> pos + obj -> vel;

    // restriction of player
    if (obj == ObjPlayer){
        if (obj -> pos.x > SPACE_WIDTH)  obj -> pos.x = SPACE_WIDTH;
        if (obj -> pos.y > SPACE_HEIGHT) obj -> pos.y = SPACE_HEIGHT;
        if (obj -> pos.x < 0) obj -> pos.x = 0;
        if (obj -> pos.y < 0) obj -> pos.y = 0;
    }

    if (obj -> pos.x < 0 || obj -> pos.y < 0 || obj -> pos.x > SPACE_WIDTH || obj -> pos.y > SPACE_HEIGHT){
        if (obj -> type == t_fighter && obj -> valid && obj->dietime == -1) EnemyAlive--;
        obj -> valid  = false;
        ListNeedClean = true;
    }
}

void UpdateAllPos(){
    int i;
    for (i = 0 ; i < ObjNumber; ++i)
        if (ObjList[i] -> valid)
            UpdatePos(ObjList[i]);
}

void CleanList(){
    int i, j = 0;
    //cout << "start cleaning" <<ObjNumber<<endl;
    for (i = 0 ; i < ObjNumber; ++i)
        if (ObjList[i] -> valid){
            ObjList[j++] = ObjList[i];
            }
        else{
            if (ObjList[i]->type != t_fighter)
            delete ObjList[i];
        }
    ListNeedClean = false;
    //cout << "cleaned " << ObjNumber <<' '<<j <<endl;
    ObjNumber = j;
}

MissileType MissileTypeList[MissileTypeMax] = {
  {5, 5, 0.1, 2},
  {5, 8, 0.2, 2}
};

Object * AddMissile(int missiletype, int team, PointD pos, PointD vel, Object * target){
    Object * tmp = new Object(t_missile, missiletype, imageMissile, team, pos, vel , MissileTypeList[missiletype].speed, MissileTypeList[missiletype].r);
    tmp -> target = target;
    return ObjList[ObjNumber++] = tmp;
}

void LaunchMissileF(Object * obj, Object * target, int missiletype, PointD vel){
    if (!(obj->valid) || obj->dietime >= 0) return ;
    AddMissile(missiletype, obj->team, obj-> pos, vel*MissileTypeList[missiletype].speed/vel.length(), target);
}

void UpdateMissile(Object * obj){
    PointD dPos = obj->target->pos - obj->pos ;
    PointD v0   = obj->vel;

    double dir = cross(dPos, v0);
    if (dir > 0){

        double rad = atan2(v0.y,v0.x) - pi / 2;
        obj->vel = obj->vel + PointD(cos(rad),sin(rad)) * MissileTypeList[obj->type_id].a;


    }else{
        double rad = atan2(v0.y,v0.x) + pi / 2;
        obj->vel = obj->vel + PointD(cos(rad),sin(rad)) * MissileTypeList[obj->type_id].a;
    }
    if (obj->vel.length() > obj->speed) obj->vel = obj->vel * obj->speed / obj->vel.length();
}

void UpdateMissiles(){
    int i;
    for (i = 0 ; i < ObjNumber; i++)
        if (ObjList[i]->valid && ObjList[i]->type == t_missile)
            UpdateMissile(ObjList[i]);
}

Object * AddBullet(int bullettype, int team, PointD pos, PointD vel){
    return ObjList[ObjNumber++] = new Object(t_bullet, bullettype, imageBullet, team, pos, vel, vel.length(), BulletTypeList[bullettype].r);
}



int ShootBullet(Object * obj, PointD vel){
    unsigned t = SDL_GetTicks();
    if (t - obj->last_shoot >= obj->interval) {
        AddBullet(obj->bullettype, obj->team, obj-> pos, vel*BulletTypeList[obj->bullettype].speed/vel.length());
        obj->last_shoot = t;
    }
    return 0;
}

int ShootBulletF(Object * obj, int bullettype, PointD vel){
    unsigned t = SDL_GetTicks();
    AddBullet(bullettype, obj->team, obj-> pos, vel*BulletTypeList[bullettype].speed/vel.length());
    return 0;
}


void FighterShoot(Object *obj){
    unsigned t = SDL_GetTicks();
    if (t - obj->last_shoot >= obj->interval) {
        int n = FighterTypeList[obj -> type_id].gun;
        PointD vel = PointD(0, 1)*(obj -> team?1:-1);
        if (n == 1) {
            ShootBulletF(obj, obj -> bullettype, vel*BulletTypeList[obj->bullettype].speed/vel.length());
        }else{
            int i;
            double spacing = FighterTypeList[obj->type_id].clip.w / (n-1);
            double left    = FighterTypeList[obj->type_id].clip.w / 2;
            for (i = 0 ; i < n; i++)
                AddBullet(obj->bullettype, obj->team, obj->pos - PointD(-left + spacing * i), vel*BulletTypeList[obj->bullettype].speed/vel.length());
        }
        obj->last_shoot = t;
    }
}

void EnemyAttack(){
    int i;
    for (i = 0 ; i < EnemyNumber ; i++)
        if (EnemyList[i] -> valid && EnemyList[i] -> shoot && EnemyList[i] -> dietime == -1)
            FighterShoot(EnemyList[i]);
}

void RadiateShoot(Object * obj, int bullettype, int num){
    int i;
    if (!(obj->valid) || obj -> dietime >= 0) return ;
    for (i = 0 ; i < num ; i++){
        ShootBulletF(obj, bullettype, PointD(sin(2*pi * i/num), cos(2*pi * i/num))* BulletTypeList[bullettype].speed);
    }
}

extern Instruct InsList[InsMax];
extern int InsNumber;
extern int InsPtr;

Object * NewFighter(int fightertype, int team, PointD pos){
    FighterType &t = FighterTypeList[fightertype];
    return ObjList[ObjNumber++] = new Object(t_fighter, fightertype, imageFighter, team, pos,PointD(),
                                              t.speed,t.r,t.hp,BulletTypeList[t.bullettype].interval);
}

int InsPtrE = 0;

int ReadIns(const Instruct & ins){
    Object * obj;

    switch (ins.opt){
    case (opt_new):
        cout<<"[script]"<<GameTime<<" new enemy:"<<ins.i1<<" at ("<<ins.r1<<", "<<ins.r2<<")"<<endl;
        obj = NewFighter(ins.i1, 1, PointD(ins.r1, ins.r2));
        AddEnemy(obj);
        break;
    case (opt_setv):
        cout<<"[script]"<<GameTime<<" set the velocity of "<<ins.id<<" as ("<<ins.r1<<", "<<ins.r2<<")"<<endl;
        EnemyList[ins.id]->vel   = PointD(ins.r1, ins.r2);
        EnemyList[ins.id]->speed = EnemyList[ins.id]->vel.length();
        break;
    case (opt_shoot):
        cout<<"[script]"<<GameTime<<" enemy"<<ins.id<<" starts shooting"<<endl;
        EnemyList[ins.id]->shoot = true;
        EnemyList[ins.id]->bullettype = ins.i1;
        EnemyList[ins.id]->interval   = BulletTypeList[ins.i1].interval;
        break;
    case (opt_stop):
        cout<<"[script]"<<GameTime<<" enemy"<<ins.id<<" stops shooting"<<endl;
        EnemyList[ins.id]->shoot = false;
        break;
    case (opt_r):
        cout<<"[script]"<<GameTime<<" enemy"<<ins.id<<" launches an all-round attack"<<endl;
        RadiateShoot(EnemyList[ins.id],ins.i1, ins.i2);
        break;
    case (opt_m):
        cout<<"[script]"<<GameTime<<" enemy"<<ins.id<<" launches a missile"<<endl;
        LaunchMissileF(EnemyList[ins.id], ObjPlayer, ins.i1, PointD(ins.r1,ins.r2));
        break;
    case (opt_aimed):
        cout<<"[script]"<<GameTime<<" enemy"<<ins.id<<" launches an aimed attack"<<endl;
        AimedBullet(EnemyList[ins.id], ObjPlayer, ins.i1, ins.i2, ins.r1);
        break;
    case (opt_e):
        cout<<"[script]"<<GameTime<<" game is preparing to end"<<endl;
        GameEnding = true;
        break;
    }
    return 0;
}

int deltaInsTime = 0;


void DealIns(){
	while (InsPtr < InsNumber && deltaInsTime + GameTime >= InsList[InsPtr].time){
        if (InsList[InsPtr].opt == opt_e) InsPtrE = InsPtr;
        ReadIns(InsList[InsPtr]);
        InsPtr++;
        if (InsPtr == InsNumber) {
            InsPtr = InsPtrE;
            deltaInsTime = InsList[InsPtr].time - GameTime;
        }
	}
}

void SuperAttack(){
    if (SuperWeapon <= 0) return;
    SuperWeapon--;
    Mix_PlayChannel(-1, soundSuperWeapon, 0);
    LastSW = GameTime;
    int i;
    for (i= 0 ; i < ObjNumber; i++)
    if (ObjList[i] -> valid && ObjList[i] -> team == 1 && ObjList[i] -> dietime == -1){
        switch (ObjList[i] -> type){
        case t_fighter:
            ObjList[i] -> hp -= SuperWeaponAttack;
            if (ObjList[i] -> hp <= 0) EnemyCrush(ObjList[i]);
            break;
        case t_missile:case t_bullet:
            ObjList[i] -> valid = false;
            break;
        }
    }
}

void AimedBullet (Object * obj, Object * target, int n, int bullettype, double theta){
    if (!(obj->valid) || (obj->dietime >=0)) return ;
    PointD dr = target -> pos - obj -> pos;
    if (n <= 1){
        ShootBulletF(obj, bullettype, dr);
        return ;
    }
    double theta0 = atan2(dr.y,dr.x);
    double rad = theta / 180 * pi;
    double dtheta = rad/(n-1);
    int i;
    for (i = 0; i < n; i++) {
       // ShootBulletF(obj, bullettype, PointD(cos(theta0 - theta / 2 + dtheta*i),sin(theta0 - theta / 2 + dtheta*i)));
       ShootBulletF(obj, bullettype, PointD(cos(theta0 + dtheta*i - rad/2),sin(theta0 + dtheta*i - rad/2)));
    }
}

