// RpcServer.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
#include "echo.pb.h"
#include "TcpConnect.h"
#include <boost/thread/thread.hpp>   
#include "RPCServerService.h"

using namespace boost::asio;

int main()    
{
	io_service ios;
	int inPara = 0;
	std::cout<<"0:server,    other:client"<<std::endl;
	std::cin>>inPara;
	if (inPara == 0){
		TcpServer *server = new TcpServer(ios);
		boost::thread t(boost::bind(&boost::asio::io_service::run, &ios));
		//ios.run();
		while (true)
		{
			string in;
			std::cin>>in;
			server->echo(in);
		}
	}else{
		TcpClient *client = new TcpClient(ios);
		boost::thread t(boost::bind(&boost::asio::io_service::run, &ios));
		while(true){
			//����д����cin>>in���������̣����½��յ������ݣ����ǲ��ص��������ݺ�����
			//ios.poll();
			string in;
			std::cin>>in;
			echo::EchoRequest request;
			request.set_message(in);
			echo::EchoService * service = new echo::EchoService::Stub(client->getConnection());
			service->Echo(NULL, &request, NULL, NULL);
			}
	}
  }
