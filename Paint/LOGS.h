//
// Created by Sherif on 08-May-18.
//

#ifndef PAINT_LOGS_H
#define PAINT_LOGS_H

#include <vector>
#include "windows.h"

using namespace std;



struct point{
    int x; int y;
};

vector<point> clicksPoints;
int operation;
int currentOperation = -1;
bool clipping = false;
int xr, xl, yt, yb;

struct operationLog {
    vector<point> points;
    int operation;
    operationLog(const vector<point> &points, int operation) : points(points), operation(operation) {}
    operationLog(){};
};

vector<operationLog> logs;

void saveOperationsLog(){
    ofstream file;
    file.open ("logs.log");
    for (int i = 0; i < logs.size(); ++i) {
        file << logs[i].points.size() << endl;
        for (int j = 0; j < logs[i].points.size(); ++j) {
            file << logs[i].points[j].x << endl;
            file << logs[i].points[j].y << endl;
        }
        file << logs[i].operation << endl;
    }
    file.close();
}

void readOperationsLog(){
    logs.clear();

    ifstream file;
    file.open ("logs.log");

    int pointSize,x ,y;
    while(file >> pointSize)
    {
        operationLog log;
        for (int i = 0; i < pointSize; ++i) {
            file >> x;
            file >> y;
            log.points.push_back({x,y});
        }
        file >> log.operation;
        logs.push_back(log);
    }

    file.close();
}

void addToOperationsLogs(operationLog operationLog){
    logs.push_back(operationLog);
}

#endif //PAINT_LOGS_H
