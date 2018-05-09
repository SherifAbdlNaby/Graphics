//
// Created by Sherif on 08-May-18.
//

#ifndef PAINT_LOGS_H
#define PAINT_LOGS_H

#include <vector>
#include "windows.h"

using namespace std;

struct operationLog {
    int xs, ys, xe, ye;
    int operation;
    operationLog(int xs, int ys, int xe, int ye, int operation) : xs(xs), ys(ys), xe(xe), ye(ye),
                                                                  operation(operation) {}

    operationLog(){};


};

vector<operationLog> logs;

void saveOperationsLog(){
    ofstream file;
    file.open ("logs.log");

    for (int i = 0; i < logs.size(); ++i)
        file << logs[i].xs << endl << logs[i].ys  << endl << logs[i].xe << endl << logs[i].ye << endl << logs[i].operation << endl;

    file.close();
}

void readOperationsLog(){
    logs.clear();

    ifstream file;
    file.open ("logs.log");

    operationLog log;
    while(file >> log.xs >> log.ys >> log.xe >> log.ye >> log.operation)
        logs.push_back(log);

    file.close();
}

void addToOperationsLogs(operationLog operationLog){
    logs.push_back(operationLog);
}

#endif //PAINT_LOGS_H
