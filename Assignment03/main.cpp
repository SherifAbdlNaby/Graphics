#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif
#include "bar.h"
#include <tchar.h>
#include <windows.h>
#include <cmath>
#include <algorithm>

LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

TCHAR szClassName[ ] = _T("LineWindow");

using namespace std;


int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;
    MSG messages;

    WNDCLASSEX wincl;


    wincl.hInstance = hThisInstance;        // Handle to the application instance
    wincl.lpszClassName = szClassName;      // The name of the window class to identify it
    wincl.lpfnWndProc = WindowProcedure;    // The window procedure method name (pointer to the window procedure)
    wincl.style  = CS_DBLCLKS;
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon =   LoadIcon (NULL, IDI_APPLICATION);   // The large icon shown for the window when the user presses ALT+TAB
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);   // The small icon shown in the task bar for this window
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);       // the cursor used (IDC_ARROW) the standard arrow cursor
    wincl.lpszMenuName = NULL;                          // The name of a menu resource to be used for the window (no menu in our case)
    wincl.cbClsExtra = 0;                               // Amount of extra data allocated for this class in memory. Usually 0.
    wincl.cbWndExtra = 0;                               // Amount of extra data allocated in memory per window of this type. Usually 0.

    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;    // The background brush to set background color for the window GetStockObject(WHITE_BRUSH);

    /* Register the window class, and if it fails quit the program */
    // Register the window class to be able to associate it with windows
    if (!RegisterClassEx (&wincl))
        return 0;

    HMENU menu = LoadMenu(NULL, MAKEINTRESOURCE(MENU_ID));

    //Initial window
    hwnd = CreateWindowEx (
           0,//WS_EX_CLIENTEDGE,                   // Window style for borders
           szClassName,                            // Name of the class registered above for windows of this app
           _T("Window"),
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


    ShowWindow (hwnd, nCmdShow);


    while (GetMessage (&messages, NULL, 0, 0))
    {

        TranslateMessage(&messages);

        DispatchMessage(&messages);
    }


    return messages.wParam;
}

void drawLineDDA(HDC hdc, int xS, int yS, int xE, int yE) {
    int dx = xE - xS;
	int dy = yE - yS;
	float m = (float)dy / dx;

	// If it's a dot
	if (!dx && !dy) {
		SetPixel(hdc, xS, yS, RGB(0,0,0));
		return;
	}

	//X -> Unit intervals, Y -> M
	if (m <= 1)
	{
		//Step
		int unitStep = dx > 0 ? 1 : -1;

		//Draw
		int x = xS;
		float y = yS;
		while (x != xE)
		{
			x += unitStep;
			y += m * unitStep;
			SetPixel(hdc, x, round(y), RGB(0,0,0));
		}
	}

	//Y -> Unit intervals, X -> 1/M
	else
	{
		//Unit Step
		int unitStep = dy > 0 ? 1 : -1;


		//Draw
		float x = xS;
		int y = yS;
		while (y != yE)
		{
			x += 1 / m * unitStep;
			y += unitStep;
			SetPixel(hdc, round(x), y, RGB(0,0,0));
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

void draw8Points(HDC hdc, int x, int y, int xc, int yc){
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

void drawCircleCart(HDC hdc, int xc, int yc, int xe, int ye){
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

int xs, ys, xe, ye;
int clicks = 0;
int algorithmBar = -1;
int chosenAlgorithm = -1;
int xCord;
int yCord;

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT p;
    switch (message)
    {
        case WM_COMMAND:
            algorithmBar = LOWORD(wParam);
            switch(algorithmBar)
            {
                case EXIT_ID:
                    PostQuitMessage(0);
                    break;
                case DDA:
                    chosenAlgorithm = 1;
                    break;
                case MIDPOINT_LINE:
                    chosenAlgorithm = 2;
                    break;
                case CARTESIAN_CIRCLE:
                    chosenAlgorithm = 3;
                    break;
            }
        case WM_PAINT:
            BeginPaint(hwnd, &p);
            if(chosenAlgorithm == 1)
                drawLineDDA(p.hdc, xs, ys, xe, ye);
            if(chosenAlgorithm == 2)
                drawLineMid(p.hdc, xs, ys, xe, ye);
            if(chosenAlgorithm == 3)
                drawCircleCart(p.hdc, xs, ys, xe, ye);
            EndPaint(hwnd, &p);
            break;
        case WM_LBUTTONDOWN:
            xCord = LOWORD(lParam);
            yCord = HIWORD(lParam);
            if(clicks == 0){
                xs = xCord;
                ys = yCord;
                clicks++;
            } else {
                xe = xCord;
                ye = yCord;
                //Reset el clicks
                clicks = 0;
                InvalidateRect(hwnd,NULL,FALSE);
            }
            break;
        case WM_DESTROY:
            PostQuitMessage (0);
            break;
        default:
            return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return 0;
}
