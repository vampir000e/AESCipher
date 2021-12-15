#define WIN32_LEAN_AND_MEAN

#define UNICODE
#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#ifdef UNICODE
#define CF_TCHAR    CF_UNICODETEXT
#else
#define CF_TCHAR    CF_TEXT
#endif // UNICODE

#include "resource.h"
#include "aes.h"
#include "head.h"

/* int WINAPI WinMain() 相当于 int main()*/
/*                 实例句柄  当前句柄名  */
/* 三种大写句柄：                        */
/* HINSTANCE：实例句柄——程序本身       */
/* HWND     ：窗口句柄                   */
/* HDC      ：设备环境句柄               */
int WINAPI WinMain(HINSTANCE hThisInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpszArgument,
                   int nCmdShow)
{
    /* 将 hPrevInstance 等参数“假用”一次，避免编译时候的警告 */
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpszArgument);

    HACCEL hAccelTable;

    MSG messages;            /* Here messages to the application are saved */

    // 初始化全局字符串
    LoadString(hThisInstance, IDC_MAIN, szWindowClass, MAX_LOADSTRING);

    /* Register the window class, and if it fails quit the program */
    MyRegisterClass(hThisInstance);

   // 执行应用程序初始化:
    if (!InitInstance(hThisInstance, nCmdShow))
    {
        return FALSE;
    }

     hAccelTable = LoadAccelerators(hThisInstance, MAKEINTRESOURCE(IDC_MAIN));
     //hAccel = LoadAccelerators(hInstance, szAppName);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    //主消息循环
    while (GetMessage(&messages, NULL, 0, 0))
    {
        if (!TranslateAccelerator(messages.hwnd, hAccelTable, &messages))
        {
            /* Translate virtual-key messages into character messages */
            TranslateMessage(&messages);
            /* Send message to WindowProcedure */
            DispatchMessage(&messages);
        }
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return (int)messages.wParam;
}



//函数:  MyRegisterClass()
//目的:  注册窗口类。
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wincl;
    /* The Window structure */
    wincl.hInstance     = hInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc   = WindowProcedure;  /* This function is called by windows */
    wincl.style         = CS_HREDRAW | CS_VREDRAW;  //窗口移动和尺寸调整时重绘
    wincl.cbSize        = sizeof(WNDCLASSEX);

    /* change icon and use default mouse-pointer */
    wincl.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HELLOWIN));
    wincl.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));

    wincl.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wincl.lpszMenuName  = MAKEINTRESOURCE(IDC_MAIN); /* No menu */
    wincl.cbClsExtra    = 0;    /* No extra bytes after the window class */
    wincl.cbWndExtra    = 0;    /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    //wincl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    //change the color of background
    //wincl.hbrBackground  = (HBRUSH)(COLOR_BTNFACE + 1);
    /* 将客户区背景颜色设置为按钮表面颜色 */
    wincl.hbrBackground  = CreateSolidBrush(GetSysColor(COLOR_BTNFACE)); //(HBRUSH)(COLOR_WINDOW + 1);

    return RegisterClassEx(&wincl);
}


//函数:  InitInstance(HINSTANCE, int)
//目的:  保存实例句柄并创建主窗口
//在此函数中，我们在全局变量中保存实例句柄并创建和显示主程序窗口。
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hwnd;
    hInst = hInstance; // 将实例句柄存储在全局变量中

    hwnd = CreateWindowEx(
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("AES算法加解密"),       /* Title Text */
               WS_OVERLAPPEDWINDOW, /* default window */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               1000,                 /* The programs width */
               650,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );

    if (!hwnd)
    {
        return FALSE;
    }
    /* Make the window visible on the screen */
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    return TRUE;
}

