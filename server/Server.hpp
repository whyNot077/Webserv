#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/_types/_intptr_t.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include <sys/event.h>
#include <istream>
#include <vector>
#include <map>
#include "Router.hpp"

class Server
{
private:
	static Server* instance;
private:
	int socket_fd;
	int kqueue_fd;
	std::vector<struct kevent> IOchanges;
	std::vector<struct kevent> IOevents;
	std::map<int, Router> sockets;
	std::map<int, Router*> pipes;
    std::map<std::string, std::string> cookies;
private:
	Server();
	Server(const int port, const char* host);
	Server(const Server& copy);
	Server& operator=(const Server& copy);
public:
	~Server();
private:
    void receiveBuffer(const int client_sockfd);
    void writeToFile(const char* buf);
    void processRequest(const std::string& buf, const int client_sockfd);
	void addIOchanges(uintptr_t ident, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata);
	void disconnect(const int client_sockfd);
	void sendBuffer(const int client_sockfd, const intptr_t bufSize);
	in_addr_t IPToInt(const std::string& ip) const;
public:
	static Server& getInstance(const int port, const char* host);
	void createSocket();
	void setSocketOptions();
	void bindSocket(int port, const char* host);
	void listenSocket();
	void acceptConnection();
	void addFd(void);
	void waitEvents(void);
	void addPipes(const int writeFd, const int readFd, Router* const router);
    int getRequestError(const int client_sockfd);
    void addCookie(const std::string& key, const std::string& value);
    const std::string& getCookie(const std::string& key);
};

#endif