#ifndef SHAPES_H
#define SHAPES_H
#include <Windows.h>
#include <windowsx.h>

struct Point{
    float x,y;
};

struct Line{
    Point start, end;
    float thickness;
};

Point getPoint(LPARAM lParam);


#endif