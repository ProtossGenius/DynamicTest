#ifndef INTERFACES_H_BGNUUIYT
#define INTERFACES_H_BGNUUIYT
#include <boost/asio.hpp>
/*this file provide some functhion witch need you achieve.*/
namespace smdtest{
	typedef  boost::asio::ip::tcp::socket socket;
	void* readPkg(socket& sock, boost::system::error_code& ec);
	void  sendPkg(socket& sock, void* pkg, boost::system::error_code& ec);
}

#endif /* end of include guard: INTERFACES_H_BGNUUIYT */
