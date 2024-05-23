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


#define STR_STDIN_LENGTH 1000



char *cookie = NULL;
char *token = NULL;



int is_valid_input(char *str)
{
    for (int i = 0; i < strlen(str); i++)
        if (str[i] == ' ' || str[i] == '\t')
            return 0;       // invalid invalid
    return 1;
}



int is_str_number(char *str)
{
    for (int i = 0; i < strlen(str); i++)
        if (str[i] < '0' || str[i] > '9')
            return 0;       // invalid input
    return 1;
}

int get_res_code(char *response)
{
    return atoi(response + 9);
}





void printAndReadInput(const char* prompt)
{
    printf("%s", prompt); // Print the prompt
    fflush(stdout); // Flush the stdout buffer to ensure prompt is displayed immediately

    char input[100]; // Assuming a maximum input length of 100 characters
    fgets(input, sizeof(input), stdin); // Read input from stdin

    printf("You typed: %s", input); // Print the input
}



char* read_console_input(const char *promt)
{
    char *str = (char *) malloc(STR_STDIN_LENGTH * sizeof(char));
    printf("%s", promt);
    fgets(str, sizeof(str), stdin);


    if (str[strlen(str) - 1] == '\n')
        str[strlen(str) - 1] = '\0';

    return str;
}

char *json_credentials(char *username, char *password)
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



char *json_book_attributes(char *id, char *title, char *author, char *publisher, char *genre, char *page_count)
{
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);

    json_object_set_string(root_object, "id", id);
    json_object_set_string(root_object, "title", title);
    json_object_set_string(root_object, "author", title);
    json_object_set_string(root_object, "publisher", title);
    json_object_set_string(root_object, "genre", title);
    json_object_set_string(root_object, "page_count", title);


    char *serialized_string = json_serialize_to_string_pretty(root_value);
    // afisare JSON: puts(serialized_string);

    // stergere json_free_serialized_string(serialized_string);
    json_value_free(root_value);

    return serialized_string;
}


void register_user()
{
    char *username = read_console_input("username=");
    char *password = read_console_input("password=");

    int is_valid_username = is_valid_input(username);
    int is_valid_password = is_valid_input(password);

    if (is_valid_username == 0 && is_valid_password == 0) {
        printf("ERROR: Username-ul si parola sunt invalide, contin spatii.\n");
        free(username);
        free(password);
        return;
    }


    if (is_valid_username == 0) {
        printf("ERROR: Username-ul este invalid, contine spatii.\n");
        free(username);
        free(password);
        return;
    }


    if (is_valid_password == 0) {
        printf("ERROR: Parola este invalida, contine spatii.\n");
        free(username);
        free(password);
        return;
    }





    char *request_body_json = json_credentials(username, password);

    char *message = compute_post_request("34.246.184.49", "/api/v1/tema/auth/register", "application/json", request_body_json, NULL, NULL);

    int sockfd = open_connection("34.246.184.49", 8080, AF_INET, SOCK_STREAM, 0);

    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);
    

    close_connection(sockfd);

    int res_code = get_res_code(response);

    // afisare raspuns: printf("%s\n", response);


    free(username);
    free(password);

    free(response);
    free(request_body_json);
    free(message);


    if (res_code != 200 && res_code != 201) {
        printf("ERROR: Username-ul este deja luat.\n");
        return;
    }


    printf("SUCCESS: Inregistrare efectuata cu succes.\n");    

}


void login()
{
    char *username = read_console_input("username=");
    char *password = read_console_input("password=");


    int is_valid_username = is_valid_input(username);
    int is_valid_password = is_valid_input(password);

    if (is_valid_username == 0 && is_valid_password == 0) {
        printf("ERROR: Username-ul si parola sunt invalide, contin spatii.\n");
        free(username);
        free(password);
        return;
    }


    if (is_valid_username == 0) {
        printf("ERROR: Username-ul este invalid, contine spatii.\n");
        free(username);
        free(password);
        return;
    }


    if (is_valid_password == 0) {
        printf("ERROR: Parola este invalida, contine spatii.\n");
        free(username);
        free(password);
        return;
    }



    char *request_body_json = json_credentials(username, password);


    int sockfd = open_connection("34.246.184.49", 8080, AF_INET, SOCK_STREAM, 0);

    char *message = compute_post_request("34.246.184.49", "/api/v1/tema/auth/login", "application/json", request_body_json, NULL, NULL);


    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);
    

    close_connection(sockfd);
    int res_code = get_res_code(response);


    // afisare raspuns: printf("%s\n", response);


    free(username);
    free(password);


    free(request_body_json);
    free(message);



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
    
    free(response);

}




