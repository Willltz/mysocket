#ifndef _SOCKET_HPP_
#define _SOCKET_HPP_

#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <unistd.h>

#define SOCKET_MAX_BUFFER_LEN 1024

using namespace std;

//typedef int SocketId;
typedef string Ip;
typedef unsigned int Port;

void error_handling(string message)
{
    //fputs(message, stderr);
    //fputc('\n', stderr);
    cout<<message<<endl;
    exit(1);
}

class Address : protected sockaddr_in
{
    public:
        Address();
        Address(Port);
        Address(Ip, Port);
        Address(struct sockaddr_in);
        Address(const Address&);

        Ip ip();
        Ip ip(Ip);

        Port port();
        Port port(Port);

        friend ostream& operator<<(ostream&, Address&);
    private:
        void _address(Ip, Port);
};
class CommonSocket
{
    protected:
        
        int _socket_type;//SOCK_STREAM or SOCK_DGRAM
        bool is_open;
        bool is_bind;
    public:
        int _socket_id;
        CommonSocket(){}
        CommonSocket(int socket_type)
        {
            //this->_socket();
            this->_socket_type = socket_type;
            this->is_open = false;
            this->is_bind = false;
            this->open();
        }
        ~CommonSocket(){}

        void open()
        {
            if(this->is_open) return;
            //一定要加括号，即保证先赋值，再判断，否则会先判断再赋值！！！
            if((this->_socket_id = socket(PF_INET, this->_socket_type, 0))==-1)//!!!
            //if((this->_socket_id = socket(PF_INET, SOCK_STREAM, 0))==-1)
                error_handling("socket() error !");
            //cout<<this->_socket_id<<endl;
            this->is_open = true;
            this->is_bind = false;
            //cout<<"open success !"<<endl;
            //cout<<this->_socket_type<<endl;
        }
        void close()
        {
            if(this->is_open)
                shutdown(this->_socket_id, SHUT_RDWR);
                //close(this->_socket_id);
            this->is_open = false;
            this->is_bind = false;
        }
        virtual void listen_on_port(Port port)
        {
            if(is_bind) error_handling("already bind !");
            if(!is_open) this->open();
            Address address(port);
            cout<<this->_socket_id<<endl;
            if(bind(this->_socket_id, (struct sockaddr*)&address, sizeof(address))==-1)
            //if(bind(3, (struct sockaddr*)&address, sizeof(struct sockaddr))==-1)
                error_handling("bind() error !");
            this->is_bind = true;
        }

};
class TCP:public CommonSocket
{
    public:
        TCP();
        TCP(const TCP&);

        Ip ip();
        Port port();
        Address address();

        void listen_on_port(Port, unsigned int);
        void connect_to(Address);

        TCP accept_client();

        template <class T> int Send(const T*, size_t);
        template <class T> int Recv(T*, size_t);
        int Write(char*, size_t);
        int Read(char*, size_t);

        bool send_file(string);
        bool receive_file(string);
    private:
        Address _address;
};
#include "Address.cpp"
#include "TCP.cpp"

#endif

