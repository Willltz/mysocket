#include "../Socket.hpp"
#include <iostream>
#include<stdio.h>
#include <string.h>
using namespace std;

//实现文件传输
int main()
{
    TCP server;
    server.listen_on_port(10010);
    TCP client = server.accept_client();
    cout << "receiving ..." << endl;
    bool flag_send=false;
    flag_send = client.receive_file("out.jpg");
    return 0;
}
