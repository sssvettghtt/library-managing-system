#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>

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
        file << title << "| "
             << author << "| "
             << genre << "| "
             << description << "| "
             << year << "| "
             << rating << "| "
             << isbn << "| ";

        for (int i = 0; i < tags.size(); i++)
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
    bool checkPassword(const std::string &input) const
    {
        return password == input;
    }

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
};

class Admin : public User
{
public:
    Admin(const std::string &u, const std::string &p) : User(u, p) {}

    bool isAdmin() const override
    {
        return true;
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

public:
    Library() = default; // no magic numbers
    ~Library()
    {
        // for now neka e taka;
        for (auto &pair : users)
        {
            delete pair.second;
        }
    }

    // no parsing
    void open(const std::string &path) {}
    void close() {};
    void save() {};
    void saveAs(const std::string &path) {};
    void help() {};

    void logIn() {};
    void logOut() {};
    void addUser() {};
    void removeUser() {};

    void booksAll() {};
    void booksInfo(std::string isbn) {};
    void booksFind(std::string option, std::string query) {};
    void booksSort(std::string option, std::string order) {};
    void booksAdd() {};
    void booksRemove() {};

    void run() {};
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
    }

    currentFilePath = path;
    isFileOpen = true;

    std::cout << "Successfully opened " << path << std::endl;
}

int main()
{
    try
    {
        Library lib;
        lib.open("books.txt");
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}