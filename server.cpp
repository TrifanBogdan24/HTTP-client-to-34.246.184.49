#include <iostream>
#include "nlohmann/json.hpp"
#include <string>
#include <vector>

#define HOST "34.246.184.49"
#define PORT "8080"
#define PATH "/api/v1/tema/library/"

using json = nlohmann::json;
using namespace std;


class Book {
 public:
    int id;
    string title;
    string author;
    string genre;
    string publisher;
    int page_count;

 public:
    // constructor default
    Book() {}


};



void to_json(json &j, const Book &b) {
    j = json{
        {"id", b.id},
        {"title", b.title},
        {"author", b.author},
        {"genre", b.genre},
        {"publisher", b.publisher},
        {"page_count", b.page_count}
    };
}


void from_json(const json &j, Book &b) {
    j.at("id").get_to(b.id);
    j.at("title").get_to(b.title);
    j.at("author").get_to(b.author);
    j.at("genre").get_to(b.genre);
    j.at("publisher").get_to(b.publisher);
    j.at("page_count").get_to(b.page_count);
}

class User {
 public:
    string username, password;
    vector<Book> library;
    bool library_access;

 public:
    // contructorul default
    User() {}

 public:
    // constructorul cu parametri
    User(const string username, const string password)
    {
        this->username = username;
        this->password = password;

        // by default, la creare, utilizatorul nu isi acceseaza biblioteca
        this->library_access = false;
    }
    
 public:
    void enter_user_library();
    void get_user_books();
    void get_user_book();
    void add_user_book();
    void delete_user_book();
};



class Server {
 public:
    vector<User> users;
    User logged_user;

    // `false` daca niciun user nu este conectat
    // `true` daca un user s-a conectat
    bool is_someone_logged;



public:
    // comenzile serverului
    void registerUser();        // register
    void login();
    void enter_library();
    void get_books();
    void get_book();
    void add_book();
    void delete_book();
    void logout();
    void exitServer();          // exit






public:
    // asundem constructorul clasei
    Server()
    {
        is_someone_logged = false;
    }


};



void Server::registerUser()
{
    string input_username, input_password;

    cout << "username=";
    getline(cin, input_username);

    cout << "password=";
    getline(cin, input_password);

    for (auto user : this->users) {
        if (user.username == input_username) {
            cerr << "ERROR: the username `" << input_username << "` is already taken.\n";
            return;
        }
    }

    cout << "SUCCESS: the user `" << input_username << "` has been registered.\n";
    
    // inregistrarea efectiva a unui utilizator
    User user(input_username, input_password);
    this->users.push_back(user);
}


void Server::login()
{
    string input_username, input_password;

    cout << "username=";
    getline(cin, input_username);

    cout << "password=";
    getline(cin, input_password);


    for (auto user : this->users) {
        if (user.username != input_username)
            continue;

        if (user.password != input_password) {
            cerr << "ERROR: invalid password. try again.\n";
            return;
        }

        if (user.password == input_password) {
            cout << "SUCCESS: user " << input_username << " is loged in\n";
            
            // logarea efectiva a unui utilizator
            this->logged_user = user;
            this->is_someone_logged = true;
            return;
        }


    }

    cerr << "ERROR: invalid username `" << input_username << "`\n";
}






void User::enter_user_library()
{
    this->library_access = true;
    cout << "SUCCESS: Welcome to you library, " << username << ".\n";
}

void Server::enter_library()
{
    if (this->is_someone_logged == false) {
        cerr << "ERROR: no user is logged in the server..\n";
        return;
    }


    logged_user.enter_user_library(); 
}



void User::get_user_books()
{
    if (this->library_access == false) {
        cerr << "ERROR: please enter the library first, " << username << ".\n";
        return;
    }


    // TODO: crearea obiectului JSON
}


void Server::get_books()
{
    if (this->is_someone_logged == false) {
        cerr << "ERROR: no user is logged in the server.\n";
        return;
    }


    logged_user.get_user_books();

}


void User::get_user_book()
{
    if (this->library_access == false) {
        cerr << "ERROR: please enter the library first, " << username << ".\n";
        return;
    }


    int input_id;
    Book book;

    cout << "id=";
    cin >> input_id;


    for (auto book : this->library) {
        if (book.id == input_id) {
            // TODO: crearea unui text JSON cu informatia cartii cu ID-ul dat
            cout << "SUCCESS: \n";
            json json_obj = book;
            cout << json_obj.dump(4) << "\n";      // indentare de 4
            

            return;
        }
    }


    cerr << "ERROR: Invalid book id `" << id << "`.\n";

}

void Server::get_book()
{
    if (this->is_someone_logged == false) {
        cerr << "ERROR: no user is logged in the server.\n";
        return;
    }

    this->logged_user.get_user_book();
}



void User::add_user_book()
{
    if (this->library_access == false) {
        cerr << "ERROR: please enter the library first, " << username << ".\n";
        return;
    }

    string input_title; 
    cout << "title=";
    getline(cin, input_title);

    string input_author;
    cout << "author=";
    getline(cin, input_author);

    string input_genre;
    cout << "genre=";
    getline(cin, input_genre);

    string input_publisher;
    cout << "publisher=";
    getline(cin, input_publisher);

    int input_pg_cnt;
    cout << "page_count=";
    cin >> input_pg_cnt;

    Book book;
    book.author = input_author;
    book.title = input_title;
    book.genre = input_genre;
    book.publisher = input_publisher;
    book.page_count = input_pg_cnt;

    // adaugarea efectiva a cartii in biblioteca utilizatorului logat
    this->library.push_back(book);
}


void Server::add_book()
{
    if (this->is_someone_logged == false) {
        cerr << "ERROR: no user is logged in the server.\n";
        return;
    }

    logged_user.add_user_book();
}


void User::delete_user_book()
{
    if (this->library_access == false) {
        cerr << "ERROR: please enter the library first, " << username << ".\n";
        return;
    }

    int input_id;
    cout << "id = ";
    cin >> input_id;


    
    for (auto itr = this->library.begin(); itr != this->library.end(); itr++) {
        if (itr->id == input_id) {
            cout << "SUCCESS: The book with id `" << input_id << "` has been succesfully deleted!\n";

            // stergerea efectiva a cartii din biblioteca utilizatorului logat
            this->library.erase(itr);
            
            return;
        }
    }

    cout << "ERROR: There is no book with id `" << input_id << "`.\n";
}


void Server::delete_book()
{
    if (this->is_someone_logged == false) {
        cerr << "ERROR: no user is logged in the server.\n";
        return;
    }

    logged_user.delete_user_book();
}


void Server::logout()
{
    if (this->is_someone_logged == false) {
        cerr << "ERROR: no user is logged in the server.\n";
        return;
    }


    cout << "SUCCESS: The user " << this->logged_user.username << " has been log out.\n";
    
    // delogarea efectiva a utilizatorului
    is_someone_logged = false;
}



void Server::exitServer()
{
    exit(EXIT_SUCCESS);
}


int main()
{
    Server server;
    string command;


    while (true) {
        getline(cin, command);

        if (command == "register")
            server.registerUser();
        else if (command == "login")
            server.login();
        else if (command == "enter_library")
            server.enter_library();
        else if (command == "get_books")
            server.get_books();
        else if (command == "get_book")
            server.get_book();
        else if (command == "add_book")
            server.add_book();
        else if (command == "delete_book")
            server.delete_book();
        else if (command == "logout")
            server.logout();
        else if (command == "exit") 
            break;
    }

    return 0;
}
