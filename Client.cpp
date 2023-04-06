//CSV library
#include "/home/rus/Desktop/TestTask/Load.cpp"

//Standart libraries
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <map>

//Boost libraries, linking key: -lboost_system -pthread
#include </usr/include/boost/asio.hpp>
#include </usr/include/boost/asio/ts/buffer.hpp>
#include </usr/include/boost/asio/ts/internet.hpp>

boost::asio::io_context service;
boost::system::error_code ec;
boost::asio::ip::tcp::endpoint ep(boost::asio::ip::make_address("localhost", ec), 1110);

void Query(std::string msg){
    boost::asio::ip::tcp::socket sock(service);
    sock.connect(ep);

    std::cout << msg << std::endl;

    sock.write_some(boost::asio::buffer(msg));
    
    sock.wait(sock.wait_read);
    size_t bytes = sock.available();  

    std::vector<char> Buff(bytes);
    sock.read_some(boost::asio::buffer(Buff.data(), Buff.size()), ec);
    
    std::cout << "Result:\n";

    for(auto c: Buff)
        std::cout << c;

    std::cout << "\n\n";
    
    sock.close();
}

int main(){
    Query("show|/home/rus/Desktop/TestTask/covid_worldwide.csv");
    
    return 0;
}
