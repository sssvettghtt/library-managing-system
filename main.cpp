#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include <windows.h>

class LibraryExceptions : public std::exception
{
    std::string msg;

public:
    LibraryExceptions(const std::string &m) : msg(m) {}
    const char *what() const noexcept override
    {
        return msg.c_str();
    }
};

class Book
{
    std::string author, title, genre, description, isbn;
    int year;
    std::vector<std::string> tags;
    double rating;

public:
    Book(const std::string &a, const std::string &t, const std::string &g, const std::string &desc, int y, const std::vector<std::string> &tg,
         double r, const std::string &id)
        : author(a), title(t), genre(g), description(desc), year(y), tags(tg), rating(r), isbn(id) {}

    std::string getTitle() const { return title; }
    std::string getAuthor() const { return author; }
    std::string getGenre() const { return genre; }
    std::string getDescription() const { return description; }
    std::string getISBN() const { return isbn; }
    int getYear() const { return year; }
    double getRating() const { return rating; }
    std::vector<std::string> getTags() const { return tags; }

    void setRating(double r)
    {
        if (r >= 0 && r <= 5)
            rating = r;
    }

    void printShort() const
    {
        std::cout << title << " by " << author << "[" << genre << "] ISBN: " << isbn << std::endl;
    }

    void printInFull() const
    {
        std::cout << "Title: " << title
                  << "\n Author: " << author
                  << "\n Genre: " << genre
                  << "\n Year: " << year
                  << "\n Rating: " << rating
                  << "\n ISBN: " << isbn
                  << "\n Description: " << description
                  << "\n Tags: ";
        for (const std::string &tag : tags)
        {
            std::cout << tag << " ";
        }
        std::cout << std::endl;
    }

    void writeToFile(std::ostream &file) const
    {
        file << "BOOK| "
             <<title << "|"
             << author << "|"
             << genre << "|"
             << description << "|"
             << year << "|"
             << rating << "|"
             << isbn << "|";

        for (size_t i = 0; i < tags.size(); i++)
        {
            file << tags[i];
            if (i != tags.size() - 1)
                file << ", ";
        }

        file << std::endl;
    }
};

class User
{
private:
    std::string username;
    std::string password;

public:
    User(const std::string &u, const std::string &p) : username(u), password(p) {}
    virtual ~User() {};

    std::string getUsername() const { return username; }
    std::string getPassword()const{ return password;}
    bool checkPassword(const std::string &input) const
    {
        return password == input;
    }

    virtual void writeToFile(std::ostream& file)const=0;
    virtual bool isAdmin() const = 0;
};

class Client : public User
{
public:
    Client(const std::string &u, const std::string &p) : User(u, p) {}

    bool isAdmin() const override
    {
        return false;
    }

    void writeToFile(std::ostream& file)const override{
        file<<"USER|"
            <<getUsername()<<"|"
            <<getPassword()<<"|"
            <<std::endl;
    }
};

class Admin : public User
{
public:
    Admin(const std::string &u, const std::string &p) : User(u, p) {}

    bool isAdmin() const override
    {
        return true;
    }

     void writeToFile(std::ostream& file)const override{
        file<<"ADMIN|"
            <<getUsername()<<"|"
            <<getPassword()<<"|"
            <<std::endl;
    }
};

class Library
{
    std::vector<Book> books;
    std::map<std::string, User *> users;
    User *currentUser = nullptr;
    std::string currentFilePath = "";
    bool isFileOpen = false;

    // helpers
    std::string toLower(std::string str)
    {
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        return str;
    }

    void clearData()
    {
        books.clear();
        //auto also work
        for(std::pair<const std::string, User*>& pair: users) delete pair.second;

        users.clear();
        currentUser = nullptr;
        currentFilePath = "";
        isFileOpen = false;
    }

    void requireOpenFile() const
    {
        if (!isFileOpen)
            throw LibraryExceptions("No file is currently open!");
    }

    void requireLogIn() const
    {
        if (currentUser == nullptr)
            throw LibraryExceptions("No users logged in.");
    }

    void requireAdmin() const
    {
        requireLogIn();

        if (!currentUser->isAdmin())
            throw LibraryExceptions("Admin access required.");
    }

    std::vector<std::string> split (const std::string& str, char delimiter){
        std::vector<std::string> result;
        std::stringstream ss(str);
        std::string token;

        while(getline(ss,token, delimiter)){
            result.push_back(token);
        }

        return result;
    }
public:
    Library() = default; // no magic numbers
    ~Library()
    {
        for (std::pair<const std::string, User*>& pair : users)
        {
            delete pair.second;
        }
    }

    // no parsing
    void open(const std::string &path);
    void close();
    void save();
    void saveAs(const std::string &path);
    void help();
    void run();

    void logIn();
    void logOut();
    void addUser();
    void removeUser();

    void booksAll();
    void booksInfo(std::string isbn);
    void booksFind(std::string option, std::string query);
    void booksSort(std::string option, std::string order);
    void booksAdd();
    void booksRemove();

    void run();
};

void Library::open(const std::string &path)
{
    if (isFileOpen)
    {
        clearData();
    }

    std::ifstream file(path);

    if (!file)
    {
        std::ofstream newFile(path);

        if (!newFile)
        {
            throw LibraryExceptions("Could not create file.");
        }

        newFile.close();

        file.open(path);
    }

    std::string line;
    while(getline(file, line)){
        if(users.empty()){
            users["admin"]= new Admin("admin", "i<3c++"); 
        }

        std::vector<std::string> token=split(line, '|');
    }

    file.close();

    currentFilePath = path;
    isFileOpen = true;

    std::cout << "Successfully opened " << path << std::endl;
}

void Library::close() {
    requireOpenFile();
    clearData();
    std::cout<<"File closed succsesfully!"<<std::endl;
}

void Library::save(){
    requireOpenFile();
    std::ofstream file(currentFilePath);

    if(!file){
        throw LibraryExceptions("Could not open file for saving.");
    }

    for(const Book& book:books){
        book.writeToFile(file);
    }

    for(const std::pair<const std::string, User*>& pair:users){
        pair.second->writeToFile(file);
    }

    file.close();

    std::cout<<"Successfully saved!"<<currentFilePath<<std::endl;
}

void Library::saveAs(const std::string& path){
    requireOpenFile();

    currentFilePath=path;

    save();

    std::cout<<"Successfully saved as "<<path<<std::endl;
}

void Library:: help(){
    std::cout<<"=============================="<<std::endl;
    std::cout<<"        LIBRARY SYSTEM        "<<std::endl; 
    std::cout<<"=============================="<<std::endl;

    std::cout<<"\nFILE COMANDS(Команди за файлове: ):"<<std::endl;
    std::cout<<"open <file>     -opens file(Отваря файл);"<<std::endl;
    std::cout<<"\nUSER COMANDS(Команди за потребителя: ):"<<std::endl;
    std::cout<<"\nBOOK COMANDS(Команди за книгите: ):"<<std::endl;

  
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    try{
        Library lib1;
        lib1.open("books.txt");

        lib1.close();
    }
    catch(const std::exception& e){
        std::cout<<e.what()<<std::endl;
    }

    return 0;
}