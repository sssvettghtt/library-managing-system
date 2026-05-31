#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include <limits>
#include <cstdlib>
#include <conio.h>

#include "Book.hpp"
#include "User.hpp"
#include "LibraryExceptions.hpp"

class Library
{
    std::vector<Book> books;
    std::map<std::string, User *> users;
    User *currentUser = nullptr;
    std::string currentFilePath = "";
    bool isFileOpen = false;

    static std::string toLower(std::string str);

    void clearData();

    void requireOpenFile() const;

    void requireLogIn() const;

    void requireAdmin() const;

    std::vector<std::string> split(const std::string &str, char delimiter) const;
    static std::string baseName(const std::string &path);
    void ensureDefaultAdmin();

public:
    Library() = default; // no magic numbers
    ~Library();

    void open(const std::string &path);
    void close();
    void save();
    void saveAs(const std::string &path);

    // raylib ne poddurja bulgarsi za help
    void help();
    void run();
    void exit();
    // login za raylib
    bool logInCredentials(const std::string &inputUsername, const std::string &inputPassword, std::string &message);
    //za raylib addBook, addUser, addBook, remove Book/User
    bool isCurrentUserAdmin() const;

    bool booksAddGui(const std::string &title,
                     const std::string &author,
                     const std::string &genre,
                     const std::string &description,
                     int year,
                     const std::vector<std::string> &tags,
                     double rating,
                     const std::string &isbn,
                     std::string &message);

    void logIn();
    void logOut();
    void addUser(const std::string &newUsername, const std::string &newPassword);
    void removeUser(const std::string &usernameToRemove);
    void booksAll();
    void booksView(const std::string &searchISBN);
    void booksFind(const std::string &option, const std::string &keyword);
    void booksSort(const std::string &option, const std::string &order);
    void booksAdd();
    void booksRemove(const std::string &targetISBN);
};