#ifndef __BOOST_H__
#define __BOOST_H__
#include<boost/shared_ptr.hpp>
#include<boost/enable_shared_from_this.hpp>
#include<boost/bind.hpp>
#include<boost/asio.hpp>

#include"msg.h"

using namespace boost::asio;

#define MEM_FN(x)       boost::bind(&self_type::x, shared_from_this())
#define MEM_FN1(x,y)    boost::bind(&self_type::x, shared_from_this(),y)
#define MEM_FN2(x,y,z)  boost::bind(&self_type::x, shared_from_this(),y,z)

class talk_to_svr: public boost::enable_shared_from_this<talk_to_svr>, boost::noncopyable {
	typedef talk_to_svr self_type;

	private:
	static const size_t s_c_size = sizeof(s_c_msg);
	static const size_t c_s_size = sizeof(c_s_msg);
	ip::tcp::socket sock_;
	char read_buffer_[s_c_size];
	char write_buffer_[c_s_size];
	bool started_;
	std::string username_;
	clock_t last_sent_;

	talk_to_svr(const std::string & username);

	void start(ip::tcp::endpoint ep)
		{
		sock_.async_connect(ep, MEM_FN1(on_connect, _1));
		}

	size_t read_complete(const boost::system::error_code &err, size_t bytes) {
		if (err) return 0;
		if (bytes>=s_c_size)return 0;
		return s_c_size-bytes;
		}

	public:
	typedef boost::system::error_code error_code;
	typedef boost::shared_ptr<talk_to_svr> ptr;

	static void start(ptr& newptr, ip::tcp::endpoint ep, const std::string & username);

	void stop();

	bool started() { return started_; }

	~talk_to_svr(){ stop(); }

	private:
	void on_connect(const error_code & err);

	void on_read(const error_code & err, size_t bytes);

	void on_write(const error_code & err, size_t bytes){
		if (err) stop();
		if (!started()) return;
		do_read();
		}

	private:
	void do_read() {
		async_read(sock_, buffer(read_buffer_), MEM_FN2(read_complete, _1, _2), MEM_FN2(on_read, _1, _2));
		}

	void do_write(const c_s_msg& m) {
		if (!started()) return;
        #ifdef MAC
        memcpy(write_buffer_, &m, c_s_size);
        #else
		memcpy_s(write_buffer_, c_s_size, &m, c_s_size);
        #endif
		sock_.async_write_some(buffer(write_buffer_, c_s_size), MEM_FN2(on_write, _1, _2));
		}

	};

#endif
