#ifndef HEAD_H_INCLUDED
#define HEAD_H_INCLUDED

#define UNTITLED TEXT ("(untitled)")

#include "stdlib.h"
#include<iostream>
#include <tchar.h>
#include <windows.h>
#include <stdio.h>

#include <TCHAR.H>
#include<string>
#include <strings.h>
#include <commdlg.h>

//#include <atlstr.h>
#include <string>
#include <vector>
#include <sstream>

using namespace std;


//����DOS����
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"wmainCRTStartup\"" )



typedef struct Rect
{
   int x1;
   int y1;
   int x2;
   int y2;
};

/*  Make the class name into a global variable  */
// ȫ�ֱ���:
HINSTANCE hInst;
TCHAR       szTitle[MAX_LOADSTRING];
TCHAR szClassName[ ] = _T("AESCipher");
TCHAR szWindowClass[MAX_LOADSTRING];            // ����������

static OPENFILENAME ofn;
static LOGFONT logfont;
static HFONT   hFont;

unsigned char input[16]; //��������
char pStr[160];//�������������С(������16����������
unsigned char key[16];   //������Կ

//��ʼ������
unsigned char IV[16]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
static int enCount = 0x00;
static int deCount = 0x00;



int isKeySet = 0;
int isSourceData = 0;

/*  Declare Windows procedure  */
// �˴���ģ���а����ĺ�����ǰ������:
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);

INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

short AskAboutSave(HWND hwnd, TCHAR * szTitleName);
BOOL PopFileOpenDlg(HWND hwnd, PTSTR pstrFileName, PTSTR pstrTitleName);
BOOL PopFileRead(HWND hwndEdit, PTSTR pstrFileName);
void OkMessage(HWND hwnd, TCHAR * szMessage, TCHAR * szTitleName);
void DoCaption(HWND hwnd, TCHAR * szTitleName);
void PopFileInitialize(HWND hwnd);
void PopFontInitialize(HWND hwndEdit);
BOOL PopFileSaveDlg(HWND hwnd, PTSTR pstrFileName, PTSTR pstrTitleName);
BOOL PopFileWrite(HWND hwndEdit, PTSTR pstrFileName);


char *cctryWideCharToAnsi(wchar_t *pWideChar);
wchar_t *cctryAnsiCharToWide(char *pChar);
vector<string> split(string str, string pattern);
unsigned char CstringTouUnchar(string sendstring);
void convertUnCharToStr(char* str, unsigned char* UnChar, int ucLen);
void convertStrToUnChar(char* str, unsigned char* UnChar);
string fun(string str);

#endif // HEAD_H_INCLUDED
