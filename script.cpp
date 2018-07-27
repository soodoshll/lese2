#include "script.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cctype>
using namespace std;

string RES_PATH_SCRIPT = getResourcePath("script");


Instruct InsList[InsMax];
int InsNumber = 0;
int InsPtr = 0;

extern string RES_PATH_SCRIPT;

inline void AddIns(int time , OptCode opt, int id, int i1, int i2, double r1, double r2){
    InsList[InsNumber ++] = (Instruct){time, opt, id, i1, i2, r1, r2};
}

void ReadInsFromFile(char * filename){

    cout << "loading instructions " <<RES_PATH_SCRIPT + filename<<endl;
    ifstream fin;
    fin.open(RES_PATH_SCRIPT + filename);
    static int time = 0;
    int c, id, i1, i2;
    double r1, r2;
    string opt;
    while ((c = fin.peek()) != EOF){

        //cout<<"peek: "<<(char)c<<endl;
        while (isblank(c)) {
            fin.get();
            c = fin.peek();
        }
        if (c == '#') while ((fin.peek()) != '\n' && fin.peek() != -1) fin.get();
        if (isalpha(c)) {
            fin >> opt;
            if (opt == "new"){
                fin >> i1 >> r1 >> r2;
                AddIns(time, opt_new, 0, i1, 0, r1, r2);
            }else if (opt == "setv"){
                fin >> id >> r1 >> r2;
                AddIns(time, opt_setv, id, 0, 0, r1, r2);
            }
            else if (opt =="shoot"){
                fin >> id >> i1;
                AddIns(time, opt_shoot, id, i1);
            }
            else if (opt == "stop"){
                fin >> id;
                AddIns(time, opt_stop, id);
            }
            else if (opt == "r"){
                fin >> id >> i1 >> i2;
                AddIns(time, opt_r, id, i1, i2);
            }
            else if (opt == "m"){
                fin >> id >> i1 >> r1 >> r2;
                AddIns(time, opt_m, id, i1, i2, r1, r2);
            }
            else if (opt == "aimed"){
                fin >> id >> i1 >> i2 >> r1;
                AddIns(time, opt_aimed, id, i1, i2, r1, 0);
            }
            else if (opt == "delay"){
                fin >> i1;
                time += i1;
            }
            else if (opt == "e"){
                AddIns(time,opt_e, 0,0,0,0,0);
            }
        }
        fin.get();
    }
}

