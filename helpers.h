#ifndef HELPERS_H
#define HELPERS_H

#include <cstdlib>      // exit, atoi, malloc, free
#include <cstdio>
#include <unistd.h>     // read, write, close
#include <cstring>      // memcpy, memset
#include <sys/socket.h> // socket, connect
#include <netinet/in.h> // struct sockaddr_in, struct sockaddr
#include <netdb.h>      // struct hostent, gethostbyname
#include <arpa/inet.h>
#include <string>
#include <iostream>

#define HEADER_TERMINATOR "\r\n\r\n"
#define HEADER_TERMINATOR_SIZE (sizeof(HEADER_TERMINATOR) - 1)
#define CONTENT_LENGTH "Content-Length: "
#define CONTENT_LENGTH_SIZE (sizeof(CONTENT_LENGTH) - 1)

void error(const std::string &msg)
{
    perror(msg.c_str());
    exit(0);
}

void compute_message(std::string &message, const std::string &line)
{
    message += line + "\r\n";
}

int open_connection(const std::string &host_ip, int portno, int ip_type, int socket_type, int flag)
{
    sockaddr_in serv_addr{};
    int sockfd = socket(ip_type, socket_type, flag);
    if (sockfd < 0)
        error("ERROR opening socket");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = ip_type;
    serv_addr.sin_port = htons(portno);
    inet_aton(host_ip.c_str(), &serv_addr.sin_addr);

    if (connect(sockfd, reinterpret_cast<struct sockaddr*>(&serv_addr), sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    return sockfd;
}

void close_connection(int sockfd)
{
    close(sockfd);
}

void send_to_server(int sockfd, const std::string &message)
{
    ssize_t bytes, sent = 0;
    ssize_t total = message.size();

    do
    {
        bytes = write(sockfd, message.c_str() + sent, total - sent);
        if (bytes < 0) {
            error("ERROR writing message to socket");
        }

        if (bytes == 0) {
            break;
        }

        sent += bytes;
    } while (sent < total);
}

std::string receive_from_server(int sockfd)
{
    constexpr size_t BUFLEN = 4096;
    char response[BUFLEN];
    Buffer buffer;
    int header_end = 0;
    int content_length = 0;

    do {
        int bytes = read(sockfd, response, BUFLEN);

        if (bytes < 0){
            error("ERROR reading response from socket");
        }

        if (bytes == 0) {
            break;
        }

        buffer.add(response, static_cast<size_t>(bytes));
        
        header_end = buffer.find(HEADER_TERMINATOR, HEADER_TERMINATOR_SIZE);

        if (header_end >= 0) {
            header_end += HEADER_TERMINATOR_SIZE;
            
            int content_length_start = buffer.find_insensitive(CONTENT_LENGTH, CONTENT_LENGTH_SIZE);
            
            if (content_length_start < 0) {
                continue;           
            }

            content_length_start += CONTENT_LENGTH_SIZE;
            content_length = std::strtol(buffer.get_data() + content_length_start, nullptr, 10);
            break;
        }
    } while (true);
    size_t total = content_length + static_cast<size_t>(header_end);
    
    while (buffer.get_size() < total) {
        int bytes = read(sockfd, response, BUFLEN);

        if (bytes < 0) {
            error("ERROR reading response from socket");
        }

        if (bytes == 0) {
            break;
        }

        buffer.add(response, static_cast<size_t>(bytes));
    }
    buffer.add("", 1);
    return std::string(buffer.get_data());
}

std::string basic_extract_json_response(const std::string &str)
{
    auto pos = str.find("{\"");
    return pos != std::string::npos ? str.substr(pos) : "";
}

#endif // HELPERS_H