/*  This function is called by the Windows function DispatchMessage()  */
//  函数:  WndProc(HWND, UINT, WPARAM, LPARAM)
//  目的:    处理主窗口的消息。
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    HDC         hdc;
    static int  cxClient, cyClient;
    PAINTSTRUCT ps;

    RECT            rect;

    int    wmId, wmEvent;


    static int      cxChar, cyChar;

    TEXTMETRIC      tm;


    HBRUSH          hBrushOld;

    static HFONT    hOrgFont, hFont;


    static TCHAR  * pText;

    static HWND hRadioButton1[3];//密钥长度的三个选项
    static HWND hRadioButton2[3];//源数据类型的三个选项
    static HWND hSetCheck;
    static HWND hSetUncheck;
    int i;

    static HWND hBanPick;
    static HWND hShowHide;

    static HWND hwndKeyLable;
    static HWND hwndKeyEdit;
    static HWND hwndSourceDataLabel;
    static HWND hwndSourceDataEdit;
    static HWND hwndResultLabel;
    static HWND hwndInvResultLabel;
    static HWND hwndResultEdit;
    static HWND hwndInvResultEdit;

    static HWND hPutKey;
    static HWND hSaveKey;
    static HWND hClear;
    static HWND hPutPlaintext;
    static HWND hSavePlaintext;
    static HWND hFileOpen;
    static HWND hFileSave;
    static HWND hFileNameLabel;
    static HWND hContentClear;
    static HWND hResultClear;

    static HWND hCipher;
    static HWND hwndGroup,hwndGroup1,hwndGroup2,hwndGroup3;

    static BOOL bNeedSave = FALSE;
    static TCHAR  szFileName[MAX_PATH], szTitleName[MAX_PATH];
    static UINT      messageFindReplace;

    static TCHAR pszBuffer[1024];

    static TCHAR pszBuffer2[1024];

    string pre;
    wchar_t sztitle[100];
    wchar_t plainsztitle[100];

    vector<string>::iterator iter;

    wchar_t tempStr[32];

    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
            {
                hdc = GetDC(hwnd);
                SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
                GetTextMetrics(hdc, &tm);
                cxChar = tm.tmAveCharWidth;
                cyChar = tm.tmHeight + tm.tmExternalLeading;
                ReleaseDC(hwnd, hdc);

                /* 创建一个包围住三个按钮的方框子窗口，方框左上角显示文本：密钥长度 */
                hwndGroup = CreateWindow(TEXT("button"), TEXT("密钥长度"),
                                         BS_GROUPBOX | WS_CHILD | WS_VISIBLE,
    //                                     cxChar / 4, 0,
    //                                     21 * cxChar, 9 * cyChar,
                                         0,0,0,0,
                                         hwnd, 0,
                                         hInst,
                                         NULL
                                        );
                /* 创建一个包围住三个按钮的方框子窗口，方框左上角显示文本：加解密对象 */
                hwndGroup1 = CreateWindow(TEXT("button"), TEXT("加/解密对象"),
                                         BS_GROUPBOX | WS_CHILD | WS_VISIBLE,
    //                                     cxChar / 4, 0,
    //                                     21 * cxChar, 9 * cyChar,
                                         0,0,0,0,
                                         hwnd, 0,
                                         hInst,
                                         NULL
                                        );
                /* 创建一个包围住三个按钮的方框子窗口，方框左上角不显示文本  */
                hwndGroup2 = CreateWindow(TEXT("button"), TEXT(""),
                                         BS_GROUPBOX | WS_CHILD | WS_VISIBLE,
    //                                     cxChar / 4, 0,
    //                                     21 * cxChar, 9 * cyChar,
                                         0,0,0,0,
                                         hwnd, 0,
                                         hInst,
                                         NULL
                                        );
                /* 创建一个包围住三个按钮的方框子窗口，方框左上角不显示文本  */
                hwndGroup3 = CreateWindow(TEXT("button"), TEXT("加/解密运算"),
                                         BS_GROUPBOX | WS_CHILD | WS_VISIBLE,
    //                                     cxChar / 4, 0,
    //                                     21 * cxChar, 9 * cyChar,
                                         0,0,0,0,
                                         hwnd, 0,
                                         hInst,
                                         NULL
                                        );

                /* 创建按钮子窗口，并将句柄赋值给 hButton */
                hRadioButton1[0] = CreateWindow(TEXT("button"),
                                            TEXT("128bit"),
                                            WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
    //                                        cxChar * 60, cyChar * 5,
    //                                        cxChar * 20, cyChar * 2,
                                            0,0,0,0,
                                            hwnd,
                                            (HMENU)ID_RADIOBUTTON1,   //子窗口按钮编号
                                            hInst,
                                            NULL
                                           );
                hRadioButton1[1] = CreateWindow(TEXT("button"),
                                            TEXT("192bit"),
                                            WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
                                            0,0,0,0,
                                            hwnd,
                                            (HMENU)ID_RADIOBUTTON2,   //子窗口按钮编号
                                            hInst,
                                            NULL
                                           );
                EnableWindow(hRadioButton1[1], FALSE); //默认禁用此按钮
                hRadioButton1[2] = CreateWindow(TEXT("button"),
                                            TEXT("256bit"),
                                            WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
                                            0,0,0,0,
                                            hwnd,
                                            (HMENU)ID_RADIOBUTTON3,   //子窗口按钮编号
                                            hInst,
                                            NULL
                                           );
                EnableWindow(hRadioButton1[2], FALSE); //默认禁用此按钮

                hRadioButton2[1] = CreateWindow(TEXT("button"),
                                            TEXT("字符串"),
                                            WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
    //                                        cxChar * 60, cyChar * 5,
    //                                        cxChar * 20, cyChar * 2,
                                            0,0,0,0,
                                            hwnd,
                                            (HMENU)ID_RADIOBUTTON5,   //子窗口按钮编号
                                            hInst,
                                            NULL
                                           );
                hRadioButton2[0] = CreateWindow(TEXT("button"),
                                            TEXT("16进制"),
                                            WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
                                            0,0,0,0,
                                            hwnd,
                                            (HMENU)ID_RADIOBUTTON4,   //子窗口按钮编号
                                            hInst,
                                            NULL
                                           );
                hRadioButton2[2] = CreateWindow(TEXT("button"),
                                            TEXT("文件"),
                                            WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
                                            0,0,0,0,
                                            hwnd,
                                            (HMENU)ID_RADIOBUTTON6,   //子窗口按钮编号
                                            hInst,
                                            NULL
                                           );
                hSetCheck = CreateWindow(TEXT("button"),
                                         TEXT("选中默认"),
                                         WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                         0,0,0,0,
                                         hwnd,
                                         (HMENU)ID_SETCHECK,   //子窗口按钮编号
                                         hInst,
                                         NULL
                                        );

                hSetUncheck = CreateWindow(TEXT("button"),
                                           TEXT("清除"),
                                           WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                           0,0,0,0,
                                           hwnd,
                                           (HMENU)ID_SETUNCHECK,   //子窗口按钮编号
                                           hInst,
                                           NULL
                                           );
                hBanPick = CreateWindow(TEXT("button"),
                             TEXT("禁用 / 启用 "),
                             WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                             0,0,0,0,
                             hwnd,
                             (HMENU)ID_BANPICK,   // 16 是子窗口按钮编号
                             hInst,
                             NULL
                            );
                hShowHide =  CreateWindow(TEXT("button"),
                             TEXT("Show / Hide "),
                             WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                             0,0,0,0,
                             hwnd,
                             (HMENU)ID_SHOWHIDE,   // 15 是子窗口按钮编号
                             hInst,
                             NULL
                            );

                hwndKeyLable = CreateWindow(TEXT("static"), TEXT("密钥: (0-9、a-f、A-F)"),
                                                 WS_CHILD | WS_VISIBLE | SS_LEFT| SS_CENTERIMAGE ,
                                                 0, 0,
                                                 0, 0,
                                                 hwnd,
                                                 (HMENU)ID_KEY_LABEL,
                                                 ((LPCREATESTRUCT)lParam)->hInstance,
                                                 NULL
                                                );
                /* ES_LEFT 是左对齐的编辑类 */
                hwndKeyEdit = CreateWindowEx(WS_EX_CLIENTEDGE,
                                            TEXT("edit"), NULL,
                                            WS_CHILD | WS_VISIBLE | ES_LEFT | ES_MULTILINE,
    //                                                WS_VSCROLL | WS_HSCROLL |
    //                                                ES_AUTOVSCROLL | ES_AUTOHSCROLL,
                                            0, 0,
                                            0, 0,
                                            hwnd,
                                            (HMENU)ID_KEY_EDIT,
                                            ((LPCREATESTRUCT)lParam)->hInstance,
                                            NULL
                                            );

                hwndSourceDataLabel = CreateWindow(TEXT("static"), TEXT("明文："),
                                                 WS_CHILD | WS_VISIBLE | SS_LEFT| SS_CENTERIMAGE ,
                                                 0, 0,
                                                 0, 0,
                                                 hwnd,
                                                 (HMENU)ID_SOURCEDATA_LABEL,
                                                 ((LPCREATESTRUCT)lParam)->hInstance,
                                                 NULL
                                                    );

                hwndSourceDataEdit = CreateWindowEx(WS_EX_CLIENTEDGE,
                                                    TEXT("edit"), NULL,
                                                    WS_CHILD | WS_VISIBLE |
                                                    ES_LEFT | ES_MULTILINE |
                                                    WS_VSCROLL | WS_HSCROLL |
                                                    ES_AUTOVSCROLL | ES_AUTOHSCROLL,
                                                    0, 0,
                                                    0, 0,
                                                    hwnd,
                                                    (HMENU)ID_SOURCEDATA_EDIT,
                                                    ((LPCREATESTRUCT)lParam)->hInstance,
                                                    NULL
                                                    );
                hwndResultLabel = CreateWindow(TEXT("static"), TEXT("加密结果："),
                                                 WS_CHILD | WS_VISIBLE | SS_LEFT| SS_CENTERIMAGE ,
                                                 0, 0,
                                                 0, 0,
                                                 hwnd,
                                                 (HMENU)ID_RESULT_LABEL,
                                                 ((LPCREATESTRUCT)lParam)->hInstance,
                                                 NULL
                                                );

                hwndInvResultLabel = CreateWindow(TEXT("static"), TEXT("解密结果："),
                                                 WS_CHILD | WS_VISIBLE | SS_LEFT| SS_CENTERIMAGE ,
                                                 0, 0,
                                                 0, 0,
                                                 hwnd,
                                                 (HMENU)ID_INVRESULT_LABEL,
                                                 ((LPCREATESTRUCT)lParam)->hInstance,
                                                 NULL
                                                );
                hwndResultEdit = CreateWindowEx(WS_EX_CLIENTEDGE,
                                                TEXT("edit"), NULL,
                                                WS_CHILD | WS_VISIBLE | ES_LEFT | ES_MULTILINE,
                                                0, 0,
                                                0, 0,
                                                hwnd,
                                                (HMENU)ID_RESULT_EDIT,
                                                ((LPCREATESTRUCT)lParam)->hInstance,
                                                NULL
                                                );
                hwndInvResultEdit = CreateWindowEx(WS_EX_CLIENTEDGE,
                                                TEXT("edit"), NULL,
                                                WS_CHILD | WS_VISIBLE | ES_LEFT | ES_MULTILINE,
                                                0, 0,
                                                0, 0,
                                                hwnd,
                                                (HMENU)ID_INVRESULT_EDIT,
                                                ((LPCREATESTRUCT)lParam)->hInstance,
                                                NULL
                                                );
                hPutKey = CreateWindow(TEXT("button"), TEXT("使用示例密钥"),
                          WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                          0,0,0,0,
                          hwnd,
                          (HMENU)ID_PUTKEY,
                          ((LPCREATESTRUCT)lParam)->hInstance,
                          NULL
                          );
                hSaveKey = CreateWindow(TEXT("button"), TEXT("更新密钥"),
                          WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                          0,0,0,0,
                          hwnd,
                          (HMENU)ID_SAVEKEY,
                          ((LPCREATESTRUCT)lParam)->hInstance,
                          NULL
                          );
                hClear = CreateWindow(TEXT("button"), TEXT("清除"),
                                      WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                      0,0,0,0,
                                      hwnd,
                                      (HMENU)ID_CLEAR,
                                      ((LPCREATESTRUCT)lParam)->hInstance,
                                      NULL
                                     );

                hPutPlaintext = CreateWindow(TEXT("button"), TEXT("放置示例明文"),
                                        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                        0,0,0,0,
                                        hwnd,
                                        (HMENU)ID_PUTPLAINTEXT,
                                        hInst,
                                        NULL
                                        );
                hSavePlaintext = CreateWindow(TEXT("button"), TEXT("更新明文"),
                                        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                        0,0,0,0,
                                        hwnd,
                                        (HMENU)ID_SAVEPLAINTEXT,
                                        hInst,
                                        NULL
                                        );
                hFileOpen = CreateWindow(TEXT("button"), TEXT("open file"),
                                        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                        0,0,0,0,
                                        hwnd,
                                        (HMENU)ID_FILEOPEN,
                                        hInst,
                                        NULL
                                        );

                hFileSave = CreateWindow(TEXT("button"), TEXT("save as"),
                                        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                        0,0,0,0,
                                        hwnd,
                                        (HMENU)ID_FILESAVE,
                                        hInst,
                                        NULL
                                        );


                hFileNameLabel = CreateWindow(TEXT("static"), TEXT(""),
                                              WS_CHILD | WS_VISIBLE | SS_LEFT,
                                              0, 0,
                                              0, 0,
                                              hwnd,
                                              (HMENU)ID_FILENAME_LABEL,
                                              ((LPCREATESTRUCT)lParam)->hInstance,
                                               NULL
                                               );
                hContentClear = CreateWindow(TEXT("button"), TEXT("清除"),
                                            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                            0,0,0,0,
                                            hwnd,
                                            (HMENU)ID_CLEARCONTENT,
                                            hInst,
                                            NULL
                                            );
                hCipher =  CreateWindow(TEXT("button"), TEXT("加密并解密"),
                                        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                        0,0,0,0,
                                        hwnd,
                                        (HMENU)ID_CIPHER,
                                        hInst,
                                        NULL
                                        );

                hResultClear =  CreateWindow(TEXT("button"), TEXT("清空全部"),
                                        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                        0,0,0,0,
                                        hwnd,
                                        (HMENU)ID_RESULTCLEAR,
                                        hInst,
                                        NULL
                                        );

                // Initialize common dialog box stuff
                PopFileInitialize(hwnd);
                PopFontInitialize(hwndSourceDataEdit);

                messageFindReplace = RegisterWindowMessage(FINDMSGSTRING);

//////////////////////////////调整好的
                hFont = (HFONT)GetStockObject(SYSTEM_FIXED_FONT);
                return 0;
            }
        case WM_SIZE:
            {
                cxClient = LOWORD(lParam);
                cyClient = HIWORD(lParam);
                 /* 摆放文本框位置和设定大小 */
                MoveWindow(hRadioButton1[0], 0+cxClient/20,cyClient/15, 8 * cxChar, 1.5 * cyChar,
                           TRUE);
                MoveWindow(hRadioButton1[1], 0+cxClient/20+8*cxChar+cxClient/40,cyClient/15, 8 * cxChar, 1.5 * cyChar,
                           TRUE);
                MoveWindow(hRadioButton1[2], 0+cxClient/20+2*8*cxChar+2*cxClient/40,cyClient/15, 8 * cxChar, 1.5 * cyChar,
                           TRUE);

                MoveWindow(hRadioButton2[0], cxClient/2+cxClient/20,cyClient/15, 10 * cxChar, 1.5 * cyChar,
                           TRUE);
                MoveWindow(hRadioButton2[1], cxClient/2+cxClient/20+10*cxChar+cxClient/40,cyClient/15, 10 * cxChar, 1.5 * cyChar,
                           TRUE);
                MoveWindow(hRadioButton2[2], cxClient/2+cxClient/20+2*10*cxChar+2*cxClient/40,cyClient/15, 10 * cxChar, 1.5 * cyChar,
                           TRUE);

                MoveWindow(hSetCheck, cxClient/20,cyClient/15+cyClient/15, 10 * cxChar, 2 * cyChar,
                           TRUE);
                MoveWindow(hSetUncheck, cxClient/20+10*cxChar+cxClient/20,cyClient/15+cyClient/15, 10 * cxChar, 2 * cyChar,
                           TRUE);
                MoveWindow(hBanPick, cxClient/20+2*10*cxChar+2*cxClient/20,cyClient/15+cyClient/15, 10 * cxChar, 2 * cyChar,
                           TRUE);
                MoveWindow(hShowHide, cxClient/20+3*10*cxChar+3*cxClient/20,cyClient/15+cyClient/15, 10 * cxChar, 2 * cyChar,
                           TRUE);

                //密钥
                MoveWindow(hwndKeyLable, cxClient/20+cxClient/80,cyClient/15+cyClient/7+2*cyChar, 20 * cxChar, 2 * cyChar,
                           TRUE);
                MoveWindow(hwndKeyEdit, cxClient/20+cxClient/80,cyClient/15+cyClient/7+2*cyChar+cyClient/18, 60 * cxChar, 2 * cyChar,
                           TRUE);
                MoveWindow(hwndSourceDataLabel, cxClient/20+cxClient/80,cyClient/15+cyClient/7+2*2*cyChar+cyClient/18, 20 * cxChar, 2 * cyChar,
                           TRUE);
                MoveWindow(hwndSourceDataEdit, cxClient/20+cxClient/80,cyClient/15+cyClient/7+3*2*cyChar+cyClient/18, 60 * cxChar,  6 * cyChar,
                           TRUE);
                MoveWindow(hwndResultLabel, cxClient/20+cxClient/80,cyClient/15+cyClient/7+3*2*cyChar+6*cyChar+cyClient/18, 20 * cxChar, 2 * cyChar,
                           TRUE);
                MoveWindow(hwndResultEdit, cxClient/20+cxClient/80,cyClient/15+cyClient/7+4*2*cyChar+6*cyChar+cyClient/18, 60 * cxChar, 4 * cyChar,
                           TRUE);
                MoveWindow(hwndInvResultLabel, cxClient/20+cxClient/80,cyClient/15+cyClient/7+4*2*cyChar+6*cyChar+3*cyClient/18, 20 * cxChar, 2 * cyChar,
                           TRUE);
                MoveWindow(hwndInvResultEdit, cxClient/20+cxClient/80,cyClient/15+cyClient/7+6*2*cyChar+4*cyChar+3*cyClient/18, 60 * cxChar, 4 * cyChar,
                           TRUE);

                MoveWindow(hPutKey, cxClient/20+2*cxClient/80+60*cxChar,cyClient/15+cyClient/7+2*cyChar+cyClient/18, 13 * cxChar, 2 * cyChar,
                           TRUE);
                MoveWindow(hSaveKey, cxClient/20+3*cxClient/80+60*cxChar+13*cxChar,cyClient/15+cyClient/7+2*cyChar+cyClient/18, 13 * cxChar, 2 * cyChar,
                           TRUE);
                MoveWindow(hClear, cxClient/20+4*cxClient/80+60*cxChar+2*13*cxChar,cyClient/15+cyClient/7+2*cyChar+cyClient/18, 13* cxChar, 2 * cyChar,
                           TRUE);

                MoveWindow(hPutPlaintext, cxClient/20+2*cxClient/80+60*cxChar,cyClient/15+cyClient/7+2*2*cyChar+2*cyClient/18, 13 * cxChar, 2 * cyChar,
                           TRUE);
                MoveWindow(hSavePlaintext, cxClient/20+3*cxClient/80+60*cxChar+13*cxChar,cyClient/15+cyClient/7+2*2*cyChar+2*cyClient/18, 13 * cxChar, 2 * cyChar,
                           TRUE);
                MoveWindow(hContentClear, cxClient/20+4*cxClient/80+60*cxChar+2*13*cxChar,cyClient/15+cyClient/7+2*2*cyChar+2*cyClient/18, 13 * cxChar, 2 * cyChar,
                           TRUE);

                MoveWindow(hFileOpen, cxClient/20+2*cxClient/80+60*cxChar,cyClient/15+cyClient/7+2*2*cyChar+3*cyClient/18+cyClient/40, 13 * cxChar, 2 * cyChar,
                           TRUE);

                MoveWindow(hFileNameLabel, cxClient/20+3*cxClient/80+60*cxChar+13*cxChar,cyClient/15+cyClient/7+2*2*cyChar+3*cyClient/18+cyClient/40, 30 * cxChar, 2 * cyChar,
                           TRUE);

                //MoveWindow(hFileSave, cxClient/20+2*cxClient/80+60*cxChar,cyClient/15+cyClient/7+2*2*cyChar+4*cyClient/18, 13 * cxChar, 2 * cyChar,
                           //TRUE);

                MoveWindow(hCipher, cxClient/20+2*cxClient/80+60*cxChar,cyClient/15+cyClient/7+3*2*cyChar+5*cyClient/18, 20 * cxChar, 2 * cyChar,
                           TRUE);
                MoveWindow(hResultClear, cxClient/20+3*cxClient/80+60*cxChar+20*cxChar,cyClient/15+cyClient/7+3*2*cyChar+5*cyClient/18, 20 * cxChar, 2 * cyChar,
                           TRUE);


                MoveWindow(hwndGroup,cxClient/20-cxClient/40,cyClient/26,cxClient/2-cxClient/20, 3* cyChar,
                           TRUE);
                MoveWindow(hwndGroup1,cxClient/2-cxClient/40,cyClient/26,cxClient/2-cxClient/20, 3* cyChar,
                           TRUE);
                MoveWindow(hwndGroup2,cxClient/20-cxClient/40,cyClient/26+2.4*cyChar,cxClient-cxClient/10, 4* cyChar,
                           TRUE);
                MoveWindow(hwndGroup3,cxClient/20-cxClient/40,cyClient/11+cyClient/7,cxClient-cxClient/10, cyClient-cyClient/3.5,
                           TRUE);
                return 0;
            }
        case WM_COMMAND:// 子窗口发送消息给父窗口
            {
                wmId    = LOWORD(wParam);
                wmEvent = HIWORD(wParam);
                switch (wmId)
                {
                    case ID_SETCHECK:    // 对按钮子窗口编号 (wmId) 为 10 的子窗口消息(选中默认)进行处理

                        SendMessage(hRadioButton1[0], BM_SETCHECK, 1, 0);
                        SendMessage(hRadioButton2[0], BM_SETCHECK, 1, 0);

                        for(i=1;i<3;i++)
                        {
                            SendMessage(hRadioButton1[i], BM_SETCHECK, 0, 0);
                            SendMessage(hRadioButton2[i], BM_SETCHECK, 0, 0);
                        }
                        break;
                    case ID_SETUNCHECK:
                        for(i=0;i<3;i++)
                        {
                            SendMessage(hRadioButton1[i], BM_SETCHECK, 0, 0);
                            SendMessage(hRadioButton2[i], BM_SETCHECK, 0, 0);
                        }
                        break;
                    case ID_BANPICK:
                        for(i=1;i<3;i++)
                        {
                            IsWindowEnabled(hRadioButton1[i]) ?
                            EnableWindow(hRadioButton1[i], FALSE) :
                            EnableWindow(hRadioButton1[i], TRUE);
                        }
                        break;
                    case ID_SHOWHIDE:    // 把6个按钮都隐藏或者恢复
                        for(i=0;i<3;i++)
                        {
                            if (IsWindowVisible(hRadioButton1[i]))
                            {
                                ShowWindow(hRadioButton1[i], SW_HIDE);
                            }
                            else
                            {
                                ShowWindow(hRadioButton1[i], SW_SHOWNORMAL);
                            }

                            if (IsWindowVisible(hRadioButton2[i]))
                            {
                                ShowWindow(hRadioButton2[i], SW_HIDE);
                            }
                            else
                            {
                                ShowWindow(hRadioButton2[i], SW_SHOWNORMAL);
                            }
                        }
                        break;
                    case ID_RADIOBUTTON1:
                        SendMessage(hRadioButton1[0], BM_SETCHECK, 1, 0);
                        SendMessage(hRadioButton1[1], BM_SETCHECK, 0, 0);
                        SendMessage(hRadioButton1[2], BM_SETCHECK, 0, 0);

                        break;
                    case ID_RADIOBUTTON2:
                        SendMessage(hRadioButton1[1], BM_SETCHECK, 1, 0);
                        SendMessage(hRadioButton1[0], BM_SETCHECK, 0, 0);
                        SendMessage(hRadioButton1[2], BM_SETCHECK, 0, 0);
                        break;
                    case ID_RADIOBUTTON3:
                        SendMessage(hRadioButton1[2], BM_SETCHECK, 1, 0);
                        SendMessage(hRadioButton1[0], BM_SETCHECK, 0, 0);
                        SendMessage(hRadioButton1[1], BM_SETCHECK, 0, 0);
                        break;
                    case ID_RADIOBUTTON4:
                        SendMessage(hRadioButton2[0], BM_SETCHECK, 1, 0);
                        SendMessage(hRadioButton2[1], BM_SETCHECK, 0, 0);
                        SendMessage(hRadioButton2[2], BM_SETCHECK, 0, 0);
                        break;
                    case ID_RADIOBUTTON5:
                        SendMessage(hRadioButton2[1], BM_SETCHECK, 1, 0);
                        SendMessage(hRadioButton2[0], BM_SETCHECK, 0, 0);
                        SendMessage(hRadioButton2[2], BM_SETCHECK, 0, 0);
                        break;
                    case ID_RADIOBUTTON6:
                        SendMessage(hRadioButton2[2], BM_SETCHECK, 1, 0);
                        SendMessage(hRadioButton2[0], BM_SETCHECK, 0, 0);
                        SendMessage(hRadioButton2[1], BM_SETCHECK, 0, 0);
                        break;
                    case ID_CLEAR: //清除选中
                        SetWindowText(hwndKeyEdit, TEXT("\0"));
                        break;
                    case ID_KEY_EDIT: //编辑密钥
                        {
                            if(wmEvent==EN_MAXTEXT)
                            {
                                MessageBox(hwnd, TEXT("超出编辑控件空间限制"), TEXT("错误"),
                                       MB_OK | MB_ICONINFORMATION
                                      );
                                break;
                            }
                            if(wmEvent==EN_ERRSPACE)
                            {
                                MessageBox(hwnd, TEXT("超出编辑控件空间限制"), TEXT("错误"),
                                       MB_OK | MB_ICONINFORMATION
                                      );
                                break;
                            }
                            if(wmEvent==EN_UPDATE)
                            {
                                GetWindowText(hwndKeyEdit, pszBuffer,
                                          sizeof(pszBuffer) / sizeof(TCHAR)
                                         );
                                //SetWindowText(hwndResultEdit, pszBuffer);
                                break;
                            }
                            break;
                        }
                    case ID_PUTKEY: // 使用默认密钥。
                        {
                            key={
                                    0x2b, 0x7e, 0x15, 0x16,
                                    0x28, 0xae, 0xd2, 0xa6,
                                    0xab, 0xf7, 0x15, 0x88,
                                    0x09, 0xcf, 0x4f, 0x3c
                                };
                            SetWindowText(hwndKeyEdit, TEXT("2b 7e 15 16 28 ae d2 a6 ab f7 15 88 09 cf 4f 3c"));
                            isKeySet = 1;
                            break;
                        }
                    case ID_SAVEKEY: //编辑密钥并保存
                        {
                            SendMessage(hwndKeyEdit,WM_GETTEXT,50,(LPARAM)sztitle);
                            MessageBox(NULL,sztitle, L"密钥保存成功", MB_OK);
                            //SetWindowText(hwndSourceDataEdit, sztitle);

                            char *pszBuf = new char[50];
                            pszBuf = cctryWideCharToAnsi(sztitle);
                            //cout<<pszBuf<<endl;

                            //char *转string：1）直接赋值；2）构造转换实现
                            // 赋值转换
                            string str = pszBuf;
                            string pattern =" ";
                            vector<string> result=split(str,pattern);

                            iter=result.begin();

                            //更改了密钥
                            i=0;
                            pre="0x";
                            char* a;
                            while(iter!=result.end())
                            {
                                //cout<<*iter<<endl;
                                //先加前缀0x转换string
                                pre.append(*iter);
                                sscanf(pre.c_str(),"%x",&key[i]);
                                // printf("%u\n",key[i]);
                                ++iter;
                                ++i;
                            }

                            isKeySet = 1;
                            break;
                        }
                    case ID_SOURCEDATA_EDIT: //编辑明文
                        {
                            if(wmEvent==EN_MAXTEXT)
                            {
                                MessageBox(hwnd, TEXT("超出编辑控件空间限制"), TEXT("错误"),
                                       MB_OK | MB_ICONINFORMATION
                                      );
                                break;
                            }
                            if(wmEvent==EN_ERRSPACE)
                            {
                                MessageBox(hwnd, TEXT("超出编辑控件空间限制"), TEXT("错误"),
                                       MB_OK | MB_ICONINFORMATION
                                      );
                                break;
                            }
                            if(wmEvent==EN_UPDATE)
                            {
                                GetWindowText(hwndSourceDataEdit, pszBuffer2,
                                          sizeof(pszBuffer2) / sizeof(TCHAR)
                                         );
                                //SetWindowText(hwndResultEdit, pszBuffer);
                                break;
                            }
                            break;
                        }
                    case ID_PUTPLAINTEXT: // 使用默认明文
                        {
                            if((SendMessage(hRadioButton1[0],BM_GETCHECK,0,0)==BST_UNCHECKED&&SendMessage(hRadioButton1[1],BM_GETCHECK,0,0)==BST_UNCHECKED&&SendMessage(hRadioButton1[2], BM_GETCHECK,0,0)==BST_UNCHECKED)||
                               (SendMessage(hRadioButton2[0],BM_GETCHECK,0,0)==BST_UNCHECKED&&SendMessage(hRadioButton2[1],BM_GETCHECK,0,0)==BST_UNCHECKED&&SendMessage(hRadioButton2[2], BM_GETCHECK,0,0)==BST_UNCHECKED)
                              )
                            {
                                MessageBox(hwnd, TEXT("请选择密钥长度、加/解密对象！"),TEXT("提示"), MB_OK);
                                break;
                            }
                            else  //密钥长度和加解密对象都设置完毕
                            {
                                //字符串
                                if(SendMessage(hRadioButton2[1], BM_GETCHECK, 0, 0) == BST_CHECKED)
                                {
                                      MessageBox(hwnd, TEXT("选中了字符串"),TEXT("提示"), MB_OK);
                                      SetWindowText(hwndSourceDataEdit, TEXT("Hello,My AES Cipher!"));
                                      char str[32]="Hello,My AES Cipher!";
                                      memcpy(pStr,str,32);
                                      isSourceData =1;
                                      //cout<<"保存的明文字符串"<<endl;
                                      //puts(pStr);//输出是正确的
                                      isSourceData=1;//使用默认明文：1
                                      break;
                                }
                                //16进制
                                else if(SendMessage(hRadioButton2[0], BM_GETCHECK, 0, 0) == BST_CHECKED)
                                {
                                      MessageBox(hwnd, TEXT("选中了16进制"),TEXT("提示"), MB_OK);
                                      //SetWindowText(hwndSourceDataEdit,TEXT("0x32 0x43 0xf6 0xa8\r\n0x88 0x5a 0x30 0x8d\r\n0x31 0x31 0x98 0xa2\r\n0xe0 0x37 0x07 0x34"));
                                    SetWindowText(hwndSourceDataEdit,TEXT("32 43 f6 a8 88 5a 30 8d 31 31 98 a2 e0 37 07 34"));
//
//                                      input={0x32, 0x43, 0xa2, 0x34,
//                                      0x32, 0x5a, 0x20, 0x22,
//                                      0xab, 0x3a, 0x14, 0x12,
//                                      0xb0, 0x43, 0x11, 0xa0};


                                      input = {
                                                0x32, 0x43, 0xf6, 0xa8,
                                                0x88, 0x5a, 0x30, 0x8d,
                                                0x31, 0x31, 0x98, 0xa2,
                                                0xe0, 0x37, 0x07, 0x34
                                              };

                                      isSourceData=1;//使用默认明文：1
                                      break;
                                }
                                //文件的待修改
                                else if(SendMessage(hRadioButton2[2], BM_GETCHECK, 0, 0) == BST_CHECKED)
                                {
                                      MessageBox(hwnd, TEXT("请点击open file打开一个文件并更新明文！"),TEXT("提示"), MB_OK);
                                      //SetWindowText(hwndSourceDataEdit,TEXT("0x32 0x43 0xf6 0xa8\r\n0x88 0x5a 0x30 0x8d\r\n0x31 0x31 0x98 0xa2\r\n0xe0 0x37 0x07 0x34"));
                                }
                            }
                            break;
                         }
                    case ID_SAVEPLAINTEXT:   //更改明文后更新数组
                        {
                            //根据源数据类型更改明文数组
                            //字符串      //字符串更改后保存 测试正确
                            if(SendMessage(hRadioButton2[1], BM_GETCHECK, 0, 0) == BST_CHECKED||SendMessage(hRadioButton2[2], BM_GETCHECK, 0, 0) == BST_CHECKED)
                            {
                                //保存输入的明文字符串  32???测试
                                SendMessage(hwndSourceDataEdit,WM_GETTEXT,32,(LPARAM)tempStr);

                                char *pszBuf1 = new char[48];
                                pszBuf1 = cctryWideCharToAnsi(tempStr);
                                memcpy(pStr,pszBuf1,strlen(pszBuf1));
                                isSourceData = 1;
                                MessageBox(hwnd, TEXT("明文更新成功"),TEXT("提示"), MB_OK);
                                return 0;
                            }
                            //16进制
                            else if(SendMessage(hRadioButton2[0], BM_GETCHECK, 0, 0) == BST_CHECKED)
                            {
                                //保存输入的明文
                                SendMessage(hwndSourceDataEdit,WM_GETTEXT,100,(LPARAM)plainsztitle);
                                char *pszBuf = new char[100];
                                pszBuf = cctryWideCharToAnsi(plainsztitle);

                                //char *转string：1）直接赋值；2）构造转换实现
                                // char*转换为string
                                // 赋值转换
                                string str = pszBuf;
                                //2.去除回车：
                                //str = str.replace('\n','');

                                //fun(str);


                                string pattern =" ";

                                 //str.split("[;,?|]");

                                vector<string> result=split(str,pattern);

                                iter=result.begin();

                                //更改了明文
                                i=0;
                                while(iter!=result.end())
                                {
                                    //cout<<"::"<<*iter<<endl;//输出正确
                                    sscanf((*iter).c_str(),"%x",&input[i]);
                                    //printf("%x\n",input[i]); //输出正确
                                    ++iter;
                                    ++i;

                                }


                                cout<<"明文数组更改后："<<endl;  //有问题
                                for(i=0;i<16;i++)
                                {
                                   printf("%x\t",input[i]);
                                }


                                isSourceData = 1;
                                MessageBox(hwnd, TEXT("明文更新成功"),TEXT("提示"), MB_OK);
                                return 0;
                            }
                            break;
                        }
                    case ID_FILEOPEN:
                        if (bNeedSave && IDCANCEL == AskAboutSave(hwnd, szTitleName))
                        {
                            return 0;
                        }
                        if (PopFileOpenDlg(hwnd, szFileName, szTitleName))
                        {
                            if (!PopFileRead(hwndSourceDataEdit, szFileName))
                            {
                                OkMessage(hwnd, TEXT("Could not read file %s!"),
                                          szTitleName);
                                szFileName[0]  = '\0';
                                szTitleName[0] = '\0';
                            }
                        }
                        DoCaption(hFileNameLabel, szTitleName);
                        bNeedSave = FALSE;
                        return 0;
                    case ID_FILESAVE:

                        if (PopFileSaveDlg(hwnd, szFileName, szTitleName))
                        {
                            MessageBox(hwnd, TEXT("保存成功！"),
                                        TEXT("save file"), MB_OK);

                            if (PopFileWrite(hwndSourceDataEdit, szFileName))
                            {
                                bNeedSave = FALSE;
                                return 1;
                            }
                            else
                            {
                                OkMessage(hwnd, TEXT("Could not write file %s"),
                                          szTitleName);
                                return 0;
                            }
                        }
                        return 0;
                    case ID_CLEARCONTENT:
                        if (bNeedSave && IDCANCEL == AskAboutSave(hwnd, szTitleName))
                        {
                            return 0;
                        }

                        SetWindowText(hwndSourceDataEdit, TEXT("\0"));
                        szFileName[0]  = '\0';
                        szTitleName[0] = '\0';
                        //DoCaption(hwnd, szTitleName);
                        bNeedSave = FALSE;
                        return 0;
                    case ID_CIPHER:
                        {
                            if((SendMessage(hRadioButton1[0],BM_GETCHECK,0,0)==BST_UNCHECKED&&SendMessage(hRadioButton1[1],BM_GETCHECK,0,0)==BST_UNCHECKED&&SendMessage(hRadioButton1[2], BM_GETCHECK,0,0)==BST_UNCHECKED)||
                               (SendMessage(hRadioButton2[0],BM_GETCHECK,0,0)==BST_UNCHECKED&&SendMessage(hRadioButton2[1],BM_GETCHECK,0,0)==BST_UNCHECKED&&SendMessage(hRadioButton2[2], BM_GETCHECK,0,0)==BST_UNCHECKED)
                              )
                            {
                                MessageBox(hwnd, TEXT("请选择密钥长度、加/解密对象！"),TEXT("提示"), MB_OK);
                                return 0;
                            }
                            if(!(isKeySet&&isSourceData))
                            {
                                cout<<"isKeySet"<<isKeySet<<" "<<"isSourceData"<<isSourceData<<endl;
                                MessageBox(hwnd, TEXT("请设置密钥和明文！"),TEXT("提示"), MB_OK);
                                return 0;
                            }
                            else
                            {
                                //源数据是十六进制
                                if(SendMessage(hRadioButton2[0], BM_GETCHECK, 0, 0) == BST_CHECKED)
                                {
                                    AES aes(key);  //带参数的构造函数

                                    //对更新后的明文数组加密解密
                                    aes.Cipher(input);//已经加密完了  input变了

                                    char temp[32];
                                    //测试正确
                                    convertUnCharToStr(temp, input,16);
                                    wchar_t *out = cctryAnsiCharToWide(temp);
                                    //将加密后的结果打印到“加密结果”框
                                    SetWindowTextW(hwndResultEdit,out);


                                    //对加密后的input数组（即密文）再解密
                                    aes.InvCipher(input);
                                    char temp1[32];
                                    //测试正确
                                    convertUnCharToStr(temp1, input,16);
                                    //printf("%s\n", temp);
                                    wchar_t *out1 = cctryAnsiCharToWide(temp1);
                                    //将解密后的结果打印到“解密结果”框
                                    SetWindowTextW(hwndInvResultEdit,out1);
                                }
                                //源数据是字符串
                                else if(SendMessage(hRadioButton2[1], BM_GETCHECK, 0, 0) == BST_CHECKED||SendMessage(hRadioButton2[2], BM_GETCHECK, 0, 0) == BST_CHECKED)
                                {
                                    //cout<<"字符串"<<endl;
                                    //puts(pStr); //输出正确
                                    AES aes(key);  //带参数的构造函数
                                    aes.Cipher((void *)pStr); //默认length=0

                                    //输出正确
                                    cout<<endl<<"加密后"<<endl;
                                    for(i=0; i<32; i++)printf("%x\t",(unsigned char)pStr[i]);  //%X读入十六进制整数
                                    cout<<endl;


                                    string str = pStr;
                                    string pattern =" ";
                                    vector<string> result1=split(str,pattern);
                                    iter=result1.begin();

                                    //更改了明文
                                    i=0;
                                    while(iter!=result1.end())
                                    {
                                        //cout<<*iter<<endl;//输出正确
                                        sscanf((*iter).c_str(),"%x",&pStr[i]);
                                        //printf("%x\n",pStr[i]); //输出正确
                                        ++iter;
                                        ++i;
                                    }
                                    wchar_t *out2 = cctryAnsiCharToWide(pStr);   //char*
                                    SetWindowText(hwndResultEdit,out2);



                                    //pStr正确  在窗口显示不正确 乱码？
                                    aes.InvCipher((void *)pStr,21);
                                    //cout<<endl;
                                    //for(i=0; i<32; i++) printf("%c ",(unsigned char)pStr[i]);


                                    //char temp1[32];
                                    //测试正确
                                    //convertUnCharToStr(temp1, input,16);
                                    //printf("%s\n", temp);
                                    wchar_t *out1 = cctryAnsiCharToWide(pStr);
                                    SetWindowTextW(hwndInvResultEdit,out1);

                                }

                               return 0;
                            }
                            break;
                        }
                    case ID_RESULTCLEAR: //清空所有内容，包括选中的选项和编辑类窗口
                        {
                            for(i=0;i<3;i++)
                            {
                                SendMessage(hRadioButton1[i], BM_SETCHECK, 0, 0);
                                SendMessage(hRadioButton2[i], BM_SETCHECK, 0, 0);
                            }
                            SetWindowText(hwndKeyEdit, TEXT("\0"));
                            SetWindowText(hwndSourceDataEdit, TEXT("\0"));
                            SetWindowText(hwndResultEdit, TEXT("\0"));
                            SetWindowText(hwndInvResultEdit, TEXT("\0"));

                            isKeySet=0;
                            isSourceData=0;

                            break;
                        }
                    case IDM_ABOUT:
                        DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);
                        break;

                    case IDM_EXIT:
                        DestroyWindow(hwnd);
                        break;
                    default:
                        break;
                }
            }

       case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            // TODO:  在此添加任意绘图代码...
            GetClientRect(hwnd, &rect);
            if (pText)
            {
                // -1 表示把所有的字符串都画出来
                DrawText(hdc, pText, -1, &rect, DT_EXPANDTABS | DT_WORDBREAK);
            }
            hOrgFont = (HFONT)SelectObject(hdc, hFont);
            SetBkMode(hdc, TRANSPARENT);
            //TextOut(hdc, 30, 30, TEXT("文本输出"), 14);
            SetBkMode(hdc, OPAQUE);
            SelectObject(hdc, hOrgFont);
            DeleteObject(hFont);
            DeleteObject(hOrgFont);

            EndPaint(hwnd, &ps);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);        /* send a WM_QUIT to the message queue */
            break;

        default:                      /* for messages that we don't deal with */
            return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);

    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }

        break;
    }

    return (INT_PTR)FALSE;
}

