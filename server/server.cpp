#include"boost.h"
#include"Game.h"
#include<string>
#include<iostream>

static Game chick2d;

int server_port = 8001;
int player_num = 4;
int box_num = 20;

io_service service;
ip::tcp::acceptor*acceptor;

int now_opened;
int now_aborted;

talk_to_client::talk_to_client():sock_(service), started_(false), username_("NULL"), id_(){}

void talk_to_client::on_read(const error_code & err, size_t bytes) {
	if (err) stop();
	if (!started()) return;
	c_s_msg m = *((c_s_msg*)(read_buffer_));
	if (username_=="NULL")
		{
		username_ = chick2d.login(m, id_);//login
		do_write(chick2d.info(id_));
		}
	else
		{
		chick2d.merge(m, id_);//receive and handle
		do_write(chick2d.info(id_));//send
		}
	}

void talk_to_client::on_write(const error_code & err, size_t bytes)
	{
	if (chick2d.alive(id_))do_read();//whether continue to connect
	else stop();
	return;
	}

talk_to_client::ptr clients[1024];

void handle_accept(talk_to_client::ptr client, const talk_to_client::error_code & err)
	{
	client->start();
	std::cout<<"A player has connected.\n";
	talk_to_client::ptr new_client = talk_to_client::new_();
	clients[now_opened] = new_client;
	if (now_opened-now_aborted<player_num)
		{
		acceptor->async_accept(new_client->sock(), boost::bind(handle_accept, new_client, _1));
		++now_opened;
		}
	}

void talk_to_client::stop() {
	if (!started_) return;
	started_ = false;
	sock_.close();
	if (username_=="NULL")
		{
		++now_aborted;
		std::cout<<"A player has aborted.\n";
		if (now_opened-now_aborted==player_num-1)
			{
			clients[now_opened] = talk_to_client::new_();
			acceptor->async_accept(clients[now_opened]->sock(), boost::bind(handle_accept, clients[now_opened], _1));
			++now_opened;
			}
		}
	else chick2d.disconnect(id_);
	}

int main(int argc,char**argv)
{
	for (int i = 1;i<argc;++i)
		{
		if (string(argv[i])=="-PORT")
			server_port = std::stoi(string(argv[++i]));
		else if (string(argv[i])=="-PLAYER")
			player_num = std::stoi(string(argv[++i]));
		else if (string(argv[i])=="-BOX")
			box_num = std::stoi(string(argv[++i]));
		}
	if (player_num<2||player_num>5||box_num<10||box_num>20)
		{
		std::cout<<"Invalid parameter!\n";
		return 0;
		}
	acceptor = new ip::tcp::acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), server_port));
	std::cout<<"port:"<<server_port<<" player number:"<<player_num<<" box number:"<<box_num<<std::endl;
	while (true)
		{
		chick2d.InitGame(player_num,box_num);//Initial the game
		printf("Initial completed\n");
		now_opened = 0;
		now_aborted = 0;
		clients[now_opened] = talk_to_client::new_();
		acceptor->async_accept(clients[now_opened]->sock(), boost::bind(handle_accept, clients[now_opened], _1));
		++now_opened;
		service.reset();
		service.run();
		for (int i = 0;i<now_opened;++i)
			{
			clients[i]->stop();
			clients[i].reset();
			}
		chick2d.EndGame();
		service.stop();
		}
	delete acceptor;
	return 0;
}
