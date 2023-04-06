//CSV library
#include "/home/rus/Desktop/TestTask/Load.h"

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
#include </usr/include/boost/algorithm/string.hpp>

boost::asio::io_context service; 
boost::system::error_code ec;
boost::asio::ip::tcp::acceptor acceptor(service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 1110));

void handle_connection(Files& a){   
    while(true){
        boost::asio::ip::tcp::socket sock(service);
        acceptor.accept(sock);
        sock.wait(sock.wait_read);
       
        size_t bytes = sock.available();
        std::vector<char> Buff(bytes);
        sock.read_some(boost::asio::buffer(Buff.data(), Buff.size()), ec);
        
        std::string msg = "";

        for(auto c: Buff)
            msg = msg + c;

        if(msg == "get list"){
            msg = a.Get_List();
        }
        else
        if(boost::algorithm::contains(msg, "sort")){
            std::vector<std::string> for_sort = Split(msg, '|');
            std::vector<std::string> Categories;
            
            for(int i = 2; i < for_sort.size(); i++)
                Categories.push_back(for_sort[i]);

            a.Sort(for_sort[1], Categories);
            msg = a.Data[for_sort[1]].CSV_Into_String();
        }
        else
        if(boost::algorithm::contains(msg, "get head")){
            std::vector<std::string> get_header = Split(msg, '|');
            msg = a.Get_Header(get_header[1]);
        }
        else
        if(boost::algorithm::contains(msg, "delete")){
            std::vector<std::string> del = Split(msg, '|');
            a.Delete(del[1]);
            msg = "Deleted!";
        }else
        if(boost::algorithm::contains(msg, "add")){
            std::vector<std::string> add = Split(msg, '|');
            CSV_Table Temporary;
            Temporary.String_Into_CSV(add[2]);
            Temporary.FilePath = add[1];
            a.Add(Temporary);
            msg = "Table is added!";
        }else
        if(boost::algorithm::contains(msg, "show")){
            std::vector<std::string> show = Split(msg, '|');
            msg = a.Show(show[1]);
        }else{ msg = "undefined query"; };

        sock.write_some(boost::asio::buffer(msg));        
        sock.close();
    }
}

int main(){

    //Test tables
    CSV_Table file1("/home/rus/Desktop/TestTask/covid_worldwide.csv");
    CSV_Table file2("/home/rus/Desktop/TestTask/prc_hpi_a__custom_3617733_page_linear.csv");
    CSV_Table file3("/home/rus/Desktop/TestTask/test.csv");

    Files Dataset;

    Dataset.Add(file1);
    Dataset.Add(file2);
    Dataset.Add(file3);

    std::cout << "Server is active!" << std::endl;
    handle_connection(Dataset);
    
    return 0;
}
