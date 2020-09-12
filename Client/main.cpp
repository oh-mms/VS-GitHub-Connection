//  Created by oh-mms. 2020.
//  e-mail	: chohmms@gmail.com
//  blog	: https://blog.naver.com/oh-mms
//  gitHub	: https://github.com/oh-mms
//  You can use it freely in any way.
//  If you want to contact me, please send a mail.

// This is a small TCP echo socket program using winsock.
// And This may occur an error in VS, because of old functions.
// So If you have something wrong, then execute in Release mode.

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <winsock2.h>

using namespace std;

void ShowErrorMessage(string message) {
	cout << "[ERROR] : " << message << '\n';
	system("pause");
	exit(1);
}

int main() {

	WSADATA				wsaData;
	SOCKET					clientSocket;
	SOCKADDR_IN		serverAddress;

	int serverPort = 9876;
	char received[256];
	string sent;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)			// Initialize the Winsock.
		ShowErrorMessage("WSAStartup() error\n");

	clientSocket = socket(PF_INET, SOCK_STREAM, 0);				// Generate a TCP socket.

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("socket() error\n");

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");	// Convert String type IP Adress to network byte.
	serverAddress.sin_port = htons(serverPort);							// Convert 2byte integer to network byte.

	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("connect() error\n");
	cout << "[State] : connect()\n";

	// Send a message to server and receive the echo message repeatedly.
	while (1) {
		cout << "[Message] : ";
		getline(cin, sent);
		if (sent == "") continue;
		send(clientSocket, sent.c_str(), sent.length(), 0);

		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = '\0';

		if (strcmp(received, "[exit]") == 0) {
			cout << "[Server Shutdown]\n";
			break;
		}

		cout << "[Server Message] : " << received << "\n";
	}

	closesocket(clientSocket);
	WSACleanup();
	system("pause");
	return 0;
}