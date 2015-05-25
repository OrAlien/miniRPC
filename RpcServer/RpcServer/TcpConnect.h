#pragma once
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio/io_service.hpp>
#include <vector>
#include <google/protobuf/service.h>

using namespace boost::asio;
class TcpConnection{
public:
	TcpConnection(boost::asio::io_service &io);
	//��������
	void sendMessage(std::string str);
	//�������ݻص�
	void write_handler(const boost::system::error_code &);
	//���յ�����
	void read_handler(const boost::system::error_code& ec,boost::shared_ptr<std::vector<char>> str);
	
	//rpc server service
	void addService(google::protobuf::Service *serv);

protected:
	boost::asio::io_service &ios;
	std::vector<google::protobuf::Service*> rpcServices;
	typedef boost::shared_ptr<boost::asio::ip::tcp::socket> sock_pt;
	sock_pt _sock;

	void deal_rpc_data(boost::shared_ptr<std::vector<char>> str);
};


class TcpServer:public TcpConnection
{
public:
	TcpServer(boost::asio::io_service & io);

private:
	boost::asio::ip::tcp::acceptor acceptor;
	void _start();

	//TCP���ӷ���ʱ���ص�����
	void accept_hander(const boost::system::error_code & ec);
};

class TcpClient:public TcpConnection{
public:
	TcpClient(io_service & io);
	
private:
	ip::tcp::endpoint ep;
	void conn_hanlder(const boost::system::error_code & ec);
};