void PopFileInitialize(HWND hwnd)
{
    static TCHAR szFilter[] = TEXT("Text Files (*.TXT)\0*.txt\0")  \
                              TEXT("ASCII Files (*.ASC)\0*.asc\0") \
                              TEXT("All Files (*.*)\0*.*\0\0");

    ofn.lStructSize       = sizeof(OPENFILENAME);
    ofn.hwndOwner         = hwnd;
    ofn.hInstance         = NULL;
    ofn.lpstrFilter       = szFilter;
    ofn.lpstrCustomFilter = NULL;
    ofn.nMaxCustFilter    = 0;
    ofn.nFilterIndex      = 0;
    ofn.lpstrFile         = NULL;          // Set in Open and Close functions
    ofn.nMaxFile          = MAX_PATH;
    ofn.lpstrFileTitle    = NULL;          // Set in Open and Close functions
    ofn.nMaxFileTitle     = MAX_PATH;
    ofn.lpstrInitialDir   = NULL;
    ofn.lpstrTitle        = NULL;
    ofn.Flags             = 0;             // Set in Open and Close functions
    ofn.nFileOffset       = 0;
    ofn.nFileExtension    = 0;
    ofn.lpstrDefExt       = TEXT("txt");
    ofn.lCustData         = 0L;
    ofn.lpfnHook          = NULL;
    ofn.lpTemplateName    = NULL;
}

