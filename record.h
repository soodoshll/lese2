#ifndef RECORD_H
#define RECORD_H
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <vector>
#include "SDL2_header.h"
using namespace std;
struct Record{
    unsigned int score;
    time_t time;
};

void WriteRecord(char *filename);

vector<Record> GetRecord(char *filename);

#endif // RECORD_H
