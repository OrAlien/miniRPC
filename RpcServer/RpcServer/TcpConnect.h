#pragma once
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio/io_service.hpp>
#include <vector>
#include <google/protobuf/service.h>


using namespace boost::asio;
typedef boost::shared_ptr<boost::asio::ip::tcp::socket> sock_pt;
class TcpConnection:
	public google::protobuf::RpcChannel
{
public:
	TcpConnection(boost::asio::io_service & io);
	~TcpConnection();
	//��������
	void sendMessage(std::string str);

	//�������ݻص�
	void write_handler(const boost::system::error_code &);
	//���յ�����
	void read_handler(const boost::system::error_code& ec,boost::shared_ptr<std::vector<char>> str);
	
	//rpc server service
	void addService(google::protobuf::Service *serv);
	sock_pt getSocket();

	void CallMethod(const MethodDescriptor* method,
                          RpcController* controller,
                          const Message* request,
                          Message* response,
                          Closure* done);

private:
	sock_pt _sock;
	std::vector<google::protobuf::Service*> rpcServices;
	//����rpc string��
	void deal_rpc_data(boost::shared_ptr<std::vector<char>> str);
};




class TcpServer
{
public:
	TcpServer(boost::asio::io_service & io);

private:
	boost::asio::ip::tcp::acceptor acceptor;
	std::vector<TcpConnection *> m_cons;//����
	TcpConnection * m_waitCon;
	boost::asio::io_service m_ios;

	void _start();

	//TCP���ӷ���ʱ���ص�����
	void accept_hander(const boost::system::error_code & ec);
};

class TcpClient
{
public:
	TcpClient(io_service & io);
	
private:
	TcpConnection * m_con;
	ip::tcp::endpoint ep;
	//boost::asio::io_service m_ios;
	void conn_hanlder(const boost::system::error_code & ec,sock_pt sock);
};