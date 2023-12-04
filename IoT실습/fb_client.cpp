#include <iostream>
#include <array>
#include <string>
#include <boost/asio.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;

void run_client(std::string host, short const port)
{
   try
   {
      io_service ioservice;
      tcp::socket tcp_socket(ioservice);
      tcp::resolver resolver(ioservice);
      tcp::resolver::query q{host, std::to_string(port)};

      connect(tcp_socket, resolver.resolve(q));

      while (true)
      {
         std::cout << "number [1-99]: ";

         int number;
         std::cin >> number;
         if (std::cin.fail() || number < 1 || number > 99)
            break;

         auto request = std::to_string(number);
         tcp_socket.write_some(buffer(request, request.length()));

         std::array<char, 1024> reply;
         auto reply_length = tcp_socket.read_some(buffer(reply, reply.size()));

         std::cout << "reply is: ";
         std::cout.write(reply.data(), reply_length);
         std::cout << std::endl;
      }
   }
   catch (std::exception const & e)
   {
      std::cerr << "exception: " << e.what() << std::endl;
   }
}

int main(int argc, char *argv[])
{
   run_client(argv[1], atoi(argv[2]));
}
