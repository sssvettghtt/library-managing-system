#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include <limits>
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
        file << "BOOK|"
             << title << "|"
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
    std::string getPassword() const { return password; }
    bool checkPassword(const std::string &input) const
    {
        return password == input;
    }

    virtual void writeToFile(std::ostream &file) const = 0;
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

    void writeToFile(std::ostream &file) const override
    {
        file << "USER|"
             << getUsername() << "|"
             << getPassword() << "|0"
             << std::endl;
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

    void writeToFile(std::ostream &file) const override
    {
        file << "ADMIN|"
             << getUsername() << "|"
             << getPassword() << "|1"
             << std::endl;
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
        // auto also work
        for (std::pair<const std::string, User *> &pair : users)
            delete pair.second;

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

    std::vector<std::string> split(const std::string &str, char delimiter)
    {
        std::vector<std::string> result;
        std::stringstream ss(str);
        std::string token;

        while (getline(ss, token, delimiter))
        {
            result.push_back(token);
        }

        return result;
    }

public:
    Library() = default; // no magic numbers
    ~Library()
    {
        for (std::pair<const std::string, User *> &pair : users)
        {
            delete pair.second;
        }
    }

    void open(const std::string &path);
    void close();
    void save();
    void saveAs(const std::string &path);
    void help();
    void run();
    void exit();

    void logIn()
    {

        if (currentUser != nullptr)
        {
            std::cout << "You are already logged in!" << std::endl;
            return;
        }

        std::string inputUsername;
        std::string inputPassword;

        std::cout << "Enter username: ";
        std::cin >> inputUsername;

        std::cout << "Enter password: ";
        std::cin >> inputPassword;

        std::map<std::string, User *>::iterator it = users.find(inputUsername);

        if (it != users.end())
        {
            User *foundUser = it->second;

            if (foundUser->getPassword() == inputPassword)
            {
                currentUser = foundUser;
                std::cout << "Welcome, " << currentUser->getUsername() << "!" << std::endl;
            }
            else
            {
                std::cout << "Invalid username or password!" << std::endl;
            }
        }
        else
        {
            std::cout << "invalid username or password!" << std::endl;
        }
    }

    void logOut()
    {
        if (currentUser == nullptr)
        {
            std::cout << "You ar enot logged in!" << std::endl;
            return;
        }

        std::string exUser = currentUser->getUsername();

        currentUser = nullptr;

        std::cout << exUser << "successfully logged out!" << std::endl;
    }
    void addUser()
    {
        if (currentUser == nullptr)
        {
            std::cout << "You are not logged in!" << std::endl;
            return;
        }

        if (!currentUser->isAdmin())
        {
            std::cout << "Access denied! You cannot add new users!" << std::endl;
            return;
        }

        std::string newUsername;
        std::string newPassword;

        std::cout << "Enter new username: ";
        std::cin >> newUsername;

        std::cout << "Enter new pasword: ";
        std::cin >> newPassword;

        std::map<std::string, User *>::iterator it = users.find(newUsername);

        if (it != users.end())
        {
            std::cout << newUsername << " arlready exists!" << std::endl;
            return;
        }

        users[newUsername] = new Client(newUsername, newPassword);

        std::cout << newUsername << "was successfully registered!" << std::endl;
    }
    void removeUser()
    {
        if (currentUser == nullptr)
        {
            std::cout << "You are not logged in!" << std::endl;
            return;
        }

        if (!currentUser->isAdmin())
        {
            std::cout << "Access denied! You cannot add new users!" << std::endl;
            return;
        }

        std::string usernameToRemove;
        std::cout << "Enter username to remove: ";
        std::cin >> usernameToRemove;

        if (currentUser->getUsername() == usernameToRemove)
        {
            std::cout << "You cannot remove your account while logged in!" << std::endl;
            return;
        }

        std::map<std::string, User *>::iterator it = users.find(usernameToRemove);

        if (it != users.end())
        {
            User *userToDelete = it->second;
            delete userToDelete;

            users.erase(it);

            std::cout << usernameToRemove << " was successfully removed!" << std::endl;
        }
        else
        {
            std::cout << usernameToRemove << " not found!" << std::endl;
        }
    }

    void booksAll()
    {
        if (books.empty())
        {
            std::cout << "The library is currently empty / no file is found!" << std::endl;
            return;
        }

        std::cout << "==============================================" << std::endl;
        std::cout << "          ALL BOOKS IN THE LIBRARY            " << std::endl;
        std::cout << "==============================================" << std::endl;

        std::cout << " ID/ISBN \t | Title \t\t\t | Author \t\t | Genre " << std::endl;
        std::cout << "================================================" << std::endl;
        int counter = 1;
        for (const Book &b : books)
        {
            std::cout << "Book Nº" << counter << "-------" << std::endl;
            b.printInFull();

            std::cout << "----------------------------------------------" << std::endl;
            counter++;
        }
        std::cout << "Books in total: " << books.size() << std::endl;
    }
    void booksView(std::string searchISBN)
    {
        if (books.empty())
        {
            std::cout << "The library is currently empty / no file is found!" << std::endl;
            return;
        }

        bool found = false;
        for (Book &b : books)
        {
            if (b.getISBN() == searchISBN)
            {
                std::cout << "\n ===DETAILED BOOK INFORMATION=== " << std::endl;
                b.printInFull();

                std::cout << "==============================================" << std::endl;
                found = true;
                break;
            }
        }

        if (!found)
        {
            std::cout << "Book with ISBN: " << searchISBN << " not found!" << std::endl;
        }
    }

    void booksFind(std::string option, std::string keyword)
    {
        if (books.empty())
        {
            std::cout << "The library is currently empty / no file is found!" << std::endl;
            return;
        }

        bool foundAny = false;
        int counter = 1;

        std::cout << "\n===SEARCH RESULTS FOR: " << option << "=" << keyword << " ===" << std::endl;

        for (const Book &b : books)
        {
            bool isMatch = false;

            if (option == "title")
            {
                if (b.getTitle() == keyword)
                {
                    isMatch = true;
                }
            }
            else if (option == "author")
            {
                if (b.getAuthor() == keyword)
                {
                    isMatch = true;
                }
            }
            else if (option == "tags")
            {
                std::vector<std::string> bookTags = b.getTags();

                for (const std::string &t : bookTags)
                {
                    if (t == keyword)
                    {
                        isMatch = true;
                        break;
                    }
                }
            }
            else
            {
                std::cout << "Invalid search option! Use 'title', 'author', 'tags'! " << std::endl;
                return;
            }

            if (isMatch)
            {
                std::cout << "\n Match Nº " << counter << ": " << std::endl;
                b.printInFull();
                std::cout << "----------------------------------------------" << std::endl;
                foundAny = true;
                counter++;
            }
        }

        if (!foundAny)
        {
            std::cout << "Book with this criteria was not found!" << std::endl;
        }

        std::cout << "\n==============================================" << std::endl;
    }

    void booksSort(std::string option, std::string order);
    //posle+bonus
    void booksAdd(){
        if (currentUser == nullptr)
        {
            std::cout << "You are not logged in!" << std::endl;
            return;
        }

        if (!currentUser->isAdmin())
        {
            std::cout << "Access denied! You cannot add new users!" << std::endl;
            return;
        }

        std::string newTitle, newAuthor, newGenre, newISBN, newDescription;
        int newYear;
        double newRating;  
        std::vector<std::string> newTags;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout<<"Enter title: ";
        std::getline(std::cin, newTitle);

        std::cout<<"Enter author: ";
        std::getline(std::cin, newAuthor);

        std::cout<<"Enter genre: ";
        std::getline(std::cin, newGenre);

        std::cout<<"Enter year: ";
        std::cin>>newYear;

        std::cout<<"Enter rating (0.0-5.0): ";
        std::cin>>newRating;

        std::cout<<"Enter ISBN: ";
        std::cin>>newISBN;

        for(const Book& b: books){
            if(b.getISBN()==newISBN){
                std::cout<<"Error! This book alredy exists in the system!"<<std::endl;
                return;
            }
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout<<"Enter description: ";
        std::getline(std::cin, newDescription);

        std::cout<<"Enter tags (PRESS ENTER WHEN READY!): ";
        std::string tagsLine;
        std::getline(std::cin, tagsLine);

        //razdelqme reda na otdelni dumi 
        std::stringstream ss(tagsLine);
        std::string tempTag;
        while(ss>>tempTag){
            newTags.push_back(tempTag);
        }

        Book newBook(newAuthor, newTitle, newGenre, newDescription, newYear, newTags, newRating, newISBN);
        books.push_back(newBook);

        std::cout<<"Book " <<newTitle<<" was successfully added!"<<std::endl;
    }
    void booksRemove(std::string targetISBN){
        if (currentUser == nullptr)
        {
            std::cout << "You are not logged in!" << std::endl;
            return;
        }

        if (!currentUser->isAdmin())
        {
            std::cout << "Access denied! You cannot add new users!" << std::endl;
            return;
        }

         if (books.empty())
        {
            std::cout << "The library is currently empty / no file is found!" << std::endl;
            return;
        }

        bool found=false;

        for(std::vector<Book>::iterator it=books.begin(); it!=books.end(); ++it){
            if(it->getISBN()==targetISBN){
                std::string bookTitle=it->getTitle();

                books.erase(it);

                std::cout<<"Book "<<bookTitle<<"(ISBN: "<<targetISBN<<") was successfully removed!"<<std::endl;
                found=true;
                break;
            }
        }

        if(!found){
            std::cout<<"Error Book with this ISBN: "<<targetISBN<<" is not found!"<<std::endl;
        }
    }
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
    while (getline(file, line))
    {
        if (users.empty())
        {
            users["admin"] = new Admin("admin", "i<3c++");
        }

        std::vector<std::string> tokens = split(line, '|');

        if(tokens[0]=="BOOK"){
            if(tokens.size()<9) continue;

            std::string title=tokens[1];
            std::string author=tokens[2];
            std::string genre=tokens[3];
            std::string description=tokens[4];

            int year=std::stoi(tokens[5]);
            double rating=std::stod(tokens[6]);
            std::string isbn=tokens[7];

            std::vector<std::string> tags=split(tokens[8], ',');

            Book newBook(author, title, genre, description, year, tags, rating, isbn);

            books.push_back(newBook);
        }
        else if(tokens[0]=="USER"){
            if(tokens.size()<4) continue;

            std::string username=tokens[1];
            std::string password=tokens[2];

            bool isAdmin=(tokens[3]=="1");

            if(isAdmin){
                users[username]=new Admin(username, password);
            }
            else{
                users[username]= new Client(username, password);
            }
        }
    }

    file.close();

    if(users.empty()){
        users["admin"]=new Admin("admin", "i<3c++");
    }

    currentFilePath = path;
    isFileOpen = true;

    std::cout << "Successfully opened " << path << std::endl;
}

void Library::close()
{
    requireOpenFile();
    clearData();
    std::cout << "File closed succsesfully!" << std::endl;
}

void Library::save()
{
    requireOpenFile();
    std::ofstream file(currentFilePath);

    if (!file)
    {
        throw LibraryExceptions("Could not open file for saving.");
    }

    for (const Book &book : books)
    {
        book.writeToFile(file);
    }

    for (const std::pair<const std::string, User *> &pair : users)
    {
        pair.second->writeToFile(file);
    }

    file.close();

    std::cout << "Successfully saved!" << currentFilePath << std::endl;
}

void Library::saveAs(const std::string &path)
{
    requireOpenFile();

    currentFilePath = path;

    save();

    std::cout << "Successfully saved as " << path << std::endl;
}

void Library::help()
{
    std::cout << "==============================" << std::endl;
    std::cout << "        LIBRARY SYSTEM        " << std::endl;
    std::cout << "==============================" << std::endl;
    std::cout << std::endl;
    std::cout << "\nFILE COMANDS(Команди за файлове: ):" << std::endl;
    std::cout << "open <file>                     -opens file(Отваря файл);" << std::endl;
    std::cout << "close                           -closes current file(зарваря текущ файл);" << std::endl;
    std::cout << "save                            -saves changes(зашазва промени);" << std::endl;
    std::cout << "saveAs <filr>                   -saves in a new file(запазва в нов файл);" << std::endl;
    std::cout << "exit                            -exits the program(излизане от програмата);" << std::endl;

    std::cout << std::endl;
    std::cout << "\nUSER COMANDS(Команди за потребителя: ):" << std::endl;
    std::cout << "log in                          -log into an account(вход в акаунт);" << std::endl;
    std::cout << "log out                         -log out of the system(изход от системата);" << std::endl;
    std::cout << "users add <users> <password>    -add new user[ADMIN ONLY](добавяне на потребител);" << std::endl;
    std::cout << "users remove <users>            -remove user[ADMIN ONLY](премахване на потребител);" << std::endl;

    std::cout << std::endl;
    std::cout << "\nBOOK COMANDS(Команди за книгите: ):" << std::endl;
    std::cout << "books all                       -list all books(списък на всички книги);" << std::endl;
    std::cout << "books info <isbn>               -detailed info for a book(информация за книга);" << std::endl;
    std::cout << "books find <option> <string>    -search by title/author/tag(Търсене на книга);" << std::endl;
    std::cout << "books dort <option> [asc|desc]  -sort books(сортиране на книгите);" << std::endl;
    std::cout << "books add                       -add new book[ADMIN ONLY](Добавяне на книга);" << std::endl;
    std::cout << "books remove <isbn>             -remove book[ADMIN ONLY](премахване на книга);" << std::endl;

    std::cout << "==============================" << std::endl;
}

void Library::exit()
{
    std::cout << "Exiting...." << std::endl;

    //is there a problem here?
    if (currentUser != nullptr)
    {
        delete currentUser;
        currentUser = nullptr;
    }

    ::exit(0);
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    try
    {
        Library lib1;
        lib1.open("books.txt");

        lib1.close();
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}