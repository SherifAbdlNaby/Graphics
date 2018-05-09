//
// Created by Sherif on 30-Apr-18.
//

#ifndef TASK_UTIL_H
#define TASK_UTIL_H

int Round(double v)
{
    return lround(v);
}

double slope(POINT s, POINT e) {
    int dx = e.x - s.x;
    int dy = e.y - s.y;
    double m = 0;
    if (dx != 0)
        m = (double) dy / dx;
    return m;
}

#endif //TASK_UTIL_H
