#include "../Socket.hpp"
#include <iostream>
#include<stdio.h>
#include <string.h>
using namespace std;

//实现字符串传输
int main()
{
    TCP server;
    server.listen_on_port(10010);
    TCP client = server.accept_client();
    cout << "receiving ..." << endl;
    char buffer[30] = {0};
    int ret = client.Read(buffer, sizeof(buffer));
    //int ret = client.Recv(buffer, sizeof(buffer));//也可以
    cout<<ret<<endl;
    printf("Message from client: %s \n", buffer);
    //puts(buffer);
    return 0;
}
