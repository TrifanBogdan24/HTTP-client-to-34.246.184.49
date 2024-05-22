/**
 *
POST /api/v1/tema/auth/login HTTP/1.1
Host: 34.246.184.49:8080
Content-Type: application/x-www-form-urlencoded
Cookie: connect.sid=s%3AKnXqtRkqHLvRlFdhJZIgC8BSQkp4z6nx.q72T5ptizYZu5jtaQNrAx8TbNFt1PlbAjuPfKguOkC4
Content-Length: 27

username=test&password=test
*/


#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>
#include "helpers.h"
#include "parson.h"




char *cookie = NULL;
char *token = NULL;

int get_res_code(char *response)
{
    return atoi(response + 9);
}



void trim(char *str) {
    char *end;

    // Trim leading space
    while (isspace((unsigned char) *str)) str++;

    if (*str == 0) // All spaces?
        return;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char) *end)) end--;

    // Write new null terminator
    *(end + 1) = 0;
}



void printAndReadInput(const char* prompt)
{
    printf("%s", prompt); // Print the prompt
    fflush(stdout); // Flush the stdout buffer to ensure prompt is displayed immediately

    char input[100]; // Assuming a maximum input length of 100 characters
    fgets(input, sizeof(input), stdin); // Read input from stdin

    printf("You typed: %s", input); // Print the input
}




void register_user()
{
    int sockfd = open_connection("34.246.184.49", 8080, AF_INET, SOCK_STREAM, 0);
    char username[50], password[50];
    printf("username=");
    scanf("%s", username);
    printf("password=");
    scanf("%s", password);
    getchar();

    trim(username);
    trim(password);

    char request_body[200];
    sprintf(request_body, "{\"username\":\"%s\",\"password\":\"%s\"}", username, password);
    char message[1024];
    sprintf(message, "POST /api/v1/tema/auth/register HTTP/1.1\r\nHost: %s:%d\r\nContent-Type: application/json\r\nContent-Length: %ld\r\n\r\n%s", "34.246.184.49", 8080, strlen(request_body), request_body);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);
    close_connection(sockfd);


    int res_code = get_res_code(response);

    if (res_code == 200 || res_code == 201) {
        printf("SUCCESS: Utilizator `%s` a fost inregistrat in baza de date.\n", username);
    } else {
        printf("ERROR: Exista deja user-ul `%s`. Alege alt nume.\n", username);
    }

    
    free(response);
}


void login_user()
{
    int sockfd = open_connection("34.246.184.49", 8080, AF_INET, SOCK_STREAM, 0);
    char username[50], password[50];
    printf("username=");
    scanf("%s", username);
    printf("password=");
    scanf("%s", password);
    getchar();

    trim(username);
    trim(password);


    char request_body[200];
    sprintf(request_body, "{\"username\":\"%s\",\"password\":\"%s\"}", username, password);
    char message[1024];
    sprintf(message, "POST /api/v1/tema/auth/login HTTP/1.1\r\nHost: %s:%d\r\nContent-Type: application/json\r\nContent-Length: %ld\r\n\r\n%s", "34.246.184.49", 8080, strlen(request_body), request_body);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);
    close_connection(sockfd);


    char *cookie_start = strstr(response, "Set-Cookie:");
    if (cookie_start != NULL) {
        // Move the pointer to the beginning of the cookie value
        cookie_start += strlen("Set-Cookie:");
        
        // Find the end of the cookie value (next semicolon)
        char *cookie_end = strchr(cookie_start, ';');
        if (cookie_end != NULL) {
            // Calculate the length of the cookie value
            size_t cookie_length = cookie_end - cookie_start;
            
            // Allocate memory for the cookie string
            cookie = (char *) malloc((cookie_length + 1) * sizeof(char));
            
            // Copy the cookie value into the allocated memory
            strncpy(cookie, cookie_start, cookie_length);
            cookie[cookie_length] = '\0'; // Null-terminate the string
            
        }
    }
    
    while (strlen(cookie) > 0 && (cookie[0] == ' ' || cookie[0] == '\t'))
        strcpy(cookie, cookie + 1);


    int res_code = get_res_code(response);
    if (res_code == 200) {
        // strcpy(session_cookie, cookie);
        printf("SUCCESS: Bine ai revenit, %s! ", username);
        printf("Acesta este cookie-ul tau: %s\n", cookie);
    } else {
        printf("ERROR: Nu exista user-ul `%s`!\n", username);
    }
    free(response);
}





void enter_library()
{
    if (cookie == NULL) {
        printf("ERROR: Conecteaza-te inainte a accesa biblioteca.!\n");
        return;
    }


    int sockfd = open_connection("34.246.184.49", 8080, AF_INET, SOCK_STREAM, 0);

    char request_body[200];
    
    // sprintf(request_body, "{\"username\":\"%s\",\"password\":\"%s\"}", username, password);
    char message[1024];
    
    sprintf(message, "GET /api/v1/tema/auth/access HTTP/1.1\r\nHost: %s:%d\r\nContent-Type: application/json\r\nCookie: %s\r\n\r\n",
        "34.246.184.49", 8080, cookie);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);


    printf("%s\n", response);
    
    int res_code = get_res_code(response);
    if (res_code == 200 || res_code == 201) {
        token = (char *) malloc(sizeof(response) * sizeof(char));
        strcpy(token, response);
        printf("SUCCESS: Ai access la biblioteca.!\n");
    } else {
        printf("ERROR: Nu poti accesa biblioteca.!\n");
    }

    close_connection(sockfd);
}



void get_book_by_id()
{
    if (cookie == NULL) {
        printf("Te rog sa te conectezi mai intai.\n");
        return;
    }

    if (token == NULL) {
        printf("Te rog sa iti accesezi biblioteca mai intai.\n");
        return;
    }


    int sockfd = open_connection("34.246.184.49", 8080, AF_INET, SOCK_STREAM, 0);
    char id[50];
    printf("id=");
    getchar();

    trim(id);


    char request_body[200];
    sprintf(request_body, "{\"username\":\"%s\",\"password\":\"%s\"}");
    char message[1024];
    sprintf(message, "POST /api/v1/tema/auth/login HTTP/1.1\r\nHost: %s:%d\r\nContent-Type: application/json\r\nContent-Length: %ld\r\n\r\n%s", "34.246.184.49", 8080, strlen(request_body), request_body);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);
    close_connection(sockfd);


}



void get_all_books()
{

}



int main()
{
    char command[20000];

    while (1) {
        fgets(command, sizeof(command), stdin);

        // citeste de la tastatura urmatoarele nume de comenzi
        /**
         * register
         * login
         * 
         * exit
        */
        if (command[strlen(command) - 1] == '\n')
            command[strlen(command) - 1] = '\0';

        if (strcmp(command, "register") == 0)
            register_user();
        else if (strcmp(command, "login") == 0)
            login_user();
        else if (strcmp(command, "enter_library") == 0)
            enter_library();
        else if (strcmp(command, "exit") == 0)
            break;


    }

    return 0;
}
