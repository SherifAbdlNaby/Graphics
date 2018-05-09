//
// Created by Sherif on 30-Apr-18.
//

#ifndef TASK_FILL_H
#define TASK_FILL_H
#include "menu.h"
#include <tchar.h>
#include <windows.h>
#include <cmath>
#include <stack>
#include <queue>
#include <string>

using namespace std;

POINT make_point(int x, int y)
{
    POINT point;
    point.x = x;
    point.y = y;
    return point;
}

bool isInWindowRange(int x, int y, int width, int height)
{
    //TO handle DFS (too lazy to make if conditions heh)
    if(x == -1 || y == -1)
        return false;

    return x < width && y < height;
}

void fillDfs(HWND hwnd, HDC hdc, int xs, int ys)
{
    COLORREF border = RGB(0,0,0);

    stack<POINT> points;

    POINT point = make_point(xs, ys);

    points.push(point);

    //START DFS
    while(!points.empty())
    {
        RECT rect;
        int width, height;

        //GET WINDOW
        if(GetClientRect(hwnd, &rect))
        {
            width = rect.right - rect.left;
            height = rect.bottom - rect.top;
        }
        else
            return;

        //pick point
        point = points.top();

        //remove from stack
        points.pop();


        if(!isInWindowRange(point.x, point.y, width, height))
            continue;

        //check if current pixel is a border.
        if(GetPixel(hdc, point.x, point.y) == border)
            continue;

        //draw color.
        SetPixel(hdc, point.x, point.y, border);

        //Add to stack all neighbour points
        points.push(make_point(point.x, point.y + 1));
        points.push(make_point(point.x + 1, point.y));
        points.push(make_point(point.x, point.y - 1));
        points.push(make_point(point.x - 1, point.y));
    }
}

void fillBfs(HWND hwnd, HDC hdc, int xs, int ys)
{
    COLORREF border = RGB(0,0,0);

    queue<POINT> points ;

    POINT point = make_point(xs, ys);

    points.push(point);

    //START BFS
    while(!points.empty())
    {
        RECT rect;
        int width, height;
        if(GetClientRect(hwnd, &rect))
        {
            width = rect.right - rect.left;
            height = rect.bottom - rect.top;
        }
        else
            return;

        //Pick from QUEUE
        point = points.front();

        //REMOVE
        points.pop();


        if(!isInWindowRange(point.x, point.y, width, height))
            continue;

        //check if current pixel is a border.
        if(GetPixel(hdc, point.x, point.y) == border)
            continue;

        //color it
        SetPixel(hdc, point.x, point.y, border);

        //Add to stack all neighbour points
        points.push(make_point(point.x, point.y + 1));
        points.push(make_point(point.x + 1, point.y));
        points.push(make_point(point.x, point.y - 1));
        points.push(make_point(point.x - 1, point.y));
    }
}


#endif //TASK_FILL_H