void enter_library()
{
    if (cookie == NULL) {
        printf("ERROR: Trebuie sa te loghezi inainte.\n");
        return;
    }


    // afisare token (primit la logare): printf("\nCookie: %s\n", cookie);
    int sockfd = open_connection("34.246.184.49", 8080, AF_INET, SOCK_STREAM, 0);



    char *message = compute_get_request("34.246.184.49", "/api/v1/tema/library/access", cookie, NULL);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    close_connection(sockfd);




    // afisare raspuns: printf("%s\n", response);

    int res_code = get_res_code(response);

    token = strstr(response, "\"token\":");
    token += strlen("\"token\":");

    token = strstr(token, "\"");
    token += strlen("\"");
    
    char *p = strstr(token, "\"");
    if (p != NULL) {
        *p = '\0';
    }


    if (res_code != 200 && res_code != 201) {
        printf("ERROR: Ne pare rau, nu ai putut sa iti accesezi biblioteca.\n");
        free(response);
        return;
    }

    // afisare token (primit la accesul bibliotecii): printf("\nToken: %s\n", token);

  

    printf("SUCCESS: Bine ai revenit in biblioteca ta!.\n");
}



void get_all_books()
{
    if (cookie == NULL) {
        printf("ERROR: Trebuie sa te loghezi inainte.\n");
        return;
    }


    if (token == NULL) {
        printf("ERROR: Trebuie sa iti accesezi bibioteca inainte.\n");
        return;
    }

    int sockfd = open_connection("34.246.184.49", 8080, AF_INET, SOCK_STREAM, 0);

    char *message = compute_get_request("34.246.184.49", "/api/v1/tema/library/books", cookie, token);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    close_connection(sockfd);

    int res_code = get_res_code(response);



    if (res_code != 200 && res_code != 201) {
        printf("ERROR: Ne pare rau, nu ai putut sa iti accesezi biblioteca.\n");
        return;
    }

    char *str_json_response = strstr(response, "[");



    printf("SUCCESS: Uite toate cartile:\n");

    if (str_json_response == NULL) {
        printf("[]\n");
    } else {
        printf("%s\n", str_json_response);
    }

    free(response);

}



void get_book_by_id()
{
    if (cookie == NULL) {
        printf("ERROR: Trebuie sa te loghezi mai intai.\n");
        return;
    }


    if (token == NULL) {
        printf("ERROR: Trebuie sa iti accesezi biblioteca mai intai.\n");
        return;
    }

    char *id = read_console_input("id=");

    
}

