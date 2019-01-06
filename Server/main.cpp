#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <Winsock.h>
#include <iostream>
#include <vector>
#pragma comment (lib, "ws2_32.lib")

using namespace std;

class Client {
private:
	int clientID;
	SOCKET clientSocket;
public:
	Client(int clientID, SOCKET clientSocket) {
		this->clientID = clientID;
		this->clientSocket = clientSocket;
	}
	int getClientID() {
		return clientID;
	}
	SOCKET getClientSocket() {
		return clientSocket;
	}
};

SOCKET serverSocket;
vector<Client> connections;
WSAData wsaData;
SOCKADDR_IN serverAddress;

int nextID;

void ServerThread(Client *client) {
	char *sent = new char[256];
	char *received = new char[256];
	char *message = new char[256];
	int size = 0;
	while (true) {
		ZeroMemory(received, 256);
		if ((size = recv(client->getClientSocket(), received, 256, NULL)) > 0) {
			for (int i = 0; i < connections.size(); i++) {
				// �޽����� ���� Ŭ���̾�Ʈ�� ���
				if (connections[i].getClientSocket() == client->getClientSocket()) {
					ZeroMemory(sent, 256);
					sprintf(sent, "Ŭ���̾�Ʈ [%i]: %s", client->getClientID(), received);
					cout << sent << endl;
				}
				// �޽����� ������ ���� �ٸ� Ŭ���̾�Ʈ�� ���
				else {
					ZeroMemory(sent, 256);
					sprintf(sent, "Ŭ���̾�Ʈ [%i]: %s", client->getClientID(), received);
					send(connections[i].getClientSocket(), sent, 256, 0);
				}
			}
		}
		else {
			ZeroMemory(message, 256);
			sprintf(message, "Ŭ���̾�Ʈ [%i]�� ������ ���������ϴ�.", client->getClientID());
			cout << message << endl;
			for (int i = 0; i < connections.size(); i++) {
				if (connections[i].getClientID() == client->getClientID()) {
					connections.erase(connections.begin() + i);
					break;
				}
			}
			delete client;
			break;
		}
	}
}

int main() {
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	serverSocket = socket(AF_INET, SOCK_STREAM, NULL);

	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddress.sin_port = htons(9876);
	serverAddress.sin_family = AF_INET;

	cout << "[ C++ ��Ƽ ä�� ���� ���� ]" << endl;
	bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress));
	listen(serverSocket, 32);

	int addressLength = sizeof(serverAddress);
	while (true) {
		SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, NULL);
		if (clientSocket = accept(serverSocket, (SOCKADDR*)&serverAddress, &addressLength)) {
			Client *client = new Client(nextID, clientSocket);
			char *id = new char[64];
			ZeroMemory(id, 64);
			sprintf(id, "%i", nextID);
			char *welcome = new char[64];
			ZeroMemory(welcome, 64);
			sprintf(welcome, "%s", "[ C++ ��Ƽ ä�� Ŭ���̾�Ʈ ���� ]");
			char *clientCount = new char[64];
			ZeroMemory(clientCount, 64);
			sprintf(clientCount, "%d", connections.size() + 1);
			send(clientSocket, id, 64, NULL); // �ű� ����ڿ��� ID ����
			send(clientSocket, welcome, 64, NULL); // ȯ�� �޽��� ����
			send(clientSocket, clientCount, 64, NULL); // ������ �� ����
			cout << "[ ���ο� ����� ���� ]" << endl;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ServerThread, (LPVOID)client, NULL, NULL);
			connections.push_back(*client);
			nextID++;
		}
		Sleep(100);
	}
}