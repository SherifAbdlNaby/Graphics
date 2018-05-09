//
// Created by Sherif on 30-Apr-18.
//

#ifndef TASK_CLIPPER_H
#define TASK_CLIPPER_H

#include <windows.h>
#include <iostream>
#include <vector>
#include "LINE.h"
#include "FILL.h"

using namespace std;

int xr, xl, yt, yb;


enum side{
    LEFT, TOP, RIGHT, BOTTOM
};

void makeClippingRect(HDC hdc, int xs, int ys, int xe, int ye) {
    xl = min(xs, xe);
    xr = max(xs, xe);
    yb = min(ys, ye);
    yt = max(ys, ye);
    drawLineMid(hdc, xl, yt, xl, yb);
    drawLineMid(hdc, xl, yt, xr, yt);
    drawLineMid(hdc, xl, yb, xr, yb);
    drawLineMid(hdc, xr, yt, xr, yb);
}

vector<side> getSides(POINT p) {

    vector<side> sides;

    if (p.x < xl)
        sides.push_back(LEFT);

    if (p.x > xr)
        sides.push_back(RIGHT);

    if (p.y > yt)
        sides.push_back(TOP);

    if (p.y < yb)
        sides.push_back(BOTTOM);

    return sides;
}

double slope(POINT s, POINT e) {
    int dx = e.x - s.x;
    int dy = e.y - s.y;
    double m = 0;
    if (dx != 0)
        m = (double) dy / dx;
    return m;
}

POINT calcIntersection(POINT s, double &slope, POINT edge) {

    if (edge.x == 0)
        edge.x = (long)((edge.y - s.y) / slope + s.x);

    if (edge.y == 0)
        edge.y =  (long)((edge.x - s.x) * slope + s.y);

    return edge;
}

POINT correctSides(POINT point, double &slope, vector<side> &sides) {

    for (int i = 0; i < 2 && !sides.empty(); ++i) {
        side side = getSides(point).front();

        if (side == TOP)
            point = calcIntersection(point, slope, make_point(0, yt));

        if (side == BOTTOM)
            point = calcIntersection(point, slope, make_point(0, yb));

        if (side == RIGHT)
            point = calcIntersection(point, slope, make_point(xr, 0));

        if (side == LEFT)
            point = calcIntersection(point, slope, make_point(xl, 0));

        //update sides
        sides = getSides(point);
    }

    return point;
}

void drawClipLine(HDC hdc, int xs, int ys, int xe, int ye) {
    POINT start = make_point(xs, ys);
    POINT end   = make_point(xe, ye);

    vector<side> startSides = getSides(start);
    vector<side> endSides   = getSides(end);

    //check for common side.
    for (int i = 0; i < startSides.size(); i++)
        for (int j = 0; j < endSides.size(); j++)
            if (startSides[i] == endSides[j])
                return;

    double lineSlope = slope(start, end);

    //fix el start
    if (!startSides.empty())
        start = correctSides(start, lineSlope, startSides);

    //fix el end
    if (!endSides.empty())
        end = correctSides(end, lineSlope, endSides);

    //finally Draw.
    drawLineMid(hdc, start.x, start.y, end.x, end.y);
}


#endif //TASK_CLIPPER_H
