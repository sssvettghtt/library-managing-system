#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>


struct Book{
    std::string author, title, genre, description, isbn;
    int year;
    std::vector<std::string> tags;
    double rating;

    void printShort()const{
        std::cout<<title<<" by "<<author<<"["<<genre<<"] ISBN: "<<isbn<<std::endl;
    }

    void printInFull()const{
        std::cout<<"Title: "<<title
        <<"\n Author: "<<author
        <<"\n Genre: "<<genre
        <<"\n Description: "<<description
        <<"\n Year: "<<year
        <<"\n Rating: "<<rating
        <<"\n ISBN: "<<isbn<<std::endl;
    }
};

class User{
    protected:
    std::string username;
    std::string password;

    public:
    User(std::string u, std::string p): username(u), password(p){}
    virtual ~User(){};

    std::string getUsername()const{return username;}
    std::string getPassword()const{return password;}

    virtual bool isAdmin()const=0;

    virtual void printInfo()const{
        std::cout<<"User: "<<username<<"[Role: "<<(isAdmin()? "Admin": "Client") <<"]"<<std::endl;
    }
   
};

class Client: public User{
    public:
    Client(std::string u, std::string p): User(u,p){}
};

class Admin: public User{
    public:
    Admin(std::string u, std::string p): User(u,p){}
    
    bool isAdmin()const override{
        return true;
    }

    void printInfo()const override{
        std::cout<<"===Administrator Account==="<<std::endl;
        User::printInfo();
    }
};

class Library{
    std::vector<Book> books;
    std::map<std::string, User*> users;
    User* currentUser=nullptr;
    std::string currentFilePath="";
    bool isFileOpen=false;

    void free(){};
    bool checkAccess(bool AdminRequired){};

    public:
    Library(){}
    ~Library(){}

    void open(std::string path){}
    void close(){};
    void save(){};
    void saveAs(){};
    void help(){};

    void logIn(){};
    void logOut(){};
    void addUser(){};
    void removeUser(){};

    void booksAll(){};
    void booksInfo(std::string isbn){};
    void booksFind(std::string option, std::string query){};
    void booksSort(std::string option, std::string order){};
    void booksAdd(){};
    void booksRemove(){};

    void run(){};

};

/*void Library::run(){

}*/

int main(){
    return 0;
}