void PopFontInitialize(HWND hwndEdit)
{
    GetObject(GetStockObject(SYSTEM_FONT), sizeof(LOGFONT),
              (PTSTR) &logfont);

    hFont = CreateFontIndirect(&logfont);
    SendMessage(hwndEdit, WM_SETFONT, (WPARAM) hFont, 0);
}

short AskAboutSave(HWND hwnd, TCHAR * szTitleName)
{
    TCHAR szBuffer[64 + MAX_PATH];
    int   iReturn;

    wsprintf(szBuffer, TEXT("Save current changes in %s?"),
             szTitleName[0] ? szTitleName : UNTITLED);

    iReturn = MessageBox(hwnd, szBuffer,TEXT("AESCipher"),
                         MB_YESNOCANCEL | MB_ICONQUESTION);

    if (iReturn == IDYES)
        if (!SendMessage(hwnd, WM_COMMAND, ID_FILESAVE, 0))
        {
            iReturn = IDCANCEL;
        }
    return iReturn;
}

BOOL PopFileOpenDlg(HWND hwnd, PTSTR pstrFileName, PTSTR pstrTitleName)
{
    ofn.hwndOwner         = hwnd;
    ofn.lpstrFile         = pstrFileName;
    ofn.lpstrFileTitle    = pstrTitleName;
    ofn.Flags             = OFN_HIDEREADONLY | OFN_CREATEPROMPT;

    return GetOpenFileName(&ofn);
}

