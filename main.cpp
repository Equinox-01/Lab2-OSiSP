#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <vector>
#include <string>
#include <cmath>
#include "functions.h"

#define WINDOW_BORDER_SIZE 5
#define WINDOW_HEADER_SIZE 25
#define WORD_CAPACITY_CORRECTION 2

int data_width, data_height, strmaxlength;
RECT winRect;
std::vector<std::vector<std::string> > table_str;

void DrawTable(HWND hwnd);
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

TCHAR szClassName[ ] = _T("Lab2");

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    setlocale(LC_ALL, "");
    if (!GetDataFromFile("C:\\indata.txt", &table_str))
        return 1;
    data_width = GetStringMaxLength(&table_str) - 1;
    data_height = table_str.size();
    strmaxlength = GetElementMaxLength(&table_str) + WORD_CAPACITY_CORRECTION;
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) WHITE_PEN;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("Лабораторная работа №2 Вересковский С.В. гр. 551006"),       /* Title Text */
               WS_OVERLAPPEDWINDOW,//&(~WS_MAXIMIZEBOX), /* default window */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               640,                 /* The programs width */
               480,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );
    ShowWindow (hwnd, nCmdShow);
    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    return messages.wParam;
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
    case WM_SYSCOMMAND:
        if (wParam == SC_MAXIMIZE)
        {
            DefWindowProc (hwnd, message, wParam, lParam);
            InvalidateRect(hwnd,NULL, true);
            GetWindowRect(hwnd, &winRect);
            DrawTable(hwnd);

        }
        else
            return DefWindowProc (hwnd, message, wParam, lParam);
        break;
    case WM_ACTIVATE:
    case WM_SIZE:
        InvalidateRect(hwnd,NULL, true);
        GetWindowRect(hwnd, &winRect);
        DrawTable(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
void DrawBorderPart(HDC hdc, int start_x, int start_y, int end_x, int end_y)
{
        MoveToEx(hdc, start_x, start_y, NULL);
        LineTo(hdc, end_x, end_y);
}

void DrawTable(HWND hwnd)
{
    PAINTSTRUCT ps;
    HFONT oldFont,newFont;

    HDC hdc = BeginPaint(hwnd, &ps);
    int x_displacement = (int)floor((winRect.right - winRect.left - WINDOW_BORDER_SIZE)/data_width);
    int y_displacement = (int)floor((winRect.bottom - winRect.top - WINDOW_HEADER_SIZE - WINDOW_BORDER_SIZE)/data_height);
    newFont = CreateFont( y_displacement, x_displacement/strmaxlength, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "Arial");
    oldFont = (HFONT)SelectObject(hdc,newFont);

    for(int i = 0; i < table_str.size(); i++)
        for(int j = 0; j < table_str[i].size(); j++)
        {
            RECT tmp_rc;
            tmp_rc.left = j*x_displacement + 1;
            tmp_rc.top = i*y_displacement + 1;
            tmp_rc.right = j*x_displacement + x_displacement - 1;
            tmp_rc.bottom = i*y_displacement + y_displacement - 1;

            DrawBorderPart(hdc, x_displacement*j, y_displacement*i, x_displacement*j, y_displacement*i + y_displacement);                                         //Left
            DrawBorderPart(hdc, x_displacement*j + x_displacement, y_displacement*i, x_displacement*j + x_displacement, y_displacement*i + y_displacement);       //Right
            DrawBorderPart(hdc, x_displacement*j, y_displacement*i, x_displacement*j + x_displacement, y_displacement*i);                                         //Up
            DrawBorderPart(hdc, x_displacement*j, y_displacement*i + y_displacement, x_displacement*j + x_displacement, y_displacement*i + y_displacement);       //Down

            DrawText(hdc, (LPCSTR)table_str[i][j].c_str(), -1, &tmp_rc, DT_CENTER);
        }

    DeleteObject(oldFont);
    DeleteObject(newFont);
    EndPaint(hwnd, &ps);
}
