//
// Created by Sherif on 30-Apr-18.
//

#ifndef TASK_CIRCLE_H
#define TASK_CIRCLE_H

#include <windows.h>
#include <cmath>

void draw8Points(HDC hdc, int x, int y, int xc, int yc)
{
    COLORREF color = RGB(0,0,0);
    SetPixel(hdc, x+xc, y+yc, color);
    SetPixel(hdc, -x+xc, y+yc, color);
    SetPixel(hdc, x+xc, -y+yc, color);
    SetPixel(hdc, -x+xc, -y+yc, color);
    SetPixel(hdc, y+xc, x+yc, color);
    SetPixel(hdc, -y+xc, x+yc, color);
    SetPixel(hdc, y+xc, -x+yc, color);
    SetPixel(hdc, -y+xc, -x+yc, color);
}

void drawCircleCart(HDC hdc, int xc, int yc, int xe, int ye)
{
    int dx = xe-xc;
    int dy = ye-yc;
    int R = sqrt(dx*dx + dy*dy);
    int x = 0, y = R;
    while(x<=y){
        draw8Points(hdc, x, y, xc, yc);
        x++;
        y = sqrt(R*R - x*x);
    }
}

void drawCircleMid(HDC hdc, int xc, int yc, int xe, int ye)
{
    int dx = xe-xc;
    int dy = ye-yc;
    int R = sqrt(dx*dx + dy*dy);
    int x = 0, y = R;
    int d = 1 - R;
    draw8Points(hdc, x, y, xc, yc);
    while(x<y)
    {
        if(d<=0)
        {
            d += 2*x + 3;
        }
        else
        {
            d+= 2*(x-y) + 5;
            y--;
        }
        x++;
        draw8Points(hdc, x, y, xc, yc);
    }

}

void drawCirclePolar(HDC hdc, int xc, int yc, int xe, int ye) {
    int dx = xe-xc;
    int dy = ye-yc;
    int R = sqrt(dx*dx + dy*dy);

    double dtheta = 1.0 / R;

    double x, y;

    double quad = M_PI / 4.0;

    for (double theta = 0; theta < quad; theta += dtheta) {
        x = R*cos(theta);
        y = R*sin(theta);
        draw8Points(hdc, round(x), round(y), xc, yc);
    }
}


#endif //TASK_CIRCLE_H