BOOL PopFileRead(HWND hwndEdit, PTSTR pstrFileName)
{
    BYTE   bySwap;
    DWORD  dwBytesRead;
    HANDLE hFile;
    int    i, iFileLength, iUniTest;
    PBYTE  pBuffer, pText, pConv;

    // Open the file.

    if (INVALID_HANDLE_VALUE ==
            (hFile = CreateFile(pstrFileName, GENERIC_READ, FILE_SHARE_READ,
                                NULL, OPEN_EXISTING, 0, NULL)))
    {
        return FALSE;
    }

    // Get file size in bytes and allocate memory for read.
    // Add an extra two bytes for zero termination.
    iFileLength = GetFileSize(hFile, NULL);
    pBuffer = (BYTE*)malloc(iFileLength + 2);

    // Read file and put terminating zeros at end.
    ReadFile(hFile, pBuffer, iFileLength, &dwBytesRead, NULL);
    CloseHandle(hFile);
    pBuffer[iFileLength] = '\0';
    pBuffer[iFileLength + 1] = '\0';

    // Test to see if the text is Unicode
    iUniTest = IS_TEXT_UNICODE_SIGNATURE | IS_TEXT_UNICODE_REVERSE_SIGNATURE;

    if (IsTextUnicode(pBuffer, iFileLength, &iUniTest))
    {
        pText = pBuffer + 2;
        iFileLength -= 2;

        if (iUniTest & IS_TEXT_UNICODE_REVERSE_SIGNATURE)
        {
            for (i = 0; i < iFileLength / 2; i++)
            {
                bySwap = ((BYTE *) pText) [2 * i];
                ((BYTE *) pText) [2 * i] = ((BYTE *) pText) [2 * i + 1];
                ((BYTE *) pText) [2 * i + 1] = bySwap;
            }
        }

        // Allocate memory for possibly converted string

        pConv = (BYTE*)malloc(iFileLength + 2);

        // If the edit control is not Unicode, convert Unicode text to
        // non-Unicode (ie, in general, wide character).

#ifndef UNICODE
       WideCharToMultiByte(CP_ACP, 0, (PWSTR) pText, -1, pConv,
                           iFileLength + 2, NULL, NULL);

        // If the edit control is Unicode, just copy the string
#else
        lstrcpy((PTSTR) pConv, (PTSTR) pText);
#endif

    }
    else      // the file is not Unicode
    {
        pText = pBuffer;

        // Allocate memory for possibly converted string.

        pConv = (BYTE *)malloc(2 * iFileLength + 2);

        // If the edit control is Unicode, convert ASCII text.

#ifdef UNICODE
        MultiByteToWideChar(CP_ACP, 0,(const char*)pText, -1, (PTSTR) pConv,
                            iFileLength + 1);

        // If not, just copy buffer
#else
        lstrcpy((PTSTR) pConv, (PTSTR) pText);
#endif
    }

    SetWindowText(hwndEdit, (PTSTR) pConv);
    free(pBuffer);
    free(pConv);

    return TRUE;
}

