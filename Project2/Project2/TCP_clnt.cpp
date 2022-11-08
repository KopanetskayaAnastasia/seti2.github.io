#include <iostream>
#include <winsock2.h>
#include <string>
#include <windows.h>
#pragma comment (lib,"Ws_32.lib")
using namespace std;
#define SRV_HOST "localhost" //��� �������
#define SRV_PORT 1234 //���� �������
#define CLNT_PORT 1235 //���� �������
#define BUF_SIZE 64 //������ ������
int main()
{
	//������������� ����������
	char buff[1023];
	if (WSAStartup(0x0202, (WSADATA*)&buff[0]))
	{
		cout << "Error WSAStartup\n" << WSAGetLastError();
		return -1;
	}
	//�������� ������ �������
	SOCKET s;
	int from_len;
	char buf[BUF_SIZE] = { 0 };
	hostent* hp;
	sockaddr_in clnt_sin, srv_sin;
	//�������� ������ �������
	s = socket(AF_INET, SOCK_STREAM, 0);
	//����������� ���������� �������
	clnt_sin.sin_family = AF_INET;
	clnt_sin.sin_addr.s_addr = 0;
	clnt_sin.sin_port = htons(CLNT_PORT);
	//���������� ������ � ������� �������
	bind(s, (sockaddr*)&clnt_sin, sizeof(clnt_sin));
	//����������� ���������� �������
	hp = gethostbyname(SRV_HOST);
	srv_sin.sin_port = htons(SRV_PORT);
	srv_sin.sin_family = AF_INET;
	((unsigned long*)&srv_sin.sin_addr)[0] = ((unsigned long**)hp->h_addr_list)[0][0];
	//���������� � ��������
	connect(s,( sockaddr*)& srv_sin, sizeof(srv_sin));
	string mst;
	//������ � ��������, ���� ������ �� �������� "Bye"
	do
	{
		//��������� ��������� �� �������
		from_len = recv(s, (char*)&buf, BUF_SIZE, 0);
		buf[from_len] = 0;
		cout << buf << endl;
		//�������� ��������� �������
		getline(cin, mst);
		int msg_size = mst.size();
		send(s, (char*)&mst[0], msg_size, 0);
	}while (mst != "Bye");
	//������ ����������
	cout << "exit" << endl;
	closesocket(s);
	return 0;
}