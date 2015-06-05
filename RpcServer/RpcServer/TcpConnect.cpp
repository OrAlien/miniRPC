#include "stdafx.h"
#include "TcpConnect.h"
#include <boost/bind.hpp>
#include <iostream>
#include "RPCServerService.h"


TcpConnection::TcpConnection(boost::asio::io_service &io):_sock(new boost::asio::ip::tcp::socket(io))
{
}

TcpConnection::~TcpConnection()
{
}

void TcpConnection::sendMessage(std::string str){
	std::cout<<"����:"<<str<<std::endl;
	_sock->async_write_some(boost::asio::buffer(str.c_str(),strlen(str.c_str())),boost::bind(&TcpConnection::write_handler,this,boost::asio::placeholders::error));
}

void TcpConnection::write_handler(const boost::system::error_code &){
	std::cout<<"���ͳɹ�!"<<std::endl;
}

void TcpConnection::read_handler(const boost::system::error_code& ec,boost::shared_ptr<std::vector<char>> str){
	if (ec){
		std::cout<<"read something err"<<std::endl;
		return;
	}
	std::cout<<"����:"<<&(*str)[0]<<std::endl;
	deal_rpc_data(str);
	boost::shared_ptr<std::vector<char>> str2(new std::vector<char>(100,0));
	_sock->async_read_some(boost::asio::buffer(*str2),boost::bind(&TcpConnection::read_handler,this,boost::asio::placeholders::error,str2));
}
//rpc server service
void TcpConnection::addService(google::protobuf::Service *serv){
	//������ʹ�ó������͵Ĳ��������ǿ���ʹ�ó������͵�������Ϊ������
	//���ֱ��ʹ�ó������Ͱ�ֵ���ݣ�����Ҫ����һ���µ�ʵ�����鲻��
	//����ǰ����ô��ݣ���ô����Ҫ�����µģ�ֱ�Ӷ��󴫽�����
	rpcServices.push_back(serv);
}

void TcpConnection::deal_rpc_data(boost::shared_ptr<std::vector<char>> str){
	echo::EchoRequest request;
	int fid = (*str)[0] - '0';
	request.ParseFromString(&(*str)[1]);
	rpcServices[0]->CallMethod(rpcServices[0]->GetDescriptor()->method(fid),NULL,&request,NULL,NULL);
}
sock_pt TcpConnection::getSocket(){
	return _sock;
}


void TcpConnection::CallMethod(const google::protobuf::MethodDescriptor* method,
                          google::protobuf::RpcController* controller,
                          const google::protobuf::Message* request,
                          google::protobuf::Message* response,
                          google::protobuf::Closure* done){
							  char c[2];
							  c[0]= '0'+method->index();//����func id��һ�����������ơ�
							  c[1] = 0;
							  std::string cstr = std::string(c);
							  cstr+=request->SerializeAsString();
							  sendMessage(cstr);
							  //done->Run();
}


TcpServer::TcpServer(boost::asio::io_service & io):m_ios(&io),acceptor(io,boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),6688)){
	m_waitCon = NULL;
	_start();
}

void TcpServer::_start(){
	m_waitCon = new TcpConnection(*m_ios);
	m_waitCon->addService(new EchoBackImplService(m_waitCon));//con��service�ľ����
	//Ŀǰֻ�ܽ���һ������
	acceptor.async_accept(*m_waitCon->getSocket(), boost::bind(&TcpServer::accept_hander,this,boost::asio::placeholders::error));//TODO bind����
}

//TCP���ӷ���ʱ���ص�����
void TcpServer::accept_hander(const boost::system::error_code & ec)
{
	std::cout<<"client is connected!"<<std::endl;
	boost::shared_ptr<std::vector<char>> str(new std::vector<char>(100,0));
	m_waitCon->getSocket()->async_read_some(boost::asio::buffer(*str),boost::bind(&TcpConnection::read_handler,m_waitCon,boost::asio::placeholders::error,str));
	_start();
	//TODO �����µ����ӣ���Ҫ����һ���µ�socket
	//acceptor.async_accept(*_sock, boost::bind(&TcpServer::accept_hander,this,boost::asio::placeholders::error));
}





TcpClient::TcpClient(boost::asio::io_service & io):m_con(new TcpConnection(io)),ep(ip::address::from_string("127.0.0.1"),6688){
	//sock_pt sock(new ip::tcp::socket(ios));
	m_con->addService(new EchoImplService(m_con));
	m_con->getSocket()->async_connect(ep,boost::bind(&TcpClient::conn_hanlder,this,boost::asio::placeholders::error,m_con));
}
//���ӳɹ��ص�����
void TcpClient::conn_hanlder(const boost::system::error_code & ec,TcpConnection * con){
	if (ec){
		std::cout<<"connect failed"<<std::endl;
		return;
	}
	boost::shared_ptr<std::vector<char>> str(new std::vector<char>(100,0));
	std::cout<<"connect success"<<std::endl;
	con->getSocket()->async_read_some(buffer(*str),boost::bind(&TcpConnection::read_handler,con,boost::asio::placeholders::error,str));
}

TcpConnection * TcpClient::getConnection(){
	return m_con;
}