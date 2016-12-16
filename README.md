# mysocket
使用c++对socket进行了封装，并实现了TCP的socket通信。
tcp_client.cpp和tcp_sever.cpp实现了字符串的传输。
tcp_client_file.cpp和tcp_sever_file.cpp实现了文件的传输。

下一步会实现同时传输文件和字符串（现在会有缓冲区覆盖的问题，导致read错误），并实现udp通信方式。
