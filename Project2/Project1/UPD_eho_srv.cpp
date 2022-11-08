#include <iostream>
#include <winsock2.h>
#include <string>
#include <windows.h>
#pragma comment (lib,"Ws_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma warning(disable: 4996)
using namespace std;
//������ �������
#define SRV_PORT 1234;
#define sHELLO "Hello,student!\n"
int main() 
{
	char buff[1024];
	//������������� Winsock
	if (WSAStartup(0x0202, (WSADATA*)&buff[0]))
	{
		//������
		cout << "WSAStartup error: " << WSAGetLastError();
		return -1;
	}
	//�������� ������ �������
	SOCKET Lsock;
	Lsock = socket(AF_INET, SOCK_DGRAM, 0);
	if (Lsock == INVALID_SOCKET)
	{
		cout << "socket() error: " << WSAGetLastError();
		WSACleanup();
		return -1;
	}
	//������� ���������� ������ � �������� ���������
	sockaddr_in Laddr;
	Laddr.sin_family = AF_INET;//�������� ���������
	Laddr.sin_addr.s_addr = INADDR_ANY;
	//��� 0 (����� IP �����)
	Laddr.sin_port = htons(SRV_PORT);
	//���������� ������ � ��������� �������
	if (bind(Lsock, (sockaddr*)&Laddr, sizeof(Laddr)))
	{
		cout << "BIND ERROR:" << WSAGetLastError();
		closesocket(Lsock);
		WSACleanup();
		return -1;
	}
	//��������� �������� ���������� ���������
	while (1)
	{
		//��������� ��� ���������� ������ �������,����������� ����������� �������
		sockaddr_in Caddr;
		int Caddr_size = sizeof(Caddr);
		//����� ������ �� �������
		int bsize = recvfrom(Lsock, &buff[0], sizeof(buff) - 1, 0, (sockaddr*)&Caddr, &Caddr_size);
		if (bsize == SOCKET_ERROR)//������ ��������
			cout << "recvfrom() error: " << WSAGetLastError();
		//����������� ���������� �������: IP-������, ����������� �����, �����
		hostent* hst;
		hst = gethostbyaddr((char*)&Caddr.sin_addr, 4, AF_INET);
		cout << "NEW DATAGRAB! \n" << ((hst) ? hst->h_name : "unknown host") << "/n" << inet_ntoa(Caddr.sin_addr) << "/n" << ntohs(Caddr.sin_port) << '\n';
		//����� ��������� ��������� � �������
		buff[bsize] = '\0';//���������� ������������ ����
		cout << "C=>S: " << buff << '\n';//����� �� �����
		//������� ����������� �������
		sendto(Lsock, &buff[0], bsize, 0, (sockaddr*)&Caddr, sizeof(Caddr));
	}
	return 0;
}