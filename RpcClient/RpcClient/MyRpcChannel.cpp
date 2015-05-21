#include "stdafx.h"
#include "MyRpcChannel.h"
#include <iostream>
#include <WINSOCK2.H> 
#include "echo.pb.h"
namespace google {
namespace protobuf {

	//���������ʹ�õĳ���    
#define SERVER_ADDRESS "127.0.0.1" //��������IP��ַ    
#define PORT           5150         //�������Ķ˿ں�    
#define MSGSIZE        1024         //�շ��������Ĵ�С    
#pragma comment(lib, "ws2_32.lib")    

MyRpcChannel::MyRpcChannel(void)
{

}


MyRpcChannel::~MyRpcChannel(void)
{
}

void MyRpcChannel::CallMethod(const MethodDescriptor* method,
                          RpcController* controller,
                          const Message* request,
                          Message* response,
						  Closure* done){
	
	WSADATA wsaData;    
    //���������׽���    
    SOCKET sClient;    
    //����Զ�̷������ĵ�ַ��Ϣ    
    SOCKADDR_IN server;    
    //�շ�������    
    //�ɹ������ֽڵĸ���    
    int ret;    
                 
    // Initialize Windows socket library    
    WSAStartup(0x0202, &wsaData);    
                 
    // �����ͻ����׽���    
    sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //AF_INETָ��ʹ��TCP/IPЭ���壻    
                                                         //SOCK_STREAM, IPPROTO_TCP����ָ��ʹ��TCPЭ��    
    // ָ��Զ�̷������ĵ�ַ��Ϣ(�˿ںš�IP��ַ��)    
    memset(&server, 0, sizeof(SOCKADDR_IN)); //�Ƚ������ַ��server��Ϊȫ0    
    server.sin_family = PF_INET; //������ַ��ʽ��TCP/IP��ַ��ʽ    
    server.sin_port = htons(PORT); //ָ�����ӷ������Ķ˿ںţ�htons()���� converts values between the host and network byte order    
    server.sin_addr.s_addr = inet_addr(SERVER_ADDRESS); //ָ�����ӷ�������IP��ַ    
                                                        //�ṹSOCKADDR_IN��sin_addr�ֶ����ڱ���IP��ַ��sin_addr�ֶ�Ҳ��һ���ṹ�壬sin_addr.s_addr�������ձ���IP��ַ    
                                                        //inet_addr()���ڽ� �����"127.0.0.1"�ַ���ת��ΪIP��ַ��ʽ    
    //�����ղ�ָ���ķ�������    
    connect(sClient, (struct sockaddr *) &server, sizeof(SOCKADDR_IN)); //���Ӻ������sClient��ʹ���������    
                                                                        //server������Զ�̷������ĵ�ַ��Ϣ    
	
    // ��������    
	char data[1000];
	int index = method->index();
	data[0] = '0'+index;
	int i = 0;
	for (;i < strlen(request->SerializeAsString().c_str()); i++)
	{
		data[i+1] = request->SerializeAsString().c_str()[i];
	}
	data[i+1] = '\0';

    send(sClient, data,strlen(data), 0); 
	//sClientָ�����ĸ����ӷ��ͣ� szMessageָ�����������ݵı����ַ ��strlen(szMessage)ָ�����ݳ���    
	std::cout<<request->SerializeAsString()<<std::endl;
                 
    // �ͷ����Ӻͽ��н�������    
    closesocket(sClient);    
    WSACleanup();

}


}}