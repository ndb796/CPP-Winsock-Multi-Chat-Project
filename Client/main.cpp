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
		// �����κ��� �޽����� ����ؼ� �޽����� ���� �޾� ����մϴ�.
		if ((size = recv(clientSocket, Buffer, 256, NULL)) > 0) {
			cout << Buffer << '\n';
		}
		Sleep(100);
	}
}

int main() {
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);

	// ������ ���� ������ �����մϴ�.
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddress.sin_port = htons(9876);
	serverAddress.sin_family = AF_INET;

	cout << "������ ������ �õ��մϴ�." << endl;
	connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress));
	
	char *id = new char[64];
	char *welcome = new char[64];
	char *clientCount = new char[64];
	
	ZeroMemory(id, 64); // NULL ������ ������ �ʱ�ȭ
	ZeroMemory(welcome, 64); // NULL ������ ������ �ʱ�ȭ
	ZeroMemory(clientCount, 64); // NULL ������ ������ �ʱ�ȭ

	recv(clientSocket, id, 64, 0); // ����� ID�� ���� �޽��ϴ�.
	recv(clientSocket, welcome, 64, 0); // ȯ�� �޽����� ���� �޽��ϴ�.
	recv(clientSocket, clientCount, 64, 0); // ȯ�� �޽����� ���� �޽��ϴ�.

	cout << welcome << endl;
	cout << "������ ��: " << clientCount << endl;
	cout << "������ ID: " << id << endl;
	cout << "�濡 �����մϴ�." << endl;

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE) clientThread, NULL, NULL, NULL);

	char *message = new char[256];
	while (true) {
		ZeroMemory(message, 256);
		// �Է��� �޽����� �ݺ������� ������ �����մϴ�.
		cin.getline(message, 256);
		send(clientSocket, message, 256, NULL);
		Sleep(100);
	}
}