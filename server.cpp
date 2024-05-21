#include "nlohmann/json.hpp"
#include "buffer.h"
#include "helpers.h"

#include <iostream>
#include <string>
#include <vector>

#define HOST "34.246.184.49"
#define PORT "8080"
#define PATH "/api/v1/tema/library/"

using json = nlohmann::json;
using namespace std;










// void to_json(json &j, const Book &b) {
//     j = json{
//         {"id", b.id},
//         {"title", b.title},
//         {"author", b.author},
//         {"genre", b.genre},
//         {"publisher", b.publisher},
//         {"page_count", b.page_count}
//     };
// }


// void from_json(const json &j, Book &b) {
//     j.at("id").get_to(b.id);
//     j.at("title").get_to(b.title);
//     j.at("author").get_to(b.author);
//     j.at("genre").get_to(b.genre);
//     j.at("publisher").get_to(b.publisher);
//     j.at("page_count").get_to(b.page_count);
// }

// void vector_to_json(json &j, const vector<Book> &books) {
//     j = json::array();
//     for (const auto& book : books) {
//         j.push_back({
//             {"id", book.id},
//             {"title", book.title}
//         });
//     }
// }

// void vector_from_json(const json &j, vector<Book> &books) {
//     for (const auto& item : j) {
//         Book b = Book();
//         item.at("id").get_to(b.id);
//         item.at("title").get_to(b.title);
//         books.push_back(b);
//     }
// }




int string_to_int(string &str)
{
    int nr = 0;
    for (auto c: str) {
        if (c < '0' || c > '9')
            return -1;
        nr = nr * 10 + c - '0';
    }

    return nr;
}


void space_trimmer(string &str)
{
    while (str.size() > 0 && str[0] == ' ')
        str.erase(str.begin(), str.begin() + 1);

    while (str.size() > 0 && str[str.size() - 1] == ' ')
        str.pop_back();
}


string read_input_string(string msg)
{
    cout << msg;
    string input_str;
    getline(cin, input_str);
    space_trimmer(input_str);
    return input_str;
}

void registerUser()
{
    // Inputul din consola (stdin)
    string username = read_input_string("username=");
    string password = read_input_string("password=");

    // Ruta de acces
    string path = "/api/v1/tema/auth/register";

    // Tip payload
    string payloadType = "application/json";

    // Construirea obiectului JSON cu datele de înregistrare
    json json_obj = {
        {"username", username},
        {"password", password}
    };

    // Serializarea obiectului JSON în format string (payload)
    string payload = json_obj.dump(4);

    // Deschiderea conexiunii către server
    const int portno = 8080;
    const string host_ipv4 = "34.246.184.49";
    int sockfd = open_connection(host_ipv4, portno, AF_INET, SOCK_STREAM, 0);

    // Trimiterea cererii POST către server
    send_to_server(sockfd, "POST " + path + " HTTP/1.1\r\n");
    send_to_server(sockfd, "Host: " + host_ipv4 + "\r\n");
    send_to_server(sockfd, "Content-Type: " + payloadType + "\r\n");
    send_to_server(sockfd, "Content-Length: " + to_string(payload.size()) + "\r\n");
    send_to_server(sockfd, "\r\n");
    send_to_server(sockfd, payload);

    // Primirea răspunsului de la server
    string response = receive_from_server(sockfd);

    // Închiderea conexiunii
    close_connection(sockfd);

    // Verificarea răspunsului de la server și tratarea erorilor
    if (response.find("200") != string::npos || response.find("OK") != string::npos) {
        cout << "SUCCESS: Contul a fost inregistrat cu succes!" << endl;
    } else {
        // Extrage doar textul erorii din răspuns
        string error_text = basic_extract_json_response(response);
        // Afișează textul erorii
        cout << "ERROR: Eroare la inregistrare: " << error_text << endl;
    }
}



void login()
{
    // inputul din consola (stdin)
    string username = read_input_string("username=");
    string password = read_input_string("password=");

    // rust de acces
    string path = "POST /api/v1/tema/auth/login";


    // tip payload
    string payload = "application/json";


    // TODO: POST request 
}


void get_books()
{

}

void get_book()
{

}

void add_book()
{

}

void delete_book()
{

}

void logout()
{

}

void exit()
{

}


void enter_library()
{

}


int main()
{
    string command;


    while (true) {
        command = read_input_string("");

        if (command == "register")
            registerUser();
        else if (command == "login")
            login();
        else if (command == "enter_library")
            enter_library();
        else if (command == "get_books")
            get_books();
        else if (command == "get_book")
            get_book();
        else if (command == "add_book")
            add_book();
        else if (command == "delete_book")
            delete_book();
        else if (command == "logout")
            logout();
        else if (command == "exit") 
            break;
    }

    return 0;
}
