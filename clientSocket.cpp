#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <Windows.h>
#include <Winsock2.h>
#include <WS2tcpip.h>

using namespace std;

int main()
{
	// Инициализация переменных и структур
	WSADATA wsaData;
	ADDRINFO hints;
	ADDRINFO* addrResult;

	SOCKET ConnectSocket = INVALID_SOCKET;
	const char* sendBuffer = "hello from Client";
	char recvBuffer[512];

	// Старт использования библиотеки сокетов: определение версии и структуры
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (result != 0)
	{
		cout << "WSAStartup failed result" << endl;
		WSACleanup();
		return 1;
	}

	// Инициализация hints
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Инициализация структуры адреса для подключения к серверу
	result = getaddrinfo("localhost", "666", &hints, &addrResult);
	if (result != 0)
	{
		cout << "WSAStartup failed error" << endl;
		WSACleanup();
		return 1;
	}

	// Создание клиентского сокета
	ConnectSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);
	if (ConnectSocket == INVALID_SOCKET)
	{
		cout << "getaddrinfo creation error" << endl;
		WSACleanup();
		return 1;
	}

	// Подключение к серверу
	ConnectSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);
	result = connect(ConnectSocket, addrResult->ai_addr, (int) addrResult->ai_addrlen);
	if (result == SOCKET_ERROR)
	{
		cout << "unable connect to server" << endl;
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}

	// Отправка данных на сервер
	result = send(ConnectSocket, sendBuffer, (int)strlen(sendBuffer), 0);
	if (result == SOCKET_ERROR)
	{
		cout << "send failed error" << result << endl;
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}
	cout << "sent" << result << "bytes" << endl;
	if (result == SOCKET_ERROR)
	{
		result = shutdown(ConnectSocket, SD_SEND);
		cout << "shutdown error" << endl;
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}

	result = send(ConnectSocket, sendBuffer, (int)strlen(sendBuffer), 0);
	if (result == SOCKET_ERROR)
	{
		cout << "send failed error" << result << endl;
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}
	cout << "sent " << result << " bytes" << endl;

	// Добавляем второе сообщение
	const char* additionalSendBuffer = "second message from Client";
	result = send(ConnectSocket, additionalSendBuffer, (int)strlen(additionalSendBuffer), 0);
	if (result == SOCKET_ERROR)
	{
		cout << "second send failed error" << result << endl;
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}
	cout << "sent " << result << " bytes" << endl;

	// Цикл приема данных от сервера
	do
	{
		ZeroMemory(recvBuffer, 512);
		result = recv(ConnectSocket, recvBuffer, 512, 0);

		if (result > 0)
		{
			cout << "received" << result << "bytes" << endl;
			cout << "received data" << recvBuffer << endl;
		}
		else if (result == 0)
			cout << "connection closed" << endl;

		else
			cout << "recv failed with error" << endl;

	} while (result > 0);

	// Освобождение ресурсов и завершение сеанса
	freeaddrinfo(addrResult);
	WSACleanup();
	return 1;
}

