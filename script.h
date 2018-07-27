#ifndef SCRIPT_H
#define SCRIPT_H
#include "SDL2_header.h"
using namespace Game;
/* This part is a script interpreter for stages in the game
 * Author : Su Qidong
 * The syntax:
 *     Line starts with a '#' :  annotation
 *     Basic Instruction:
 *         time  command  obj_id  parameters
 * Every instruction has at most 2 real parameters and 2 integer parameters, denoted as r1, r2, i1, i2

 */

const int InsMax = 1000;

enum OptCode {
    opt_new,
    opt_setv,
    opt_shoot,
    opt_stop,
    opt_r,
    opt_m,
    opt_aimed,
    opt_delay,
    opt_e
};

struct Instruct {
    unsigned int time;
    OptCode opt;
    int     id;
    int     i1;
    int     i2;
    double  r1;
    double  r2;

};

inline void AddIns(int time , OptCode opt, int id = 0, int i1 = 0, int i2 = 0, double r1 = 0, double r2 = 0);

void ReadInsFromFile(char * filename);


#endif // SCRIPT_H
