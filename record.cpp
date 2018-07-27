#include "record.h"
#include "object.h"
string RES_PATH_RECORD = getResourcePath("record");
extern unsigned int Score;
void WriteRecord(char *filename){
    ofstream fout(RES_PATH_RECORD + filename, ios::app);
    fout << Score << ' ' << time(nullptr) << endl;
}

bool cmp(Record &a, Record &b){
    return a.score > b.score;
}

vector<Record> GetRecord(char *filename){
    vector<Record> r;
    unsigned int s;
    time_t t;
    ifstream fin(RES_PATH_RECORD + filename, ios::app);
    while (fin >> s){
        fin >> t;
        r.push_back ((Record){s, t});
    }
    sort(r.begin(), r.end(), cmp);
    return r;
}
