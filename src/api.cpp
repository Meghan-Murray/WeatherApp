#include "api.h"
#include "json.h"
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <netinet/tcp.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/types.h>
#include <stdio.h>
#include <bits/stdc++.h>

int socket_connect(char * host, in_port_t port)
{
    struct hostent * hp;
    struct sockaddr_in addr;
    int on = 1, sock;

    if((hp = gethostbyname(host)) == NULL) {
        herror("gethostbyname");
        exit(1);
    }
    bcopy(hp->h_addr, &addr.sin_addr, hp->h_length);
    addr.sin_port   = htons(port);
    addr.sin_family = AF_INET;
    sock            = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char *)&on, sizeof(int));

    if(sock == -1) {
        perror("setsockopt");
        exit(1);
    }

    if(connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {
        perror("connect");
        exit(1);
    }
    return sock;
}

std::string getRequest(char* request)
{
    int fd;
    char buffer[BUFFER_SIZE];
    fd = socket_connect((char*)URL, 80);
    write(fd, request, strlen(request));
    bzero(buffer, BUFFER_SIZE);

    std::string result;
    while(read(fd, buffer, BUFFER_SIZE - 1) != 0) {
		result.append(buffer);
        bzero(buffer, BUFFER_SIZE);
    }
	
	result = result.substr(result.find('{'));

  //  std::cout << result << std::endl;
    shutdown(fd, SHUT_RDWR);
    close(fd);
	return result;
}

std::string getInfoOfCity(const char* name)
{
	//if (strlen(name) > 60) 
	char request[BUFFER_SIZE] = "GET /v1/current.json?key=" APIKEY "&q=";
	strcat(request, name);
	strcat(request, " HTTP/1.1\r\nHost: " URL "\r\nConnection: close\r\n\r\n");
    std::string jsonText = getRequest(request);

	return jsonText;
}