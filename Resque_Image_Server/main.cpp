#include <vector>
#include <fstream>

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string.h>

#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "ws2_32.lib")


#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

std::string robo1 = "robot1";
std::string robo2 = "robot2";
int main(){

	int port = 9877;
	int recvsocket;

	struct sockaddr_in recvSockAddr;
	struct sockaddr_in clientSockAddr;

	int clientaddrlen = sizeof(clientSockAddr);

	//�e��p�����[�^
	int status;
	int numrcv;
	unsigned long on = 1;


	///////////////////////////////
	WSADATA data;
	SOCKET sock;
	WSAStartup(MAKEWORD(2, 0), &data);
	memset(&recvSockAddr, 0, sizeof(recvSockAddr));

	recvSockAddr.sin_family = AF_INET;
	recvSockAddr.sin_port = htons(port);
	recvSockAddr.sin_addr.S_un.S_addr = INADDR_ANY;

	//�\�P�b�g�쐬 : �G���[�̏ꍇ�AINVALID�QSOCKET��Ԃ��B

	//recvsocket = socket(AF_INET,SOCK_DGRAM,0);
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET){
		std::cout << "socket : " << WSAGetLastError() << std::endl;
		return 2;
	}

	//bind :�@�\�P�b�g���A�h���X�A�|�[�g�Ɍ��ѕt����@����̏ꍇ0���Ԃ�B


	status = bind(sock, (LPSOCKADDR)&recvSockAddr, (int)sizeof(recvSockAddr));
	if (status != 0){
		std::cout << "bind : " << WSAGetLastError() << std::endl;
		return 1;
	}


	//�E�B���h�E���쐬
	cv::namedWindow("camera", 1);

	cv::Mat image = cv::Mat(320, 240, CV_8UC3);
	static const int receivesize = 65500;
	static char buff[receivesize];

	std::vector<uchar> ibuff;
	//cv::waitKey(20000);
	std::cout << "first listen" << std::endl;

//	char* sss = "192.168.2.27";
	while (cv::waitKey(10) == -1){
		//numrcv = recvfrom(recvsocket,buff,receivesize,0,(LPSOCKADDR)&clientSockAddr,&clientaddrlen);
		numrcv = recvfrom(sock, buff, receivesize, 0, (LPSOCKADDR)&clientSockAddr, &clientaddrlen);
		//ip�A�h���X����v���Ă�����̓��s�ŏ���
		//if (clientSockAddr.sin_addr.S_un.S_addr == inet_addr("192.168.2.27")){
		//	std::cout << "ok" << std::endl;
		//}

		for (int i = 0; i < sizeof(buff); i++){
			ibuff.push_back((uchar)buff[i]);
		}
		if (numrcv == -1){
			std::cout << "error : " << WSAGetLastError() << std::endl;
			status = closesocket(sock);
			break;
		}
		image = imdecode(cv::Mat(ibuff), CV_LOAD_IMAGE_COLOR);
		//std::cout << "width : " << image.cols << " height : " << image.rows << std::endl;
		if (clientSockAddr.sin_addr.S_un.S_addr == inet_addr("192.168.2.27")){
			//std::cout << "ok" << std::endl;
			cv::imshow(robo2,image);
		}
		if (clientSockAddr.sin_addr.S_un.S_addr == inet_addr("192.168.2.22"))
		{
			cv::imshow(robo1,image);
		}
		//if (){

		//}
		ibuff.clear();
	}

	closesocket(sock);
	WSACleanup();

	return 1;
}