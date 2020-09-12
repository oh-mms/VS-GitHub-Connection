//  Created by oh-mms. 2020.
//  e-mail	: chohmms@gmail.com
//  blog	: https://blog.naver.com/oh-mms
//  gitHub	: https://github.com/oh-mms
//  You can use it freely in any way.
//  If you want to contact me, please send a mail.

// This is a small TCP echo socket program using winsock.
// And This may occur an error in VS, because of old functions.
// So If you have something wrong, then execute in Release mode.

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
	SOCKET					serverSocket, clientSocket;					// Server has a server socket and a client socket.
	SOCKADDR_IN		serverAddress, clientAddress;

	int serverPort = 9876;
	char received[256];

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)		// Initialize the Winsock.
		ShowErrorMessage("WSAStartup() error");

	serverSocket = socket(PF_INET, SOCK_STREAM, 0);		// Generate a TCP socket.

	if (serverSocket == INVALID_SOCKET)
		ShowErrorMessage("socket() error");

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);	// Convert 4byte integer to network byte.
	serverAddress.sin_port = htons(serverPort);						// Convert 2byte integer to network byte.

	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("bind() error");
	cout << "[State] : bind()\n";

	if (listen(serverSocket, 5) == SOCKET_ERROR)
		ShowErrorMessage("listen() error");
	cout << "[State] : listen()\n";

	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
	cout << "[State] : accept()\n";

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("accept() error\n");

	// Send the message received to client again (echo).
	while (1) {
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = NULL;

		cout << "[Client Message] : " << received << '\n';
		cout << "[Message Sent] : " << received << '\n';

		if (strcmp(received, "[exit]") == 0) {
			send(clientSocket, received, sizeof(received) - 1, 0);
			cout << "[Server Shutdown]\n";
			break;
		}

		send(clientSocket, received, sizeof(received) - 1, 0);
	}

	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();
	system("pause");
	return 0; 
}