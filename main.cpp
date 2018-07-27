/* main.cpp : 主要实现绘图和游戏进程的控制 */

//#include <SDL2/SDL_mixer.h>
#include "SDL2_header.h"
#include "object.h"
#include "script.h"
#include "record.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <map>
#include <cmath>
#include <ctime>
#include <cctype>
#include <string>
#include <SDL2/SDL_mixer.h>


char * RecordFile = "record.txt";
char * StageFile  = "stage3.game";

const int Game::SCREEN_WIDTH	= 800;
const int Game::SCREEN_HEIGHT	= 600;

string RES_PATH_MUSIC = getResourcePath("music");

const std::string Game::TitleName = "LESE-II";

using namespace Game;
using namespace std;
std::map<int, bool> keyboard;

extern string RES_PATH_SCRIPT;

#include "draw.h"

Mix_Chunk *soundExplode, *soundSuperWeapon, * soundDie;


void loadSound(){
    Mix_Music *sound=Mix_LoadMUS((RES_PATH_MUSIC + "rydeen.wav").c_str());
    Mix_VolumeMusic(MIX_MAX_VOLUME/16);
  	Mix_PlayMusic(sound,-1);
    Mix_AllocateChannels(32);
  	soundExplode = Mix_LoadWAV((RES_PATH_MUSIC + "explode.wav").c_str());
  	Mix_VolumeChunk(soundExplode, MIX_MAX_VOLUME/8);
    soundSuperWeapon = Mix_LoadWAV((RES_PATH_MUSIC + "superweapon.wav").c_str());
    Mix_VolumeChunk(soundSuperWeapon, MIX_MAX_VOLUME/8);
    soundDie = Mix_LoadWAV((RES_PATH_MUSIC + "laser8.wav").c_str());
    Mix_VolumeChunk(soundDie, MIX_MAX_VOLUME/8);
}

void initialize()
{
	//Display FPS
	FPS_DISPLAY = true;

    SDL_Init(SDL_INIT_AUDIO);
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048);

    loadSound();
	loadPictures();
	LoadBullet();

	int bw,bh;
	getImageSize(imageBackground, bw, bh);
	BackgroundH = -bh + SPACE_HEIGHT;

    ReadInsFromFile(StageFile);
}

int GameStage = 0; //0 welcome 1 Game 2 High ScoreScore 3 GameOver 4 Win
int Cursor    = 0;

// 欢迎画面
void welcome(){
    DrawBackground();
    DrawButton("start", 200, 300, Cursor == 0);
    DrawButton("score", 200, 400, Cursor == 1);
    DrawButton("exit",  200, 500, Cursor == 2);
    drawImage(imageRBackground, SPACE_WIDTH, 0);  //绘制画面右侧
    drawImage(imageWelcome, 50, 100);
}


// 纪录画面
void DisplayRecord(int n){

    int i;
    const int LeftMargin = 40;
    const int UpMargin = 80;
    const int Spacing = 30;
    vector<Record> recordlist = GetRecord(RecordFile);
    char str[100];
    drawText("Press R to return to menu", LeftMargin, 40, 25);
    for (i = 0 ; i < n && i < recordlist.size() ; i++){
        sprintf(str, "No%d: %d  %s", i+1, recordlist[i].score, ctime(&(recordlist[i].time)));
        drawText(str, LeftMargin, UpMargin + Spacing * i, 20);
    }

}

void draw()
{
    DrawBackground();

    //绘制分割左右两边的线
    setPenColor(0,255,255, 100);
    drawLine(SPACE_WIDTH, 0, SPACE_WIDTH, SCREEN_HEIGHT);

    if (GameStage == 0){
        welcome();
        return ;
    }

    if (GameStage == 2){
        DisplayRecord(15);
        return ;
    }

    if (GameStage == 3){
        //DrawBackground();
        drawText("Good Game", 60, 250, 64);
        drawText("Press R to return to menu", 60, 320);
        DrawInformation();
        return ;
    }

    if (GameStage == 4){
        //DrawBackground();
        drawText("You Win!", 60, 250, 64);
        drawText("Press R to return to menu", 60, 320);
        DrawInformation();
        return ;
    }
	//int bw,bh;
	//getImageSize(imageBackground, bw, bh);

    DrawObjects();
    drawImage(imageRBackground, SPACE_WIDTH, 0);
    DrawInformation();

    //暂停画面变暗
    if (Suspend) {
        setPenColor(0, 0, 0, 100);
        SDL_Rect rect = {0, 0, SPACE_WIDTH, SPACE_HEIGHT};
        drawRect(rect, true);
    }

    //发动超级攻击之后的闪烁
    int swflip = (GameTime - LastSW) / 60;
    if (swflip < 8 && swflip % 2){
        SDL_Rect rect = {0, 0, SPACE_WIDTH, SPACE_HEIGHT};
        setPenColor(255,255,255,50);
        drawRect(rect, true);

    }

}



void gameover(){
    GameStage = 3;
    WriteRecord(RecordFile);
}


