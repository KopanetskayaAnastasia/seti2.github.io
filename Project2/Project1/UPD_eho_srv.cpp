#include <iostream>
#include <winsock2.h>
#include <string>
#include <windows.h>
#pragma comment (lib,"Ws_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma warning(disable: 4996)
using namespace std;
//данные сервера
#define SRV_PORT 1234;
#define sHELLO "Hello,student!\n"
int main() 
{
	char buff[1024];
	//инициализация Winsock
	if (WSAStartup(0x0202, (WSADATA*)&buff[0]))
	{
		//ошибка
		cout << "WSAStartup error: " << WSAGetLastError();
		return -1;
	}
	//создание сокета сервера
	SOCKET Lsock;
	Lsock = socket(AF_INET, SOCK_DGRAM, 0);
	if (Lsock == INVALID_SOCKET)
	{
		cout << "socket() error: " << WSAGetLastError();
		WSACleanup();
		return -1;
	}
	//задание параметров сокета в адресной структуре
	sockaddr_in Laddr;
	Laddr.sin_family = AF_INET;//адресная структура
	Laddr.sin_addr.s_addr = INADDR_ANY;
	//или 0 (любой IP адрес)
	Laddr.sin_port = htons(SRV_PORT);
	//связывание сокета с локальным адресом
	if (bind(Lsock, (sockaddr*)&Laddr, sizeof(Laddr)))
	{
		cout << "BIND ERROR:" << WSAGetLastError();
		closesocket(Lsock);
		WSACleanup();
		return -1;
	}
	//обрвботув пакетовБ присланных клиентами
	while (1)
	{
		//структура для параметров сокета клиента,приславщего дейтаграмму серверу
		sockaddr_in Caddr;
		int Caddr_size = sizeof(Caddr);
		//прием данных от клиента
		int bsize = recvfrom(Lsock, &buff[0], sizeof(buff) - 1, 0, (sockaddr*)&Caddr, &Caddr_size);
		if (bsize == SOCKET_ERROR)//ошибка передачи
			cout << "recvfrom() error: " << WSAGetLastError();
		//определение параметров клиента: IP-адреса, символьного имени, порта
		hostent* hst;
		hst = gethostbyaddr((char*)&Caddr.sin_addr, 4, AF_INET);
		cout << "NEW DATAGRAB! \n" << ((hst) ? hst->h_name : "unknown host") << "/n" << inet_ntoa(Caddr.sin_addr) << "/n" << ntohs(Caddr.sin_port) << '\n';
		//вывод принятого сообщение в консоли
		buff[bsize] = '\0';//добавление завершающего нуля
		cout << "C=>S: " << buff << '\n';//вывод на экран
		//посылка дейтаграммы клиенту
		sendto(Lsock, &buff[0], bsize, 0, (sockaddr*)&Caddr, sizeof(Caddr));
	}
	return 0;
}