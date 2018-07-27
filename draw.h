/*
��Ҫʵ�ָ��ֶ���Ļ��ƹ���
*/

#ifndef DRAW_H
#define DRAW_H
#include "SDL2_header.h"
#include <iostream>
#include "object.h"
using namespace Game;

//��Ҫ�õ���ͼƬ
extern Image *imagePlayer, *imageBullet, *imageFighter, *imageCircle, *imageMissile, *imageBonus,
      *imageProtect, *imageBackground, *imageButton, *imageRBackground, *imageExplode, *imageWelcome;

//�������������ٶ�
const double BackgroundSpeed = 4;

//��¼����������������
extern double BackgroundH;

//����ͼƬ
void loadPictures();

//���ư�ť
void DrawButton(std::string caption, int x, int y, bool highlight);

//����Ѫ��
void DrawFighterHP(Object * obj);

//�����ض�����
int DrawObject(Object * obj);

//���ƻ����ϵ����ж���
int DrawObjects();

//�ұ����� �ϼ�� ���� �м��
const int UpperMargin = 20;
const int LeftMargin  = 20;
const int LineSpace   = 10;

//���ұ�����ʾ����
inline void DisplayTextLine(char * text);

//���ұ�����ʾ��Ϣ
void DrawInformation();

//���Ʊ���
void DrawBackground();


#endif // DRAW_H
