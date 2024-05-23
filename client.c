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
#include "requests.h"
#include "parson.h"


#define STDIN_STR_LENGTH 1000



char *cookie = NULL;
char *token = NULL;


char *trim(char *str)
{
    char *end;

    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end))
        end--;

    end[1] = '\0';

    return str;
}

int get_res_code(char *response) {
    return atoi(response + 9);
}

int read_string_as_number(char *name) {
    int invalid_str = 1;
    char line[LINELEN];
    char *p;
    while (invalid_str) {
        invalid_str = 0;
        printf("%s", name);
        fgets(line, LINELEN, stdin);
        trim(line);
        p = line;
        while (*p != '\0') {
            if (*p < 48 || *p > 57) {
                printf("Please enter a valid number!\n");
                invalid_str = 1;
                break;
            }
            p++;
        }
    }
    return atoi(line);
}



void printAndReadInput(const char* prompt)
{
    printf("%s", prompt); // Print the prompt
    fflush(stdout); // Flush the stdout buffer to ensure prompt is displayed immediately

    char input[100]; // Assuming a maximum input length of 100 characters
    fgets(input, sizeof(input), stdin); // Read input from stdin

    printf("You typed: %s", input); // Print the input
}


char *json_username_and_password(char *username, char *password)
{
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);

    json_object_set_string(root_object, "username", username);
    json_object_set_string(root_object, "password", password);

    char *serialized_string = json_serialize_to_string_pretty(root_value);
    // afisare JSON: puts(serialized_string);

    // stergere json_free_serialized_string(serialized_string);
    json_value_free(root_value);

    return serialized_string;
}


void register_user()
{
    int sockfd = open_connection("34.246.184.49", 8080, AF_INET, SOCK_STREAM, 0);
    char username[STDIN_STR_LENGTH], password[STDIN_STR_LENGTH];
    printf("username=");
    fgets(username, sizeof(username), stdin);

    printf("password=");
    fgets(password, sizeof(password), stdin);



    if (username[strlen(username) - 1] == '\n')
        username[strlen(username) - 1] = '\0';

    if (password[strlen(password) - 1] == '\n')
        password[strlen(password) - 1] = '\0';

    char *request_body = json_username_and_password(username, password);

    char *message = compute_post_request("34.246.184.49", "/api/v1/tema/auth/register", "application/json", request_body, NULL, NULL);


    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);
    

    close_connection(sockfd);

    // afisare raspuns: printf("%s\n", response);

    int res_code = get_res_code(response);

    if (res_code != 200 && res_code != 201) {
        printf("ERROR: Username-ul este deja luat.\n");
        free(response);
        return;
    }


    printf("SUCCESS: Inregistrare efectuata cu succes.\n");    
    free(response);
}


void login_user()
{
    int sockfd = open_connection("34.246.184.49", 8080, AF_INET, SOCK_STREAM, 0);
    char username[STDIN_STR_LENGTH], password[STDIN_STR_LENGTH];
    printf("username=");
    fgets(username, sizeof(username), stdin);

    printf("password=");
    fgets(password, sizeof(password), stdin);



    if (username[strlen(username) - 1] == '\n')
        username[strlen(username) - 1] = '\0';

    if (password[strlen(password) - 1] == '\n')
        password[strlen(password) - 1] = '\0';

    char *request_body = json_username_and_password(username, password);

    char *message = compute_post_request("34.246.184.49", "/api/v1/tema/auth/login", "application/json", request_body, NULL, NULL);


    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);
    
    // afisare raspuns: printf("%s\n", response);

    close_connection(sockfd);





    int res_code = get_res_code(response);

    if (res_code != 200 && res_code != 201) {
        printf("ERROR: Credentiale invalide.\n");
        free(response);
        return;
    }



    printf("SUCCESS: Logare efectuata cu succes.\n");

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

    
    

    free(response);
}




void enter_library()
{
    int sockfd = open_connection("34.246.184.49", 8080, AF_INET, SOCK_STREAM, 0);

    if (cookie == NULL) {
        printf("ERROR: Trebuie sa te loghezi inainte.\n");
        return;
    }


    // afisare token (primit la logare): printf("\nCookie: %s\n", cookie);

    char *message = compute_get_request("34.246.184.49", "/api/v1/tema/library/access", cookie, NULL);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    close_connection(sockfd);

    // afisare raspuns: printf("%s\n", response);


    int res_code = get_res_code(response);

    if (res_code != 200 && res_code != 201) {
        printf("ERROR: Ne pare rau, nu ai putut sa iti accesezi biblioteca.\n");
        free(response);
        return;
    }

    // afisare token (primit la accesul bibliotecii): printf("\nToken: %s\n", token);

    printf("SUCCESS: Bine ai revenit in biblioteca ta!.\n");
}





int main()
{
    char command[STDIN_STR_LENGTH];

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
