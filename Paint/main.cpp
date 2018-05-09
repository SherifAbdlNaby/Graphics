#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <cmath>
#include <fstream>
#include "menu.h"
#include "FILL.h"
#include "LINE.h"
#include "CIRCLE.h"
#include "CLIPPER.h"
#include "LOGS.h"

using namespace std;

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

TCHAR szClassName[] = _T("LineWindow");


int xs, ys, xe, ye;
bool interceptSecondClick = false;
int operation;
int currentOperation = -1;
bool clipping = false;

void drawCurrentOperation(HWND &hwnd, PAINTSTRUCT &p){
    BeginPaint(hwnd, &p);

    if (LINE_DDA == currentOperation)
        if (clipping)
            drawClipLine(p.hdc, xs, ys, xe, ye);
        else
            drawLineDDA(p.hdc, xs, ys, xe, ye);
    else if (LINE_MIDPOINT == currentOperation)
        if (clipping)
            drawClipLine(p.hdc, xs, ys, xe, ye);
        else
            drawLineMid(p.hdc, xs, ys, xe, ye);
    else if (CIRCLE_CART == currentOperation)
        drawCircleCart(p.hdc, xs, ys, xe, ye);
    else if (CIRCLE_MIDPOINT == currentOperation)
        drawCircleMid(p.hdc, xs, ys, xe, ye);
    else if (FILL_BFS == currentOperation)
        fillBfs(hwnd, p.hdc, xs, ys);
    else if (FILL_DFS == currentOperation)
        fillDfs(hwnd, p.hdc, xs, ys);
    else if (LINE_CLIP_ENABLE == currentOperation)
        makeClippingRect(p.hdc, xs, ys, xe, ye), clipping = true;
    else if (LINE_CLIP_DISABLE == currentOperation)
        clipping = false;
    else return;

    EndPaint(hwnd, &p);
}

void drawOperationsLog(HWND &hwnd, PAINTSTRUCT &p){
    for (int i = 0; i < logs.size(); ++i) {
        xs = logs[i].xs;
        xe = logs[i].xe;
        ys = logs[i].ys;
        ye = logs[i].ye;
        currentOperation = logs[i].operation;
        InvalidateRect(hwnd, NULL, FALSE);
        drawCurrentOperation(hwnd, p);
    }
    //reset operation
    currentOperation = -1;
}

int WINAPI WinMain(HINSTANCE hThisInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpszArgument,
                   int nCmdShow) {
    HWND hwnd;
    MSG messages;

    WNDCLASSEX wincl;


    wincl.hInstance = hThisInstance;        // Handle to the application instance
    wincl.lpszClassName = szClassName;      // The name of the window class to identify it
    wincl.lpfnWndProc = WindowProcedure;    // The window procedure method name (pointer to the window procedure)
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */ // CS* is the class style
    wincl.cbSize = sizeof(WNDCLASSEX);        // The size of the stucture

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION); // The large icon shown for the window when the user presses ALT+TAB
    wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);// The small icon shown in the task bar for this window
    wincl.hCursor = LoadCursor(NULL, IDC_ARROW);   // the cursor used (IDC_ARROW) the standard arrow cursor
    wincl.lpszMenuName = NULL;                 // The name of a menu resource to be used for the window (no menu in our case)
    wincl.cbClsExtra = 0;                      // Amount of extra data allocated for this class in memory. Usually 0.
    wincl.cbWndExtra = 0;                      // Amount of extra data allocated in memory per window of this type. Usually 0.
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;    // The background brush to set background color for the window GetStockObject(WHITE_BRUSH);

    /* Register the window class, and if it fails quit the program */
    // Register the window class to be able to associate it with windows
    if (!RegisterClassEx(&wincl))
        return 0;

    HMENU menu = LoadMenu(NULL, MAKEINTRESOURCE(MENU_ID));

    hwnd = CreateWindowEx(
            0,//WS_EX_CLIENTEDGE,                   // Window style for borders
            szClassName,                            // Name of the class registered above for windows of this app
            _T("Paint"),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            544,
            375,
            HWND_DESKTOP,
            menu,
            hThisInstance,
            NULL
    );

    ShowWindow(hwnd, nCmdShow);

    while (GetMessage(&messages, NULL, 0, 0)) {

        TranslateMessage(&messages);

        DispatchMessage(&messages);
    }

    return messages.wParam;
}


LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT p;
    int x, y;
    switch (message) {
        case WM_COMMAND:
            operation = LOWORD(wParam);
            switch (operation) {
                case EXIT_ID:
                    PostQuitMessage(0);
                    break;
                case SAVE_ID:
                    saveOperationsLog();
                    break;
                case LOAD_ID:
                    readOperationsLog();
                    drawOperationsLog(hwnd, p);
                    break;
                default:
                    currentOperation = operation;
                    interceptSecondClick = false;
                    break;
            }
            break;
        case WM_PAINT:
            drawCurrentOperation(hwnd, p);

            //LOG DATA FOR SAVE
            addToOperationsLogs(operationLog(xs, ys, xe, ye, currentOperation));

            EndPaint(hwnd, &p);
            break;
        case WM_LBUTTONDOWN:
            x = LOWORD(lParam);
            y = HIWORD(lParam);

            if (interceptSecondClick) {
                xe = x;
                ye = y;
                interceptSecondClick = false;
            } else {
                xs = x;
                ys = y;

                //NEED TWO CLICKS.
                if (currentOperation != FILL_DFS &&
                        currentOperation != FILL_BFS &&
                        currentOperation != LINE_CLIP_DISABLE)
                {
                    interceptSecondClick = true;
                    break;
                }
            }
            InvalidateRect(hwnd, NULL, FALSE);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}
