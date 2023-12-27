#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <windowsx.h>
#include <stdexcept>
#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")
#include "shapes.h"
#include "matrix.h"



struct StateInfo{};
enum MouseState{
    IDLE,
    CLICKED
};

MouseState mouseState = IDLE;

int numberOfClicks = 0;
int mouseX;
int mouseY;
int pointsYVec[4];
Matrix matrix1;

void drawLine();
double* creatingTheGraph();
void holdingDownLeftClick(LPARAM lParam, HWND handle);
LRESULT CALLBACK windowProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
inline StateInfo* getAppState(HWND handle);


// hPrevInstance is uselss but windows still has it for no reeason. It was used in 16bit windows.
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int cmdShow){
    const wchar_t CLASS_NAME[]  = L"Sample Window Class";

    WNDCLASSW wc{};
    wc.lpfnWndProc = windowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClassW(&wc);

    HWND handle = CreateWindowEx(0, L"Sample Window Class", L"Random name", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    if(handle == NULL){
        throw std::runtime_error("Failed to create the window!");
        }
        
    ShowWindow(handle, cmdShow);

    MSG msg{};
    while(GetMessage(&msg, NULL, 0, 0) > 0){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK windowProc(HWND handle, UINT uMsg, WPARAM wParam, LPARAM lParam){
    StateInfo *pState;
    if(uMsg == WM_CREATE){
        CREATESTRUCT *pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pState = reinterpret_cast<StateInfo*>(pCreate->lpCreateParams);
        SetWindowLongPtr(handle, GWLP_USERDATA, (LONG_PTR)pState);
        
    }else{
        pState = getAppState(handle);
    }

    switch(uMsg){
        case(WM_CLOSE):{
            if(MessageBox(handle, L"Ya really wanna quit?", L"Closing interface", MB_OKCANCEL) == IDOK){
                DestroyWindow(handle);
            } 
        }break;
        case(WM_PAINT):{
            /*PAINTSTRUCT ps;
            HDC hdc = BeginPaint(handle, &ps);
            //FillRect(hdc, &ps.rcPaint, (HBRUSH) ((COLOR_WINDOW) + 1));
            SetPixel(hdc, mouseX, mouseY, 0x000000);
            EndPaint(handle, &ps);*/
        }break;
        case(WM_DESTROY):{
            PostQuitMessage(0);
            return 0;
        }break;
        case(WM_LBUTTONDOWN):{
            mouseState = CLICKED;
            getPoint(lParam);
            numberOfClicks++;
            if(numberOfClicks == 2){
                //drawLine();
                numberOfClicks = 0;
            }
            mouseX = GET_X_LPARAM(lParam);
            mouseY = GET_Y_LPARAM(lParam);
            InvalidateRect(handle, NULL, FALSE);
        }break;
        case(WM_MOUSEMOVE):{
            holdingDownLeftClick(lParam, handle);
            mouseX = GET_X_LPARAM(lParam);
            mouseY = GET_Y_LPARAM(lParam);
            InvalidateRect(handle, NULL, FALSE);
        }break;
        case(WM_LBUTTONUP):{
            mouseState = IDLE;
        }break;
    }
    return DefWindowProc(handle, uMsg, wParam, lParam);
}


inline StateInfo* getAppState(HWND handle){
    LONG_PTR ptr = GetWindowLongPtr(handle, GWLP_USERDATA);
    StateInfo *pState = reinterpret_cast<StateInfo*>(ptr);
    return pState;

}

void holdingDownLeftClick(LPARAM lParam, HWND handle){
    static POINT lastPoint{0,0};
    if(mouseState == CLICKED){
        int i = 0;
        matrix1.creatingMatrix(4);
        while (i < 4) {
            if (mouseState == CLICKED) {
                POINT p{0, 0};
                GetCursorPos(&p);
                ScreenToClient(handle, &p);
                if(matrix1.matrix[((i-1)*4 + 1)] != p.x || i == 0){//checking  if the previous p.x saved in the previous row have different values. The second condition is necessary because i = 0 in the first condition causes
                //the index to become negative, thus causing an undefined behaviour to occur.
                    matrix1.addElementToMatrix(1, (i)*4 + 1);
                    matrix1.addElementToMatrix(p.x, (i)*4 + 2);
                    matrix1.addElementToMatrix(pow(p.x, 2), (i)*4 + 3);
                    matrix1.addElementToMatrix(pow(p.x, 3), (i)*4 + 4);
                    pointsYVec[i] = p.y;
                    i++;     
                }
            }
            if (i == 4) {
                int* posX;
                int* posY;
                double* cofactors = new double[4];
                HDC hdc = GetDC(handle);
                cofactors = creatingTheGraph();
                int sizeX = matrix1.matrix[(i-3)* (i) + 1] - matrix1.matrix[1];//gets the number of x and y where the polynomial will get applied
                posX = new int[sizeX];
                posY = new int[sizeX];
                for(int i = 0; i < sizeX; i++){
                    posX[i] = matrix1.matrix[1] + i;
                    posY[i] = cofactors[0] + cofactors[1] * posX[i] +  cofactors[2] * posX[i]*posX[i] +  cofactors[3] * posX[i] * posX[i] * posX[i];//polynomial of third degree
                    SetPixel(hdc, posX[i], posY[i], RGB(255, 190, 0));
                }
                ReleaseDC(handle, hdc);
                i = 0;
                break;  
            }  
        }
    }
}


double* creatingTheGraph(){
    int rowCount = matrix1.rowCount;
    long double* invMat = new long double[rowCount * rowCount];
    invMat = matrix1.inverseOfMatrix();
    double* resultVector = new double[rowCount];
    int k = 0;
    int j = 0;
    double cofactor = 0;
    for(int i = 0; i < (rowCount * rowCount); i++){
        cofactor += invMat[i] * pointsYVec[j];
        j++;
        if( (i % rowCount == (rowCount-1))){
            resultVector[k] = cofactor;
            j = 0;
            cofactor = 0;
            k++;
            
        }
    }
    //matrix1.deletingMatrix();
    return resultVector;
}

/*void drawLine(){
    std::cout << "Testing";
}*/
