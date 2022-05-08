// OS_lab.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include "string"
#include <Windows.h>
#include <queue>

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
void RB(queue<process> prs, const int N, const int period, const int top_offset){
    cout << "Round Robin (" << period << "):\n";
    process tmp;
    for(int i = 0; i < N; i++){
        tmp = prs.front();
        cout << tmp.duration << " ";
        cout << tmp.priority << " ";
        cout << tmp.delay << "\n";
        prs.push(tmp);
        prs.pop();
    }
    int tick = 0;
    int top_offset_modificator = 0;
    while(1){
        int newPeriod = period;
        tmp = prs.front();
        for(int j = 0; j < period; j++){
            gotoxy(20 + tick + tick * period + j, 1 + top_offset + top_offset_modificator);
            cout << "И";
            tmp.duration--;
            if(tmp.duration == 0){
                tmp.finished = true;
                newPeriod = j + 1;
                break;
            }
        }
        prs.push(tmp);
        prs.pop();
        for(int i = 1; i < N - top_offset_modificator; i++){
            tmp = prs.front();
            for(int j = 0; j < newPeriod; j++){
                if(!tmp.finished && (tmp.delay <= tick)){
                    gotoxy(20 + tick  + tick * period + j, i + 1 + top_offset + top_offset_modificator);
                    cout << "Г";
                }
            }
            prs.push(tmp);
            prs.pop();
        }
        for(int i = N - top_offset_modificator; i < N; i++){
            tmp = prs.front();
            for(int j = 0; j < newPeriod; j++){
                if(!tmp.finished && (tmp.delay <= tick)){
                    gotoxy(20 + tick + tick * period + j, i + 1 + top_offset - N + top_offset_modificator);
                    cout << "Г";
                }
            }
            prs.push(tmp);
            prs.pop();
        }
        tmp = prs.front();
        prs.push(tmp);
        prs.pop();
        top_offset_modificator += 1;
        if(top_offset_modificator == N)
            top_offset_modificator = 0;
        tick++;
        bool stop = true;
        for(int i = 0; i < N; i++){
            tmp = prs.front();
            if(tmp.finished == false){
                stop = false;
                break;
            }
            prs.push(tmp);
            prs.pop();
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
    queue<process> prs2;
    prs2.push(process(4, 0, 0));
    prs2.push(process(1, 0, 0));
    prs2.push(process(3, 0, 0));
    prs2.push(process(5, 0, 0));
    //process prs2[N] = {process(4, 0, 0), process(1, 0, 0), process(3, 0, 0), process(5, 0, 0)};
    RB(prs2, prs2.size(), 2, 5);
    gotoxy(0, 20);
}