void OkMessage(HWND hwnd, TCHAR * szMessage, TCHAR * szTitleName)
{
    TCHAR szBuffer[64 + MAX_PATH];

    wsprintf(szBuffer, szMessage, szTitleName[0] ? szTitleName : UNTITLED);

    MessageBox(hwnd, szBuffer, TEXT("AesCipher"), MB_OK | MB_ICONEXCLAMATION);
}

void DoCaption(HWND hwnd, TCHAR * szTitleName)
{
    TCHAR szCaption[64 + MAX_PATH];

    wsprintf(szCaption, TEXT("%s - %s"), TEXT("AesCipher"),
             szTitleName[0] ? szTitleName : UNTITLED);

    SetWindowText(hwnd, szCaption);
}

BOOL PopFileSaveDlg(HWND hwnd, PTSTR pstrFileName, PTSTR pstrTitleName)
{
    ofn.hwndOwner         = hwnd;
    ofn.lpstrFile         = pstrFileName;
    ofn.lpstrFileTitle    = pstrTitleName;
    ofn.Flags             = OFN_OVERWRITEPROMPT;

    return GetSaveFileName(&ofn);
}

BOOL PopFileWrite(HWND hwndEdit, PTSTR pstrFileName)
{
    DWORD  dwBytesWritten;
    HANDLE hFile;
    int    iLength;
    PTSTR  pstrBuffer;
    WORD   wByteOrderMark = 0xFEFF;

    // Open the file, creating it if necessary

    if (INVALID_HANDLE_VALUE ==
            (hFile = CreateFile(pstrFileName, GENERIC_WRITE, 0,
                                NULL, CREATE_ALWAYS, 0, NULL)))
    {
        return FALSE;
    }

    // Get the number of characters in the edit control and allocate
    // memory for them.

    iLength = GetWindowTextLength(hwndEdit);
    pstrBuffer = (PTSTR) malloc((iLength + 1) * sizeof(TCHAR));

    if (!pstrBuffer)
    {
        CloseHandle(hFile);
        return FALSE;
    }

    // If the edit control will return Unicode text, write the
    // byte order mark to the file.

#ifdef UNICODE
    WriteFile(hFile, &wByteOrderMark, 2, &dwBytesWritten, NULL);
#endif

    // Get the edit buffer and write that out to the file.

    GetWindowText(hwndEdit, pstrBuffer, iLength + 1);
    WriteFile(hFile, pstrBuffer, iLength * sizeof(TCHAR),
              &dwBytesWritten, NULL);

    if ((iLength * sizeof(TCHAR)) != (int) dwBytesWritten)
    {
        CloseHandle(hFile);
        free(pstrBuffer);
        return FALSE;
    }

    CloseHandle(hFile);
    free(pstrBuffer);

    return TRUE;
}


