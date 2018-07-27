/*
主要实现各种对象的绘制功能
*/

#ifndef DRAW_H
#define DRAW_H
#include "SDL2_header.h"
#include <iostream>
#include "object.h"
using namespace Game;

//将要用到的图片
extern Image *imagePlayer, *imageBullet, *imageFighter, *imageCircle, *imageMissile, *imageBonus,
      *imageProtect, *imageBackground, *imageButton, *imageRBackground, *imageExplode, *imageWelcome;

//主背景滚动的速度
const double BackgroundSpeed = 4;

//记录主背景滚到哪里了
extern double BackgroundH;

//加载图片
void loadPictures();

//绘制按钮
void DrawButton(std::string caption, int x, int y, bool highlight);

//绘制血条
void DrawFighterHP(Object * obj);

//绘制特定对象
int DrawObject(Object * obj);

//绘制画面上的所有对象
int DrawObjects();

//右边栏的 上间距 左间距 行间距
const int UpperMargin = 20;
const int LeftMargin  = 20;
const int LineSpace   = 10;

//在右边栏显示文字
inline void DisplayTextLine(char * text);

//在右边栏显示信息
void DrawInformation();

//绘制背景
void DrawBackground();


#endif // DRAW_H
