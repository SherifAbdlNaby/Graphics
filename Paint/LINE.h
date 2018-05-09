//
// Created by Sherif on 30-Apr-18.
//

#ifndef TASK_LINE_H
#define TASK_LINE_H

#include "menu.h"
#include "UTIL.h"
#include "LOGS.h"
#include "CLIPPER.h"
#include <tchar.h>
#include <windows.h>
#include <cmath>
#include <stack>
#include <queue>
#include <string>

using namespace std;

void drawLine(HDC hdc, int xS, int yS, int xE, int yE) {
    int dx = xE - xS;
    int dy = yE - yS;
    if(abs(dy) <= abs(dx))
    {
        if(dx < 0)
        {
            swap(xE,xS);
            swap(yE,yS);
            dx = -dx;
            dy = -dy;
        }
        double m = 0;

        if(dx != 0)
            m = (double)dy/dx;

        float x = xS,y = yS;
        for (x = xS; x <= xE ; ++x) {
            y = m * (x - xS) + yS;
            SetPixel(hdc,x,y,RGB(0, 0, 0));
        }
    }
    else
    {
        if(dy < 0)
        {
            swap(xE,xS);
            swap(yE,yS);
            dx = -dx;
            dy = -dy;
        }
        double m = 0;
        double change = 0;

        if(dx != 0){
            m = (double)dy/dx;
            change = 1/m;
        }

        float x = xS,y = yS;
        for (y = yS; y <= yE ; ++y) {
            x = (float)change * (y - yS) + xS;
            SetPixel(hdc,x,y,RGB(0, 0, 0));
        }
    }
}

void drawLineDDA(HDC hdc, int xS, int yS, int xE, int yE) {
    int dx = xE - xS;
    int dy = yE - yS;
    if(abs(dy) <= abs(dx))
    {
        if(dx < 0)
        {
            swap(xE,xS);
            swap(yE,yS);
            dx = -dx;
            dy = -dy;
        }
        double m = 0;

        if(dx != 0)
            m = (double)dy/dx;

        SetPixel(hdc,xS,yS,RGB(0,0,0));
        int x = xS;
        double y = yS;
        while(x<xE)
        {
            x += 1;
            y += m;
            SetPixel(hdc,x,Round(y),RGB(0, 0, 0));
        }
    }
    else
    {
        if(dy < 0)
        {
            swap(xE,xS);
            swap(yE,yS);
            dx = -dx;
            dy = -dy;
        }
        double m = 0;
        double change = 0;

        if(dx != 0){
            m = (double)dy/dx;
            change = 1/m;
        }

        SetPixel(hdc,xS,yS,RGB(0, 0, 0));

        int y = yS;
        double x = xS;
        while(y<yE)
        {
            y += 1 ;
            x += change ;
            SetPixel(hdc,Round(x),y,RGB(0, 0, 0));
        }
    }
}

void drawLineMid(HDC hdc, int xs, int ys, int xe, int ye) {
    int dx = xe - xs;
    int dy = ye - ys;

    //Work on X
    if (abs(dy) <= abs(dx)) {
        //force left -> right drawing.
        if (dx < 0) {
            swap(xe, xs);
            swap(ye, ys);
            dx = dx * -1;
            dy = dy * -1;
        }

        //decision
        int d = dx - abs(2 * dy);

        //calculate Initial P/N
        int P = 2 * dx - abs(2 * dy);
        int N = -2 * abs(dy);

        //set first pixel
        SetPixel(hdc, xs, ys, RGB(0, 0, 0));

        int x = xs;
        int y = ys;
        while (x < xe) {
            x++;
            if (d <= 0) {
                d += P;
                if (dy <= 0)
                    y -= 1;
                else
                    y += 1;
            } else
                d += N;
            SetPixel(hdc, x, y, RGB(0, 0, 0));
        }
    }
        // Work on Y (mirrored)
    else {
        //force up -> down drawing.
        if (dy < 0) {
            swap(ye, ys);
            swap(xe, xs);
            dy = dy * -1;
            dx = dx * -1;
        }

        //decision
        int d = dy - abs(2 * dx);

        //calculate Initial P/N
        int P = -2 * abs(dx);
        int N = 2 * dy - abs(2 * dx);


        //set first pixel
        SetPixel(hdc, xs, ys, RGB(0, 0, 0));

        int y = ys;
        int x = xs;
        while (y < ye) {
            y++;
            if (d <= 0) {
                d += N;
                if (dx <= 0)
                    x -= 1;
                else
                    x += 1;
            } else
                d += P;
            SetPixel(hdc, x, y, RGB(0, 0, 0));
        }
    }
}


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

POINT calcIntersection(POINT s, double &slope, POINT edge) {

    if (edge.x == 0)
        edge.x = (long)((edge.y - s.y) / slope + s.x);

    if (edge.y == 0)
        edge.y =  (long)((edge.x - s.x) * slope + s.y);

    return edge;
}

POINT correctSides(POINT point, double &slope, vector<side> &startSides, vector<side> &endSides) {

    for (int i = 0; !startSides.empty(); ++i) {

        //check for common side.
        for (int i = 0; i < startSides.size(); i++)
            for (int j = 0; j < endSides.size(); j++)
                if (startSides[i] == endSides[j])
                    return make_point(-1,-1);

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
        startSides = getSides(point);
    }

    return point;
}

void drawClippedLine(HDC hdc, int xs, int ys, int xe, int ye) {
    POINT start = make_point(xs, ys);
    POINT end   = make_point(xe, ye);

    vector<side> startSides = getSides(start);
    vector<side> endSides   = getSides(end);


    double lineSlope = slope(start, end);

    //fix el start
    if (!startSides.empty())
        start = correctSides(start, lineSlope, startSides, endSides);

    //fix el end
    if (!endSides.empty())
        end = correctSides(end, lineSlope, endSides, startSides);

    //Line is out of range.
    if(start.x == -1 || end.x == -1)
        return;

    //finally Draw.
    drawLineMid(hdc, start.x, start.y, end.x, end.y);
}


void drawPolygon(HDC hdc, vector<point> points, int operation)
{
    int n = 0;
    if(operation == THREE_POINT)
        n = 3;
    if(operation == FOUR_POINT)
        n = 4;
    if(operation == FIVE_POINT)
        n = 5;
    if(operation == SIX_POINT)
        n = 6;

    int i;
    for (i = 0; i < n-1; ++i) {
        if(clipping)
            drawClippedLine(hdc, points[i].x, points[i].y, points[i+1].x, points[i+1].y);
        else
            drawLineMid(hdc, points[i].x, points[i].y, points[i+1].x, points[i+1].y);
    }
    if(clipping)
        drawClippedLine(hdc, points[i].x, points[i].y, points[0].x, points[0].y);
    else
        drawLineMid(hdc, points[i].x, points[i].y, points[0].x, points[0].y);
}

#endif //TASK_LINE_H
