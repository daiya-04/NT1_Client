#include <iostream>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <vector>
#include <string>

#pragma comment (lib, "WSock32.lib")

int main() {

	WSADATA wsaData;
	//WinSock�̏�����
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
		//�������o���Ȃ�������...
		std::cout << "WinSock�̏������Ɏ��s���܂���" << std::endl;
		return 1;
	}

	//�|�[�g�ԍ��̎w��
	unsigned short portNum = 0;
	std::cout << "�g�p����|�[�g�ԍ����w�肵�Ă��������[�[��";
	std::cin >> portNum;

	//IP�A�h���X�̎w�� 192.168.1.16
	std::string ipAdress;
	std::cout << "IP�A�h���X���w�肵�Ă��������[�[��";
	std::cin >> ipAdress;
	ipAdress += '\0';

	//�\�P�b�g�̍쐬
	SOCKET cSocket;
	cSocket = socket(AF_INET, SOCK_STREAM, 0);
	//���s������I��
	if (cSocket == INVALID_SOCKET) {
		std::cout << "�\�P�b�g�̍쐬�Ɏ��s���܂���" << std::endl;
		WSACleanup();
		return 0;
	}
	std::cout << "�\�P�b�g���I�[�v�����܂���" << std::endl;

	//�z�X�g������IP�A�h���X�擾
	HOSTENT* hostIp;

	hostIp = gethostbyname(ipAdress.c_str());

	//�\�P�b�g���T�[�o�[�ɐڑ�
	SOCKADDR_IN saddr;
	memset(&saddr, 0, sizeof(SOCKADDR_IN));
	saddr.sin_family = hostIp->h_addrtype;
	saddr.sin_port = htons(portNum);
	saddr.sin_addr.S_un.S_addr = *(reinterpret_cast<u_long*>(hostIp->h_addr_list[0]));

	if (connect(cSocket, reinterpret_cast<SOCKADDR*>(&saddr), sizeof(saddr)) == SOCKET_ERROR) {
		std::cout << "�T�[�o�[�Ɛڑ��ł��܂���ł���" << std::endl;
		closesocket(cSocket);
		WSACleanup();
		return 0;
	}

	///��b�J�n
	while (true) {

		int nRcv;
		std::vector<char> szBuf(1024);

		std::cout << "���M�[�[��";
		std::cin >> szBuf.data();
		send(cSocket, szBuf.data(), static_cast<int>(strlen(szBuf.data())), 0);


		nRcv = recv(cSocket, szBuf.data(), szBuf.size() - 1, 0);
		szBuf[nRcv] = '\0';
		std::cout << "��M�[�[��" << szBuf.data() << std::endl;

	}
	
	//�\�P�b�g�̉��
	closesocket(cSocket);
	//WinSock�̏I��
	WSACleanup();

	return 0;
}