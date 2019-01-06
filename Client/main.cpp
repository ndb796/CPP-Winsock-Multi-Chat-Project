#include <iostream>
#include <WinSock.h>
#include <Windows.h>
#pragma comment (lib, "ws2_32.lib")

using namespace std;

SOCKET clientSocket;
WSAData wsaData;
SOCKADDR_IN serverAddress;

void clientThread() {
	char *Buffer = new char[256];
	int size = 0;
	while (true) {
		ZeroMemory(Buffer, 256);
		// 서버로부터 메시지를 계속해서 메시지를 전달 받아 출력합니다.
		if ((size = recv(clientSocket, Buffer, 256, NULL)) > 0) {
			cout << Buffer << '\n';
		}
		Sleep(100);
	}
}

int main() {
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);

	// 서버에 대한 정보를 설정합니다.
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddress.sin_port = htons(9876);
	serverAddress.sin_family = AF_INET;

	cout << "서버에 연결을 시도합니다." << endl;
	connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress));
	
	char *id = new char[64];
	char *welcome = new char[64];
	char *clientCount = new char[64];
	
	ZeroMemory(id, 64); // NULL 값으로 데이터 초기화
	ZeroMemory(welcome, 64); // NULL 값으로 데이터 초기화
	ZeroMemory(clientCount, 64); // NULL 값으로 데이터 초기화

	recv(clientSocket, id, 64, 0); // 사용자 ID를 전달 받습니다.
	recv(clientSocket, welcome, 64, 0); // 환영 메시지를 전달 받습니다.
	recv(clientSocket, clientCount, 64, 0); // 환영 메시지를 전달 받습니다.

	cout << welcome << endl;
	cout << "접속자 수: " << clientCount << endl;
	cout << "배정된 ID: " << id << endl;
	cout << "방에 입장합니다." << endl;

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE) clientThread, NULL, NULL, NULL);

	char *message = new char[256];
	while (true) {
		ZeroMemory(message, 256);
		// 입력한 메시지를 반복적으로 서버로 전달합니다.
		cin.getline(message, 256);
		send(clientSocket, message, 256, NULL);
		Sleep(100);
	}
}