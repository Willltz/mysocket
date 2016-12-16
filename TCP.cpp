#ifndef _TCP_CPP_
#define _TCP_CPP_

#include <iostream>
#include <malloc.h>
#include "Socket.hpp"
using namespace std;

TCP::TCP():CommonSocket(SOCK_STREAM)
{
}
TCP::TCP(const TCP& tcp)
{
    this->_socket_id = tcp._socket_id;
    this->is_open = tcp.is_open;
    this->is_bind = tcp.is_bind;
}
Ip TCP::ip()
{
    return this->_address.ip();
}
Port TCP::port()
{
    return this->_address.port();
}
Address TCP::address()
{
    return Address(this->_address);
}
void TCP::listen_on_port(Port port, unsigned int listeners = 1)
{
    //cout<<"t1 !"<<endl;
    CommonSocket::listen_on_port(port);
    //cout<<"t2 !"<<endl;
    if(listen(this->_socket_id, listeners)==-1)
        error_handling("listen() error !");
}
void TCP::connect_to(Address address)
{
    if(is_bind) error_handling("already bind !");
    if(!is_open) this->open();
    if(connect(this->_socket_id, (struct sockaddr*)&address, sizeof(struct sockaddr_in))==-1)
        error_handling("connect() error !");
    this->is_bind = true;
}
TCP TCP::accept_client()
{
    TCP clnt;
    socklen_t len = sizeof(struct sockaddr_in);
    clnt.close();
    clnt._socket_id = accept(this->_socket_id, (struct sockaddr*)&clnt._address, &len);
    if(clnt._socket_id==-1)
        error_handling("accept() error !");
    cout<<clnt._socket_id<<endl;
    return clnt;
}
template <class T>
int TCP::Send(const T* buffer, size_t len)
{
    len = len*sizeof(T);
    if (len > (SOCKET_MAX_BUFFER_LEN * sizeof(T)))
        error_handling("the length of data is out of limit !");
    int ret;
    if ((ret = send(this->_socket_id, (const char*)buffer, len, 0)) == -1)
        error_handling("Send() error !");
    return ret;
}
template <class T>
int TCP::Recv(T* buffer, size_t len)
{
    len *= sizeof(T);
    if (len > (SOCKET_MAX_BUFFER_LEN * sizeof(T)))
        error_handling("the length of data is out of limit !");
    int ret;
    if ((ret = recv(this->_socket_id, buffer, len, 0)) == -1)
        error_handling("Recv() error !");
    //cout<<"receive success !"<<endl;
    return ret;
}
int TCP::Write(char* buffer, size_t len)
{
    if (len > SOCKET_MAX_BUFFER_LEN)
        error_handling("the length of data is out of limit !");
    int ret=0;
    if ((ret = write(this->_socket_id, buffer, len)) == -1)
        error_handling("Write() error !");
    return ret;
}
int TCP::Read(char *buffer, size_t len)
{
    //if (len > SOCKET_MAX_BUFFER_LEN)
        //error_handling("the length of data is out of limit !");
    int ret=0;
    if ((ret = read(this->_socket_id, buffer, len)) == -1)
        error_handling("Read() error !");
    /*int idx=0, read_len=0;
    while(read_len=read(this->_socket_id, &buffer[idx++], 1))
    {
        if(read_len == -1) error_handling("Read() error !");
        ret += read_len;
    }*/
    return ret;
}
bool TCP::send_file(string file_name)
{
    unsigned long long file_size;
    char chunk[SOCKET_MAX_BUFFER_LEN];
    char sync;
    fstream fp(file_name.c_str(), ios::in | ios::binary);
    if (!fp.is_open())
        error_handling("Cannot open the file !");
    fp.seekg(0, ifstream::end);
    file_size = fp.tellg();
    fp.seekg(0, ifstream::beg);
    this->Send<unsigned long long>(&file_size, 1);
    for (unsigned long long i = 0; i < file_size / SOCKET_MAX_BUFFER_LEN; i++)
    {
        this->Recv<char>(&sync, 1);
        fp.read(chunk, SOCKET_MAX_BUFFER_LEN);
        this->Send<char>(chunk, SOCKET_MAX_BUFFER_LEN);
    }
    if ((file_size % SOCKET_MAX_BUFFER_LEN) != 0)
    {
        this->Recv<char>(&sync, 1);
        fp.read(chunk, file_size % SOCKET_MAX_BUFFER_LEN);
        this->Send<char>(chunk, file_size % SOCKET_MAX_BUFFER_LEN);
    }
    fp.close();
    return true;
}
bool TCP::receive_file(string file_name)
{
    unsigned long long file_size;
    char chunk[SOCKET_MAX_BUFFER_LEN];
    char sync;
    fstream fp(file_name.c_str(), ios::out | ios::binary);
    if (!fp.is_open())
        error_handling("Cannot open the file !");
    this->Recv<unsigned long long>(&file_size, 1);
    for(unsigned long long i = 0; i < file_size / SOCKET_MAX_BUFFER_LEN; i++)
    {
        this->Send<char>(&sync, 1);
        this->Recv<char>(chunk, SOCKET_MAX_BUFFER_LEN);
        fp.write(chunk, SOCKET_MAX_BUFFER_LEN);
    }
    if ((file_size % SOCKET_MAX_BUFFER_LEN) != 0)
    {
        this->Send<char>(&sync, 1);
        this->Send<char>(chunk, file_size % SOCKET_MAX_BUFFER_LEN);
        fp.write(chunk, file_size % SOCKET_MAX_BUFFER_LEN);
    }
    fp.close();
    return true;
}
#endif















