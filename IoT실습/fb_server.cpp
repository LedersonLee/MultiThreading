#include <iostream>
#include <array>
#include <string>
#include <boost/asio.hpp>

#define CLIENT 5

using namespace boost::asio;
using namespace boost::asio::ip;

io_service ioservice;
std::vector<boost::asio::ip::tcp::socket> socket_vector;
std::vector<std::thread> thread_vector;

std::string fizzbuzz(int num){
    std::string return_data;
    if(num%3 == 0 && num%5 ==0)
        return_data="fizzbuzz";
    else if(num%3 ==0)
        return_data="fizz";
    else if(num%5 ==0)
        return_data="buzz";
    else
        return_data=std::to_string(num);
    
    return return_data;
}

void client_handler(tcp::socket *client_socket){
    while(1){
        std::array<char, 1024> reply;
        auto reply_length = client_socket->read_some(buffer(reply, reply.size()));
        std::cout << "reply is: ";
        std::cout.write(reply.data(), reply_length);
        std::cout << std::endl;
        
        int num = std::stoi(std::string(reply.data(), reply_length));

        std::string send_data=fizzbuzz(num);
        std::cout<<"send: "<<send_data<<std::endl;
        client_socket->write_some(buffer(send_data, send_data.length()));     
    }
}

void thread_function(tcp::acceptor* acceptor){
    tcp::socket client_socket(ioservice);

    acceptor->async_accept(client_socket, [&](boost::system::error_code ec)
    {
        if (!ec){
            std::cout<<"새로운 client가 접속하였습니다."<<std::endl;
            client_handler(&client_socket);
        }
    });

    ioservice.run();
}

int main(int argc, char *argv[])
{
    if (argc != 2)
        {
            std::cerr << "사용법: " << argv[0] << " <포트번호>\n";
            return 1;
        }   
    
    tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[1]));
    tcp::acceptor acceptor(ioservice, endpoint);
    acceptor.listen();

    for (int i = 0; i < CLIENT; i++)
    {
        // io_service를 인자로 전달하여 스레드 함수 호출
        std::thread new_thread(thread_function, &acceptor);
        thread_vector.push_back(std::move(new_thread));
    }
    
    for (auto& thd : thread_vector) {
        if (thd.joinable()) {
            thd.join();
        }
    }
    
    return 0;
}
