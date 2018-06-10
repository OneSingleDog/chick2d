#include"boost.h"
#include"msg.h"
#include"Game.h"

#define server_ip "127.0.0.1"
#define server_port 8001

static Game chick2d;

io_service service;
ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::address::from_string(server_ip), server_port));

const size_t s_c_size = sizeof(s_c_msg);
const size_t c_s_size = sizeof(c_s_msg);

class talk_to_client: public boost::enable_shared_from_this<talk_to_client>, boost::noncopyable {
	typedef talk_to_client self_type;

private:
	ip::tcp::socket sock_;
	char read_buffer_[c_s_size];
	char write_buffer_[s_c_size];
	bool started_;
	std::string username_;
	int id_;

	talk_to_client(int id):sock_(service), started_(false), username_("NULL"), id_(id){}

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

	static ptr new_(int id) { ptr new_(new talk_to_client(id)); return new_; }

	void stop() {
		if (!started_) return;
		started_ = false;
		sock_.close();
		}

	bool started() const { return started_; }
	ip::tcp::socket & sock() { return sock_; }
	std::string username() const { return username_; }
	int id() const { return id_; };

private:
	void on_read(const error_code & err, size_t bytes) {
		if (err) stop();
		if (!started()) return;
		c_s_msg m=*((c_s_msg*)(read_buffer_));
		if (username_=="NULL")
			{
			username_ = chick2d.login(m,id_);//登陆处理
			do_write(chick2d.info(id_));
			}
		else
			{
			chick2d.merge(m,id_);//合并信息块
			do_write(chick2d.info(id_));//当前信息
			}
		}

	void on_write(const error_code & err, size_t bytes)
		{
		if(chick2d.alive(id_))do_read();//是否继续连接
		else stop();
		return;
		}

	void do_read() {
		async_read(sock_, buffer(read_buffer_), MEM_FN2(read_complete, _1, _2), MEM_FN2(on_read, _1, _2));
		}

	void do_write(const s_c_msg& m) {
		if (!started()) return;
		memcpy_s(write_buffer_, s_c_size, &m, s_c_size);
		sock_.async_write_some(buffer(write_buffer_, s_c_size), MEM_FN2(on_write, _1, _2));
		}
};

int now_opened;
talk_to_client::ptr clients[MAXPLAYER];

void handle_accept(talk_to_client::ptr client, const talk_to_client::error_code & err)
	{
	client->start();
	printf("A player has connected\n");
	talk_to_client::ptr new_client = talk_to_client::new_(now_opened+1);
	clients[now_opened] = new_client;
	if(now_opened<MAXPLAYER)acceptor.async_accept(new_client->sock(), boost::bind(handle_accept, new_client, _1));
	++now_opened;
	}

int main()
{
	while (true)
		{
		chick2d.InitGame();//初始化游戏
		printf("Initial completed\n");
		now_opened = 0;
		clients[now_opened] = talk_to_client::new_(now_opened+1);
		acceptor.async_accept(clients[now_opened]->sock(), boost::bind(handle_accept, clients[now_opened], _1));
		++now_opened;
		service.run();
		for (int i = 0;i<MAXPLAYER;++i)
			{
			clients[i]->stop();
			clients[i].reset();
			}
		chick2d.EndGame();
		service.reset();
		}
	return 0;
}