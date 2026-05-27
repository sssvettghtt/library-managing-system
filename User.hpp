#pragma once
#include <iostream>
#include <string>

class User
{
private:
    std::string username;
    std::string password;

public:
    User(const std::string &u, const std::string &p);
    virtual ~User();

    std::string getUsername() const;
    std::string getPassword() const;
    bool checkPassword(const std::string &input) const;

    virtual void writeToFile(std::ostream &file) const=0;
    virtual bool isAdmin() const=0;
};

class Client : public User
{
public:
    Client(const std::string &u, const std::string &p);

    bool isAdmin() const override;

    void writeToFile(std::ostream &file) const override;
};

class Admin : public User
{
public:
    Admin(const std::string &u, const std::string &p);

    bool isAdmin() const override;

    void writeToFile(std::ostream &file) const override;
};