void deal()
{
    drawImage(imageRBackground, SPACE_WIDTH, 0);

    if (GameStage == 0)
        return ;
    if (GameStage == 2)
        return ;
    if (GameStage == 3)
        return ;
    if (GameStage == 4)
        return ;

	bool move = false;
	//Calculate velocity

	if (GameEnding && EnemyAlive <= 0){
        GameStage = 4;
        WriteRecord(RecordFile);
    }

	if (Suspend) {

        LastTime = SDL_GetTicks();
        //drawText( "PAUSE", 300, 0 );
        return ;
	}


    int deltaT = SDL_GetTicks() - LastTime;
	GameTime += deltaT;

	//计算恢复时间的变化
    if (ObjPlayer->recovery_time > 0) ObjPlayer-> recovery_time -= deltaT;
    if (ObjPlayer->recovery_time < 0) ObjPlayer-> recovery_time = 0;

	LastTime = SDL_GetTicks();

    DealIns();

    ObjPlayer->vel = PointD();

	if( keyboard[KEY_UP]	|| keyboard['w'] )
	{
		ObjPlayer->vel = ObjPlayer->vel + PointD(0,-1)*ObjPlayer->speed;
		move = true;
	}
	if( keyboard[KEY_DOWN]	|| keyboard['s'] )
	{
		ObjPlayer->vel = ObjPlayer->vel + PointD(0,+1)*ObjPlayer->speed;
		move = true;
	}
	if( keyboard[KEY_LEFT]	|| keyboard['a'] )
	{
		ObjPlayer->vel = ObjPlayer->vel + PointD(-1,0)*ObjPlayer->speed;
		move = true;
	}
	if( keyboard[KEY_RIGHT] || keyboard['d'] )
	{
		ObjPlayer->vel = ObjPlayer->vel + PointD(+1,0)*ObjPlayer->speed;
		move = true;
	}
    if ( keyboard['z'])
        FighterShoot(ObjPlayer);


	//Limit player's speed
	double len = ObjPlayer->vel.length();
	if( len > ObjPlayer->speed )
	{
		ObjPlayer->vel = ObjPlayer->vel/len*ObjPlayer->speed;
	}

    if ( keyboard[SDLK_LSHIFT])
        ObjPlayer->vel = ObjPlayer -> vel * 0.6;
	//Calculate new position

    UpdateAllPos();
    EnemyAttack();
    UpdateMissiles();

	//Stop player

    ColDetectPlayer();
    ColDetectEnemy();

    if (Life <= 0)
        gameover();

	if ((SDL_GetTicks() - LastClean >= CleanPeriod) && ListNeedClean){
            LastClean = SDL_GetTicks();
            CleanList();
	}


}

bool Quit = false;

int work( bool &quit )
{
	//Calculate sth.
	deal();

	//Draw on the screen
	draw();

	//if( keyboard[KEY_ESC] )
	//    quit = true;
	if (Quit)
	    quit = Quit;

	return 0;
}

void mousePress()
{
}

void mouseMove()
{

}

void mouseRelease()
{

}

void keyDown()
{
	keyboard[keyValue] = true;
}

//开始游戏 进行初始化
void StartGame(){
    GameStage = 1;
    EnemyAlive = 0;
    GameEnding = false;
    LastTime = SDL_GetTicks();
    int i;

    for (i = 0 ; i < EnemyNumber ; i++){
        if ((!EnemyList[i]->valid)) delete EnemyList[i];
    }

    EnemyNumber = 0;
    deltaInsTime = 0;

    for (i = 0 ; i < ObjNumber ; i++){
        delete ObjList[i];
        ObjList[i] = nullptr;
    }

    ObjNumber = 0;
    ObjPlayer = ObjList[ObjNumber++]  = new Object(t_fighter,0,  imageFighter, 0, PointD(SPACE_WIDTH / 2, 560), PointD(), 5, 8, 100, 200);
    Score = 0;
    BonusScore[0] = BonusScore[1] = BonusScore[2] =  0;
    Life = InitLife;
    SuperWeapon = InitSuperWeapon;
    InsPtr = 0 ;
    GameTime = 0;
}

void keyUp()
{
    //欢迎菜单的选择
    if (GameStage == 0){
        //welcome();
        if( keyboard[KEY_UP]	|| keyboard['w'] )
            Cursor = (Cursor + 2) % 3;
        if( keyboard[KEY_DOWN]	|| keyboard['s'] )
            Cursor = (Cursor + 1) % 3;
        if( keyboard[KEY_ENTER])
        switch (Cursor){
        case 0:
            StartGame();
            break;
        case 1:
            GameStage = 2;
            //LastTime = SDL_GetTicks();
            break;
        case 2:
            Quit = true;
            break;
        }

    }
    if (GameStage == 2 || GameStage == 3 || GameStage == 4){
        if( keyboard['r'] )
            GameStage = 0;
    }

    if ( keyboard['x'])
        SuperAttack();
    if (keyboard[KEY_ESC]) Suspend = !Suspend;
	keyboard[keyValue] = false;
}

void finale()
{
	//Delete all images
	cleanup(imagePlayer, imageBullet, imageFighter, imageCircle, imageMissile, imageBonus,
      imageProtect, imageBackground, imageButton, imageRBackground, imageExplode, imageWelcome);
    Mix_FreeChunk(soundExplode);
    Mix_FreeChunk(soundSuperWeapon);
    Mix_FreeChunk(soundDie);
}

