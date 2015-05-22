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

MyRpcChannel::MyRpcChannel(TcpClient * tp)
{
	 m_tcpClient = tp;
}


MyRpcChannel::~MyRpcChannel(void)
{
}

void MyRpcChannel::CallMethod(const MethodDescriptor* method,
                          RpcController* controller,
                          const Message* request,
                          Message* response,
						  Closure* done){
							  char c[2];
							  c[0]= '0'+method->index();
							  c[1] = 0;
							  std::string cstr = std::string(c);
							  cstr+=request->SerializeAsString();
							  m_tcpClient->sendMessage(cstr);
							  std::cout<<"send:"<<cstr<<std::endl;
}



}}//namespace