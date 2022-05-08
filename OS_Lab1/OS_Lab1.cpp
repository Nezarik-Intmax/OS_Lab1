// OS_lab.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include "string"
#include <Windows.h>
#include <list>

using namespace std;
void gotoxy(int xpos, int ypos){
    COORD scrn;
    HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
    scrn.X = xpos; scrn.Y = ypos;
    SetConsoleCursorPosition(hOuput, scrn);
}
class process{
public:
    int duration;
    int priority;
    int delay;
    int id;
    bool finished;
    process(){
        duration = 0;
        priority = 0;
        delay = 0;
        finished = false;
    }
    process(int d, int p, int dy){
        duration = d;
        priority = p;
        delay = dy;
        finished = false;
    }
    process(int _id, int d, int p, int dy){
        id = _id;
        duration = d;
        priority = p;
        delay = dy;
        finished = false;
    }
    ~process(){}
};
void FIFS(process* prs, const int N){
    cout << "FIFS:\n";
    for(int i = 0; i < N; i++){
        cout << prs[i].duration << " ";
        cout << prs[i].priority << " ";
        cout << prs[i].delay << "\n";
    }
    int tick = 0;
    while(1){
        int highPr = -1;
        int highPrInd = 0;
        for(int i = 0; i < N; i++){
            if((prs[i].priority > highPr) && (prs[i].delay <= tick) && (!prs[i].finished)){
                highPr = prs[i].priority;
                highPrInd = i;
            }
        }
        for(int i = 0; i < N; i++){
            if(i == highPrInd){
                gotoxy(20 + tick * 2, i + 1);
                cout << "И";
                prs[i].duration--;
                if(prs[i].duration == 0) prs[i].finished = true;
            } else{
                if(!prs[i].finished && (prs[i].delay <= tick)){
                    gotoxy(20 + tick * 2, i + 1);
                    cout << "Г";
                }
            }
        }
        tick++;
        bool stop = true;
        for(int i = 0; i < N; i++){
            if(prs[i].finished == false){
                stop = false;
                break;
            }
        }
        if(stop) break;

    }
}
void RB(list<process> prs, const int N, const int period, const int top_offset){
    cout << "Round Robin (" << period << "):\n";
    for(list<process>::iterator it = prs.begin(); it != prs.end(); it++){
        cout << it->duration << " ";
        cout << it->priority << " ";
        cout << it->delay << "\n";
    }
    int tick = 0;
    int periodNum = 0;
    bool deleted = false;
    while(!prs.empty()){
        int newPeriod = period;
        list<process>::iterator it = prs.begin();
        for(int j = period - 1; j >= 0; j--){
            gotoxy(20 + tick + periodNum, 1 + it->id + top_offset);
            cout << "И";
            it->duration--;
            tick++;
            if(it->duration == 0){
                newPeriod = period - j;
                break;
            }
        }
        if(it->duration == 0){
            it = prs.erase(it);
            deleted = true;
        }else{
            it++;
        }
        if(!prs.empty()){
            for(; it != prs.end(); it++){
                for(int j = newPeriod; j > 0; j--){
                    gotoxy(20 + tick + periodNum - j, it->id + 1 + top_offset);
                    cout << "Г";
                }
            }
            if(!deleted){
                prs.push_back(prs.front());
                prs.pop_front();
            }
            deleted = false;
        }
        periodNum++;
    }
}
int main(){
    setlocale(0, "rus");
    const int N = 4;
    process prs[N] = {process(3, 0 ,0), process(1, 0 ,0), process(2, 0 ,0), process(4, 1 ,1)};
    FIFS(prs, N);
    cout << "\n\n";
    list<process> prs2;
    prs2.push_back(process(0, 4, 0, 0));
    prs2.push_back(process(1, 1, 0, 0));
    prs2.push_back(process(2, 3, 0, 0));
    prs2.push_back(process(3, 5, 0, 0));
    //process prs2[N] = {process(4, 0, 0), process(1, 0, 0), process(3, 0, 0), process(5, 0, 0)};
    RB(prs2, prs2.size(), 2, 5);
    gotoxy(0, 20);
}