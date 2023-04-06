Project : /home/rus/Desktop/TestTask/Server.o /home/rus/Desktop/TestTask/Load.cpp
	g++ -I/usr/lib/x86_64-linux-gnu/ -lboost_system -pthread /home/rus/Desktop/TestTask/Load.cpp /home/rus/Desktop/TestTask/Server.o -o Project 

/home/rus/Desktop/TestTask/Server.o : /home/rus/Desktop/TestTask/Server.cpp
	g++ -c -I/usr/include/boost/ -lboost_system -pthread /home/rus/Desktop/TestTask/Server.cpp -o /home/rus/Desktop/TestTask/Server.o

/home/rus/Desktop/TestTask/Load.o : /home/rus/Desktop/TestTask/Load.cpp
	g++ -c /home/rus/Desktop/TestTask/Load.cpp -o /home/rus/Desktop/TestTask/Load.o
