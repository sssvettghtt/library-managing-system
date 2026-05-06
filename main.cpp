#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>


class Book{
    std::string author, title, genre, description, isbn;
    int year;
    std::vector<std::string> tags;
    double rating;

    public:
    Book(std::string a, std::string t, std::string g, std::string desc, int y, std::vector<std::string> tg,
        double r, std::string id)
        : author(a), title(t), genre(g), description(desc), year(y), tags(tg), rating(r), isbn(id){}


    std::string getTitle()const{return title;}
    std::string getAuthor()const{return author;}
    std::string getGenre()const{return genre;}
    std::string getISBN()const{return isbn;}
    int getYear()const{return year;}
    double getRating()const{return rating;}
    std::vector<std::string> getTags()const{return tags;}

    void setRating(double r){
        if(r>=0 && r<=5) rating=r;
    }

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
        <<"\n ISBN: "<<isbn
        <<"\n Description: "<<description
        <<"\n Tags: ";
        for(const std::string& tag: tags){
            std::cout<<tag<<" ";
        }        
        std::cout<<std::endl;
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

    //do i need it?
    /*virtual void printInfo()const{
        std::cout<<"User: "<<username<<"[Role: "<<(isAdmin()? "Admin": "Client") <<"]"<<std::endl;
    }*/
   
};

class Client: public User{
    public:
    Client(std::string u, std::string p): User(u,p){}

    bool isAdmin()const override{
        return false;
    }
};

class Admin: public User{
    public:
    Admin(std::string u, std::string p): User(u,p){}
    
    bool isAdmin()const override{
        return true;
    }

    //ako nqmam nujda ot print info i ot tova nqmam
    /*void printInfo()const override{
        std::cout<<"===Administrator Account==="<<std::endl;
        User::printInfo();
    }*/
};

class Library{
    std::vector<Book> books;
    std::map<std::string, std::string> users;
    User* currentUser=nullptr;
    std::string currentFilePath="";
    bool isFileOpen=false;

    std::string toLower(std::string str){
        //what?
        //std::transform(str.begin(), str.end(), ::tolower);
        return str;
    }

    void cleanData(){
        books.clear();
        currentUser=nullptr;
        currentFilePath="";
        isFileOpen=false;
    }

    //void free(){};
    //bool checkAccess(bool AdminRequired){};

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