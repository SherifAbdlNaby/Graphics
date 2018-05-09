//
// Created by Sherif on 30-Apr-18.
//

#ifndef TASK_LINE_H
#define TASK_LINE_H

#include "menu.h"
#include "UTIL.h"
#include <tchar.h>
#include <windows.h>
#include <cmath>
#include <stack>
#include <queue>
#include <string>

using namespace std;

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

#endif //TASK_LINE_H
