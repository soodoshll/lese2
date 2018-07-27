#include "SDL2_header.h"
#include "draw.h"

Image *imagePlayer, *imageBullet, *imageFighter, *imageCircle, *imageMissile, *imageBonus,
      *imageProtect, *imageBackground, * imageButton, *imageRBackground, *imageExplode, *imageWelcome;

void loadPictures()
{
	imageCircle   = loadImage( "circle.png"	);
	imageBullet   = loadImage( "bullets.png"	);
	imageFighter  = loadImage( "fighter.png" );
	imageMissile  = loadImage( "missile.png" );
	imageBonus    = loadImage( "bonus.png" );
	imageProtect  = loadImage( "protect.png" );
	imageBackground  = loadImage( "background.png" );
    imageButton   = loadImage( "button.png" );
    imageRBackground = loadImage( "rbackground.png" );
    imageExplode  = loadImage( "red_strip24.png" );
    imageWelcome  = loadImage( "welcome.png" );
}


void DrawButton(std::string caption, int x, int y, bool highlight){
    int ButtonW,ButtonH;
    getImageSize(imageButton, ButtonW, ButtonH);
    ButtonH /= 2;
    Rect rect = {0, highlight * ButtonH, ButtonW, ButtonH};
    drawImage(imageButton, x, y, 1, 1, 0, NULL, FLIP_NONE, &rect);
    int w,h;
    getImageSize(imageButton, w, h);
    int upmargin   = 12;
    int leftmargin = 16;
    drawText(caption, x + leftmargin , y + upmargin);
}

void DrawFighterHP(Object * obj){
    int fullHP = FighterTypeList[obj->type_id].hp;
    int HP     = obj -> hp;
    const double rat = 0.05;
    double leftpart  = HP * rat;
    double rightpart = (fullHP - HP) * rat;
    const int h = 10;
    SDL_Rect rect1 = {obj->pos.x - fullHP * rat / 2,  obj -> pos.y - h / 2, leftpart, h};
    SDL_Rect rect2 = {obj->pos.x - fullHP * rat / 2 , obj -> pos.y - h / 2, fullHP * rat, h};
    setPenColor(255,0,0,95);
    drawRect(rect1, true);
    setPenColor(255,0,0,100);
    drawRect(rect2, false);
}


int DrawObject(Object * obj){
	int w,h;
	SDL_Rect rect;

	switch (obj->type){
	    case(t_fighter):

	        if (obj -> recovery_time > 0){
                getImageSize( imageProtect, w, h );
                drawImage(imageProtect, obj->pos.x - w/2, obj->pos.y - h/2);
	        }
	        if (obj -> dietime >= 0){
                int flip = (GameTime - obj->dietime)/50;
                SDL_Rect frect = {32 * flip, 0, 32 ,32};
                if (flip >= 24) obj -> valid = false;
                drawImage( imageExplode, obj->pos.x - 16 , obj->pos.y - 16,
                       1, 1, 0,NULL, FLIP_NONE,
                      &frect);
                return 1;
	        }
	        rect = FighterTypeList[obj->type_id].clip;
            drawImage( obj->img, obj->pos.x -   rect.w / 2 , obj->pos.y - rect.h / 2,
                       1, 1, obj->team * 180,NULL, FLIP_NONE,
                      &FighterTypeList[obj->type_id].clip);
            DrawFighterHP(obj);
            if (obj->team == 0)
                drawImage( imageCircle, obj->pos.x - FighterTypeList[obj->type_id].r , obj->pos.y - FighterTypeList[obj->type_id].r
                      , obj->r/32, obj->r/32);
        	break;

        case(t_missile):
            getImageSize( obj->img, w, h );
            drawImage( obj->img, obj->pos.x - w/2, obj->pos.y - h/2, 1, 1,  atan2(obj->vel.y, obj->vel.x)/pi*180);
            break;

        case(t_bullet):
            drawImage( obj->img, obj->pos.x -  BIwidth / 2 , obj->pos.y - BIheight / 2,
                       1, 1, atan2(obj->vel.y, obj->vel.x)/pi*180,NULL, FLIP_NONE,
                      &BulletTypeList[obj->type_id].clip);
            break;

        case(t_bonus):
            rect = {BonusImageSize[0] * obj->type_id, 0, BonusImageSize[0], BonusImageSize[1]};
            drawImage( obj->img, obj->pos.x - BonusImageSize[0]/2, obj->pos.y - BonusImageSize[1]/2, 1, 1, 0, NULL, FLIP_NONE,
                      &rect);
            break;
	}
	return 0;
}


int DrawObjects(){
    int i;
    for (i = ObjNumber -1 ; i >= 0 ; --i)
        if (ObjList[i] -> valid)
            DrawObject(ObjList[i]);

    return 0;
}

static int line = 0;

inline void DisplayTextLine(char * text){
    drawText( text, LeftMargin + SPACE_WIDTH, UpperMargin + (fontSize + LineSpace) * line  );
    ++line;
}



void DrawInformation(){
	char str[30];
	line = 0;

	sprintf(str, "Time: %d", GameTime/1000);
	DisplayTextLine(str);

	sprintf(str, "Life: %d", Life);
	DisplayTextLine(str);

	sprintf(str, "Score: %d", Score);
	DisplayTextLine(str);

	sprintf(str, "SuperWeapon: %d", SuperWeapon);
	DisplayTextLine(str);

    sprintf(str, "P: %d", BonusScore[0]);
	DisplayTextLine(str);

    sprintf(str, "R: %d", BonusScore[1]);
	DisplayTextLine(str);

    sprintf(str, "S: %d", BonusScore[2]);
	DisplayTextLine(str);

    //sprintf(str, "Object Number: %d", ObjNumber);
	//DisplayTextLine(str);

    //sprintf(str, "Enemy Alive: %d", EnemyAlive);
	//DisplayTextLine(str);

    if (Suspend)
        DisplayTextLine( "PAUSE");

}

double BackgroundH;


void DrawBackground(){
    drawImage(imageBackground, 0, BackgroundH);
    BackgroundH += BackgroundSpeed;
    if (BackgroundH > 0) {
	    int bw,bh;
	    getImageSize(imageBackground, bw, bh);
	    BackgroundH = -bh + SPACE_HEIGHT;
    }
}


