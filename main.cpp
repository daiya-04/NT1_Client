#include <iostream>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <vector>
#include <string>

#pragma comment (lib, "WSock32.lib")

int main() {

	WSADATA wsaData;
	//WinSockの初期化
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
		//初期化出来なかったら...
		std::cout << "WinSockの初期化に失敗しました" << std::endl;
		return 1;
	}

	//ポート番号の指定
	unsigned short portNum = 0;
	std::cout << "使用するポート番号を指定してくださいーー＞";
	std::cin >> portNum;

	//IPアドレスの指定 192.168.1.16
	std::string ipAdress;
	std::cout << "IPアドレスを指定してくださいーー＞";
	std::cin >> ipAdress;
	ipAdress += '\0';

	//ソケットの作成
	SOCKET cSocket;
	cSocket = socket(AF_INET, SOCK_STREAM, 0);
	//失敗したら終了
	if (cSocket == INVALID_SOCKET) {
		std::cout << "ソケットの作成に失敗しました" << std::endl;
		WSACleanup();
		return 0;
	}
	std::cout << "ソケットをオープンしました" << std::endl;

	//ホスト名からIPアドレス取得
	HOSTENT* hostIp;

	hostIp = gethostbyname(ipAdress.c_str());

	//ソケットをサーバーに接続
	SOCKADDR_IN saddr;
	memset(&saddr, 0, sizeof(SOCKADDR_IN));
	saddr.sin_family = hostIp->h_addrtype;
	saddr.sin_port = htons(portNum);
	saddr.sin_addr.S_un.S_addr = *(reinterpret_cast<u_long*>(hostIp->h_addr_list[0]));

	if (connect(cSocket, reinterpret_cast<SOCKADDR*>(&saddr), sizeof(saddr)) == SOCKET_ERROR) {
		std::cout << "サーバーと接続できませんでした" << std::endl;
		closesocket(cSocket);
		WSACleanup();
		return 0;
	}

	///会話開始
	while (true) {

		int nRcv;
		std::vector<char> szBuf(1024);

		std::cout << "送信ーー＞";
		std::cin >> szBuf.data();
		send(cSocket, szBuf.data(), static_cast<int>(strlen(szBuf.data())), 0);


		nRcv = recv(cSocket, szBuf.data(), szBuf.size() - 1, 0);
		szBuf[nRcv] = '\0';
		std::cout << "受信ーー＞" << szBuf.data() << std::endl;

	}
	
	//ソケットの解放
	closesocket(cSocket);
	//WinSockの終了
	WSACleanup();

	return 0;
}