//VOID CALLBACK TimerProc(HWND hWnd, UINT message, UINT iTimerID, DWORD dwTime)
//{
//    static  BOOL fFlipFlop = FALSE; //色彩翻转标记
//    HBRUSH  hBrush;
//    HDC     hdc;
//    RECT    rect;
//
//    MessageBeep(1);
//    fFlipFlop = !fFlipFlop; //可以放在这里或者 FillRect 表达式的后面
//
//    GetClientRect(hWnd, &rect);
//
//    hdc = GetDC(hWnd);
//
//    hBrush = CreateSolidBrush((fFlipFlop ? RGB(158, 89, 128) : RGB(45, 94, 86)));
//    FillRect(hdc, &rect, hBrush);
//
//    ReleaseDC(hWnd, hdc);
//
//    DeleteObject(hBrush);
//}

////线框矩形
//void linerect(HWND &hWnd,HDC &hDC,HBRUSH &NewBrush,RECT &r,PAINTSTRUCT &Ps, Rect rect)
//{
//
//    //hDC = BeginPaint(hWnd, &Ps);
//    hDC=GetDC(hWnd);
//    NewBrush = CreateSolidBrush(RGB(25, 25, 5));
//    SelectObject(hDC, NewBrush);
//    SetRect (&r, rect.x1,rect.y1,rect.x2, rect.y2);
//    FrameRect(hDC, &r, NewBrush);
//    DeleteObject(NewBrush);
//    DeleteObject(NewBrush);
//    //EndPaint(hWnd, &Ps);
//    ReleaseDC(hWnd,hDC);
//}

