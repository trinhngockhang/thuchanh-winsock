// bai2_phan3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1
#include <string.h>
#include <stdlib.h>

#include "winsock2.h"
#include <string>
using namespace std;
char sendBuf[256];
int ret;
DWORD WINAPI ClientThread(LPVOID);
int main()
{
	
	
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(9000);

	bind(listener, (SOCKADDR *)&addr, sizeof(addr));
	listen(listener, 5);
	while (true)
	{
		SOCKET client = accept(listener, NULL, NULL);
		printf("Accepted client: %d\n", client);
		CreateThread(0, 0, ClientThread, &client, 0, 0);
	}
	
	WSACleanup();
	system("PAUSE");
    return 0;
}

DWORD WINAPI ClientThread(LPVOID lpParam)
{
	SOCKET client = *(SOCKET *)lpParam;
	while (true) {
		char buf[2048];
		ret = recv(client, buf, sizeof(buf), 0);
		if (ret <= 0) break;
		buf[ret] = 0;
		printf("Received: %s\n", buf);
		if (strncmp(buf, "GET / HTTP", 10) == 0) {
			WIN32_FIND_DATAA DATA;
			HANDLE h = FindFirstFileA("C:\\*.*", &DATA);
			char msg[2048] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body>";
			char endHtml[128] = " </body> </html>";
			char btag[8] = " <b> ";
			char bTagEnd[8] = " </b> ";
			char brTag[8] = " <br/>";
			char iTag[4] = "<i>";
			char iTagEnd[8] = "</i>";
			do
			{
				if (DATA.dwFileAttributes &
					FILE_ATTRIBUTE_DIRECTORY) {
					strcat(msg, btag);
					strcat(msg, DATA.cFileName);
					strcat(msg, bTagEnd);
					strcat(msg, brTag);
				}
				else
				{
					INT64 size = (DATA.nFileSizeHigh*(MAXDWORD + 1)) + DATA.nFileSizeLow;
					strcat(msg, iTag);
					strcat(msg, DATA.cFileName);
					strcat(msg, iTagEnd);
					strcat(msg, brTag);
				}
			} while (FindNextFileA(h, &DATA));
			strcat(msg, endHtml);
			send(client, msg, strlen(msg), 0);
			closesocket(client);
		}
		else {
			char msg[1024] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Sai duong dan</h1></body></html>";
			send(client, msg, strlen(msg), 0);
			closesocket(client);
		}
		closesocket(client);
	}
}