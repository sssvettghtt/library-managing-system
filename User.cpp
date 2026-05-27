#include "User.hpp"

    User::User(const std::string &u, const std::string &p) : username(u), password(p) {}
     User::~User() {};

    std::string User::getUsername() const { return username; }
    std::string User::getPassword() const { return password; }
    bool User::checkPassword(const std::string &input) const
    {
        return password == input;
    }

    Client::Client(const std::string &u, const std::string &p) : User(u, p) {}

    bool Client::isAdmin() const
    {
        return false;
    }

    void Client::writeToFile(std::ostream &file) const 
    {
        file << "USER|"
             << getUsername() << "|"
             << getPassword() << "|0"
             << std::endl;
    }

    Admin::Admin(const std::string &u, const std::string &p) : User(u, p) {}

    bool Admin::isAdmin() const 
    {
        return true;
    }

    void Admin::writeToFile(std::ostream &file) const 
    {
        file << "ADMIN|"
             << getUsername() << "|"
             << getPassword() << "|1"
             << std::endl;
    }