void add_book()
{
    if (cookie == NULL) {
        printf("ERROR: Trebuie sa te loghezi mai intai.\n");
        return;
    }


    if (token == NULL) {
        printf("ERROR: Trebuie sa iti accesezi biblioteca mai intai.\n");
        return;
    }

    char *title = read_console_input("title=");
    char *author = read_console_input("author=");
    char *publisher = read_console_input("publisher=");
    char *genre = read_console_input("genre=");
    char *page_count = read_console_input("page_count=");
    



    int valid = 1;
    valid &= is_valid_input(title);
    valid &= is_valid_input(author);
    valid &= is_valid_input(publisher);
    valid &= is_valid_input(genre);
    valid &= is_valid_input(page_count);

    if (valid == 0) {
        printf("ERROR: Input-ul de la tastatura este invalid (contine spatii).\n");
        free(title);
        free(author);
        free(publisher);
        free(genre);
        free(page_count);
        return;
    }


    if (is_str_number(page_count) == 0) {
        printf("ERROR: Atributul introdus pentru numarul de pagini nu este un numar.\n");
        free(title);
        free(author);
        free(publisher);
        free(genre);
        free(page_count);
        return;
    }



    size_t json_size = 100 + strlen(title) + strlen(author) + strlen(publisher) + strlen(genre) + strlen(page_count);

    char *request_body_json = (char *) malloc(json_size * sizeof(char));

    snprintf(request_body_json, json_size,
            "{\n\t\"title\": \"%s\",\n\t\"author\": \"%s\",\n\t\"genre\": \"%s\",\n\t\"page_count\": %s,\n\t\"publisher\": \"%s\"\n}",
            title, author, genre, page_count, publisher);

    // pentru afisare: printf("%s\n", request_body_json);



    int sockfd = open_connection("34.246.184.49", 8080, AF_INET, SOCK_STREAM, 0);
    char *message = compute_post_request("34.246.184.49", "/api/v1/tema/library/books", "application/json",
        request_body_json, cookie, token);

    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);
    

    close_connection(sockfd);

    free(message);
    free(request_body_json);

    free(title);
    free(author);
    free(publisher);
    free(genre);
    free(page_count);

    // afisare raspuns: printf("%s\n", response);

    int res_code = get_res_code(response);
    free(response);


    if (res_code != 200 && res_code != 201) {
        printf("ERROR: Cartea nu a putut fi adaugata.\n");
        return;
    }


    printf("SUCCESS: Cartea a fost adaugata in biblioteca ta.\n");    
}




void delete_book()
{
    if (cookie == NULL) {
        printf("ERROR: Trebuie sa te loghezi mai intai.\n");
        return;
    }


    if (token == NULL) {
        printf("ERROR: Trebuie sa iti accesezi biblioteca mai intai.\n");
        return;
    }


    char *id = read_console_input("id=");

    if (is_valid_input(id) == 0) {
        printf("ERROR: ID-ul introdus este invalid (contine spatii).\n");
        free(id);
        return;
    }

    if (is_str_number(id) == 0) {
        printf("ERROR: ID-ul introdus este invalid (nu este un numar natural intreg).\n");
        free(id);
        return;
    }
    

    
    int sockfd = open_connection("34.246.184.49", 8080, AF_INET, SOCK_STREAM, 0);


    int path_size = 1000;
    char *path = (char *) malloc(path_size * sizeof(char));
    snprintf(path, path_size,  "/api/v1/tema/library/books/:%s", id);


    char *message = compute_delete_request("34.246.184.49", path, cookie, token);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    close_connection(sockfd);

    int res_code = get_res_code(response);


    free(response);


    if (res_code != 200 && res_code != 201) {
        printf("ERROR: Eroare la stergerea cartii. ID-ul cartii nu sa gasteste in biblioteca.\n");
        return;
    }



    printf("SUCCESS: Cartea a fost stearsa cu succes.\n");

}



void logout()
{
    if (cookie == NULL) {
        printf("ERROR: Trebuie sa te loghezi mai intai.\n");
        return;
    }




    int sockfd = open_connection("34.246.184.49", 8080, AF_INET, SOCK_STREAM, 0);

    char *message = compute_get_request("34.246.184.49", "/api/v1/tema/auth/logout", cookie, token);


    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    close_connection(sockfd);
    int res_code = get_res_code(response);


    free(response);

    if (res_code != 200 && res_code != 201) {
        printf("ERROR: Nu te-ai putut deconecta.\n");
        return;
    }




    printf("SUCCESS: Deconectare cu succes.\n");


    cookie = NULL;
    token = NULL;
}



int main()
{
    char command[STR_STDIN_LENGTH];

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
            login();
        else if (strcmp(command, "enter_library") == 0)
            enter_library();
        else if (strcmp(command, "get_books") == 0)
            get_all_books();
        else if (strcmp(command, "get_book") == 0)
            get_book_by_id();
        else if (strcmp(command, "add_book") == 0)
            add_book();
        else if (strcmp(command, "delete_book") == 0)
            delete_book();
        else if (strcmp(command, "logout") == 0)
            logout();
        else if (strcmp(command, "exit") == 0)
            break;
    }



    return 0;
}
