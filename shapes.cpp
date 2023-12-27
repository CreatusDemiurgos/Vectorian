#include "shapes.h"


Point getPoint(LPARAM lParam){
    Point p;
    p.x = GET_X_LPARAM(lParam);
    p.y - GET_Y_LPARAM(lParam);
    return p;
}