char *cctryWideCharToAnsi(wchar_t *pWideChar)
{
    if (!pWideChar)  return NULL;
    char *pszBuf = NULL;
    int needBytes = WideCharToMultiByte(CP_ACP, 0, pWideChar, -1, NULL, 0, NULL, NULL);
    if (needBytes > 0)
    {
        //cout<<"needbytes>0"<<endl;
        pszBuf = new char[needBytes+1];
        ZeroMemory(pszBuf, (needBytes+1)*sizeof(char));
        WideCharToMultiByte(CP_ACP, 0, pWideChar, -1, pszBuf, needBytes, NULL, NULL);
    }
    return pszBuf;
}

wchar_t *cctryAnsiCharToWide(char *pChar)
{
    if (!pChar) return NULL;
    wchar_t *pszBuf = NULL;
    int needWChar = MultiByteToWideChar(CP_ACP, 0, pChar, -1, NULL, 0);
    if (needWChar > 0)
    {
        pszBuf = new wchar_t[needWChar+1];
        ZeroMemory(pszBuf, (needWChar+1)*sizeof(wchar_t));
        MultiByteToWideChar(CP_ACP, 0, pChar, -1, pszBuf, needWChar);
    }
    return pszBuf;
}

//字符串分割函数
std::vector<std::string> split(std::string str, std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str += pattern;//扩展字符串以方便操作
    int size = str.size();
    for (int i = 0; i < size; i++)
    {
        pos = str.find(pattern, i);
        if (pos < size)
        {
            std::string s = str.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}

////字符串转换为16进制
//string string_to_hex(const string& str) //transfer string to hex-string
//{
//    string result="0x";
//    string tmp;
//    stringstream ss;
//    for(int i=0;i<str.size();i++)
//    {
//        //ss<<hex<<int(str[i])<<endl;
//        ss>>tmp;
//        result+=tmp;
//    }
//    return result;
//}


unsigned char CstringTouUnchar(string sendstring)
{
    unsigned char temp=sendstring[0];
    unsigned char temp1=sendstring[1];
    if(temp>='0'&&temp<='9')
    {
      temp&=0x0f;
    }
    if(temp>='a'&&temp<='f')
    {
      temp&=0x0f;
      temp+=0x09;
    }
    else
    {
     // MessageBox("第一位输入有误");
    }
    temp<<=4;

     if(temp1>='0'&&temp1<='9')
     {
        temp1&=0x0f;
     }
     if(temp1>='a'&&temp1<='f')
     {
          temp1&=0x0f;
          temp1+=0x09;
     }
     else
     {
        // MessageBox("第二位输入有误");
     }
     return (temp+temp1);
}

void convertUnCharToStr(char* str, unsigned char* UnChar, int ucLen)
{
    int i = 0;
    for(i = 0; i < ucLen; i++)
    {
        //格式化输str,每unsigned char 转换636f7079e799bee5baa6e997aee7ad9431333363383464字符占两位置%x写输%X写输
        sprintf(str + i * 2, "%02x", UnChar[i]);
    }
}

void convertStrToUnChar(char* str, unsigned char* UnChar)
{
    int i = strlen(str), j = 0, counter = 0;
    char c[2];
    unsigned int bytes[2];
    for (j = 0; j < i; j += 2)
    {
        if(0 == j % 2)
        {
            c[0] = str[j];
            c[1] = str[j + 1];
            sscanf(c, "%02x" , &bytes[0]);
            UnChar[counter] = bytes[0];
            counter++;
        }
    }
    return;
}


string fun(string str)
{
    int i,j;
    for (i = 0; str[i];)
    {
        if (str[i]=='\n'|5261|str[i]=='\r' || str[i]=='\t' || str[i]==' ')
        {
            for (j=i; str[j]; j++)
            {
                str[j]=str[j+1];
            }
        }
        else i++;
    }
    return str;
}
