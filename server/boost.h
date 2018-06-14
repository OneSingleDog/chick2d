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

class talk_to_client: public boost::enable_shared_from_this<talk_to_client>, boost::noncopyable {
	typedef talk_to_client self_type;

private:
	static const size_t s_c_size = sizeof(s_c_msg);
	static const size_t c_s_size = sizeof(c_s_msg);
	ip::tcp::socket sock_;
	char read_buffer_[c_s_size];
	char write_buffer_[s_c_size];
	bool started_;
	std::string username_;
	int id_;

	talk_to_client();

	size_t read_complete(const boost::system::error_code &err, size_t bytes) {
		if (err) return 0;
		if (bytes>=c_s_size)return 0;
		return c_s_size-bytes;
		}

public:
	typedef boost::system::error_code error_code;
	typedef boost::shared_ptr<talk_to_client> ptr;

	void start() {
		started_ = true;
		do_read();
		}

	static ptr new_() { ptr new_(new talk_to_client()); return new_; }

	void stop();

	bool started() const { return started_; }
	ip::tcp::socket & sock() { return sock_; }
	std::string username() const { return username_; }
	int id() const { return id_; };

private:
	void on_read(const error_code & err, size_t bytes);

	void on_write(const error_code & err, size_t bytes);

	void do_read() {
		async_read(sock_, buffer(read_buffer_), MEM_FN2(read_complete, _1, _2), MEM_FN2(on_read, _1, _2));
		}

	void do_write(const s_c_msg& m) {
		if (!started()) return;
		memcpy(write_buffer_, &m, s_c_size);
		sock_.async_write_some(buffer(write_buffer_, s_c_size), MEM_FN2(on_write, _1, _2));
		}
};


#endif