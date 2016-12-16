#include "../Socket.hpp"
#include <iostream>
using namespace std;

#define IP   "127.0.0.1"
#define PORT 10010

//实现字符串传输
int main()
{
    TCP client;
    client.connect_to(Address(IP, PORT));
    cout << "sending ..." << endl;
    char word[]="Hello World !";
    int ret = client.Write(word, sizeof(word)); 
    //int ret = client.Send(word, sizeof(word)); //也可以
    cout<<ret<<endl;
    return 0;
}
