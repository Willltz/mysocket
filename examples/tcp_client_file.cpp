#include "../Socket.hpp"
#include <iostream>
using namespace std;

#define IP   "127.0.0.1"
#define PORT 10010

//实现文件传输
int main()
{
    TCP client;
    client.connect_to(Address(IP, PORT));
    cout << "sending ..." << endl;
    bool flag_send=false;
    flag_send=client.send_file("in.jpg");
    return 0;
}
