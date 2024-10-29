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


#define STR_STDIN_LENGTH 1000


char *cookie = NULL;
char *token = NULL;

void debug(char *message, char *response)
{   
    printf("cookie %s\n\n", cookie);
    printf("token %s\n\n", token);
    printf("message %s\n\n", message);
    printf("response %s\n\n", response);
}

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

/**
 * extregem codul din raspunsul HTTP, situat imediat dupa textul
 * `HTTP/1.1 `
 * 
 * exemplu: pt `HTTP/1.1 200 OK....`, codul va fi 200
*/
int get_res_code(char *response)
{
    return atoi(response + 9);
}





char* read_console_input(const char *prompt)
{
    char *str = (char *) malloc(STR_STDIN_LENGTH * sizeof(char));
    
    if (prompt != NULL)
        printf("%s", prompt);
    
    fgets(str, STR_STDIN_LENGTH, stdin);
    
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }

    return str;
}






void register_user()
{
    char *username = read_console_input("username=");
    char *password = read_console_input("password=");

    int is_valid_username = is_valid_input(username);
    int is_valid_password = is_valid_input(password);

    if (is_valid_username == 0 && is_valid_password == 0) {
        printf("ERROR: Username-ul si parola sunt invalide, ambele contin spatii.\n");
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



    size_t json_size = 100 + strlen(username) + strlen(password);

    char *request_body_json = (char *) malloc(json_size * sizeof(char));

    snprintf(request_body_json, json_size,
        "{\n\t\"username\": \"%s\",\n\t\"password\": \"%s\"\n}",
        username, password);




    free(username);
    free(password);

    char *message = compute_post_request("34.246.184.49", "/api/v1/tema/auth/register", "application/json", request_body_json, NULL, NULL);

    int sockfd = open_connection("34.246.184.49", 8080, AF_INET, SOCK_STREAM, 0);

    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);
    

    close_connection(sockfd);

    int res_code = get_res_code(response);

    // afisare raspuns: printf("%s\n", response);




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



    size_t json_size = 100 + strlen(username) + strlen(password);

    char *request_body_json = (char *) malloc(json_size * sizeof(char));

    snprintf(request_body_json, json_size,
        "{\n\t\"username\": \"%s\",\n\t\"password\": \"%s\"\n}",
        username, password);


    free(username);
    free(password);

    int sockfd = open_connection("34.246.184.49", 8080, AF_INET, SOCK_STREAM, 0);

    char *message = compute_post_request("34.246.184.49", "/api/v1/tema/auth/login", "application/json", request_body_json, NULL, NULL);


    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    close_connection(sockfd);
    int res_code = get_res_code(response);


    // afisare raspuns: printf("%s\n", response);





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
    
    debug(message, response);

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

    debug(message, response);

    if (res_code != 200 && res_code != 201) {
        printf("ERROR: Ne pare rau, nu ai putut sa iti accesezi biblioteca.\n");
        free(response);
        return;
    }

    // afisare token (primit la accesul bibliotecii): printf("\nToken: %s\n", token);

  

    printf("SUCCESS: Bine ai revenit in biblioteca ta!\n");
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

    debug(message, response);


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
    char *id = read_console_input("id=");

    if (cookie == NULL) {
        printf("ERROR: Trebuie sa te loghezi mai intai.\n");
        free(id);
        return;
    }


    if (token == NULL) {
        printf("ERROR: Trebuie sa iti accesezi biblioteca mai intai.\n");
        free(id);
        return;
    }
    


    if (is_str_number(id) == 0) {
        printf("ERROR: ID-ul este invalid (nu este un numar intreg pozitiv).\n");
        free(id);
        return;
    }


    int path_size = 1000;
    char *url_path = (char *) malloc(path_size * sizeof(char));

    snprintf(url_path, path_size, "/api/v1/tema/library/books/%s", id);


    int sockfd = open_connection("34.246.184.49", 8080, AF_INET, SOCK_STREAM, 0);
    char *message = compute_get_request("34.246.184.49", url_path, cookie, token);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    close_connection(sockfd);

    int res_code = get_res_code(response);

    free(url_path);
    free(id);

    free(message);


    if (res_code != 200 && res_code != 201) {
        printf("ERROR: Nu a fost gasita o carte cu ID-ul respectiv.\n");
        free(response);
        return;
    }


    char *book_json = strstr(response, "{\"id\":");

    printf("SUCCESS: Informatiile despre carte:\n");
    printf("%s\n", book_json);
    free(response);
}

void add_book()
{
    char *title = read_console_input("title=");
    char *author = read_console_input("author=");
    char *publisher = read_console_input("publisher=");
    char *genre = read_console_input("genre=");
    char *page_count = read_console_input("page_count=");
    

    if (cookie == NULL) {
        printf("ERROR: Trebuie sa te loghezi mai intai.\n");
        free(title);
        free(author);
        free(publisher);
        free(genre);
        free(page_count);
        return;
    }


    if (token == NULL) {
        printf("ERROR: Trebuie sa iti accesezi biblioteca mai intai.\n");
        free(title);
        free(author);
        free(publisher);
        free(genre);
        free(page_count);
        return;
    }




    if (is_str_number(page_count) == 0) {
        printf("ERROR: Atributul introdus pentru numarul de pagini nu este un numar intreg pozitiv.\n");
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

    free(title);
    free(author);
    free(publisher);
    free(genre);
    free(page_count);

    int sockfd = open_connection("34.246.184.49", 8080, AF_INET, SOCK_STREAM, 0);
    char *message = compute_post_request("34.246.184.49", "/api/v1/tema/library/books", "application/json",
        request_body_json, cookie, token);

    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);
    int res_code = get_res_code(response);
    debug(message, response);


    puts(response);


    close_connection(sockfd);

    free(message);
    free(request_body_json);



    // afisare raspuns: printf("%s\n", response);

    free(response);


    if (res_code != 200 && res_code != 201) {
        printf("ERROR: Cartea nu a putut fi adaugata.\n");
        return;
    }


    printf("SUCCESS: Cartea a fost adaugata in biblioteca ta.\n");    
}




void delete_book()
{


    char *id = read_console_input("id=");

    if (cookie == NULL) {
        printf("ERROR: Trebuie sa te loghezi mai intai.\n");
        free(id);
        return;
    }


    if (token == NULL) {
        printf("ERROR: Trebuie sa iti accesezi biblioteca mai intai.\n");
        free(id);
        return;
    }

    if (is_str_number(id) == 0) {
        printf("ERROR: ID-ul introdus este invalid (nu este un numar intreg pozitiv).\n");
        free(id);
        return;
    }
    

    


    int path_size = 1000;
    char *url_path = (char *) malloc(path_size * sizeof(char));


    snprintf(url_path, path_size, "/api/v1/tema/library/books/%s", id);


    int sockfd = open_connection("34.246.184.49", 8080, AF_INET, SOCK_STREAM, 0);
    char *message = compute_delete_request("34.246.184.49", url_path, cookie, token);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    close_connection(sockfd);

    int res_code = get_res_code(response);

    free(url_path);
    free(id);

    free(message);
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


    free(message);
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
    char *command;

    while (1) {
        command = read_console_input(NULL);

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


    free(command);
    return 0;
}
