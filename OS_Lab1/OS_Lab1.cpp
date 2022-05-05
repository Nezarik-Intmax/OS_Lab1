// OS_lab.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include "string"
#include <Windows.h>
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
    bool finished;
    process(int d, int p, int dy){
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
void RB(process* prs, const int N, const int period, const int top_offset){
    cout << "Round Robin (" << period << "):\n";
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
                for(int j=0; j < period; j++){
                    if(prs[i].duration == 0) cout << " ";
                    else{
                        gotoxy(20 + tick + tick * period + j, i + 1 + top_offset);
                        cout << "И";
                        prs[i].duration--;
                        if(prs[i].duration == 0) prs[i].finished = true;
                    }
                }
            } else{
                for(int j = 0; j < period; j++){
                    if(!prs[i].finished && (prs[i].delay <= tick)){
                        gotoxy(20 + tick  + tick * period + j, i + 1 + top_offset);
                        cout << "Г";
                    }
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
int main(){
    setlocale(0, "rus");
    const int N = 4;
    process prs[N] = {process(3, 0 ,0), process(1, 0 ,0), process(2, 0 ,0), process(4, 1 ,1)};
    FIFS(prs, N);
    cout << "\n\n";
    process prs2[N] = {process(4, 0, 0), process(1, 0, 0), process(3, 0, 0), process(5, 0, 0)};
    RB(prs2, N, 2, 5);
    gotoxy(0, 20);
}