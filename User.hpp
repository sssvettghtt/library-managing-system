#pragma once
#include <iostream>
#include <string>

/**
 * @class User
 * @brief Абстрактен базов клас запотребител в библиотечна система.
 * 
 * Класът съхранява общите данни за всички потребители:
 * потребителско име и парола. От него наследяват Client и Admin.
 * 
 * Класът е абстрактен, защото има чисто виртуални методи:
 * WriteToFile() и isAdmin().
 */

class User
{
private:
    std::string username;       ///<Потребителско има на потребителя.
    std::string password;       ///<Парола на потребителя.

public:

/**
 * @brief Създава потребител с потребителско име и парола.
 * 
 * @param u Потребителско име.
 * @param p Парола
 */
    User(const std::string &u, const std::string &p);

/**
 * @brief Виртуален деструктор.
 * Нужен е, защото класът се използва полиморфно чрез указатели от тип User*.
 */
    virtual ~User();

/**
 * @brief Връща потребителско име.
 * 
 * @return Потребителското име на потребителя. 
 */
    std::string getUsername() const;

/**
 * @brief Връща парола на потребителя.
 * 
 * @return Паролата на потребителя. 
 */
    std::string getPassword() const;

 /**
 * @brief Проверява дали подадената паролата съвпада с паролата на потребителя. 
 * 
 * @param input Парола, въведена от потребителя.
 * @return true Ако паролата съвпада.
 * @return false Ако паролата не съвпада.
 */
    bool checkPassword(const std::string &input) const;

/**
 * @brief Записва потребителя във файл.
 * 
 * Методът е чисто виртуален, защото клиентите и администраторите
 * се записват по различен начин.
 * 
 * @param file Изходен поток към файла.
 */
    virtual void writeToFile(std::ostream &file) const=0;

/**
 * @brief Проверява дали потребителят е администратор. 
 * 
 * @return true Ако потребителят е администратор.
 * @return false Ако патребителят е клиент.
 */
    virtual bool isAdmin() const=0;
};

/**
 * @class Client
 * @brief Представлява обикновен потребител в библиотечната система.
 * 
 * Клиентът може да преглежда, търси и сортира книги, но няма
 * администраторски права за добавяне и премахване на потребители или книги.
 */
class Client : public User
{
public:

/**
 * @brief Създава клиент с потребителско име и парола.
 * 
 * @param u Потребителско име.
 * @param p Парола.
 */
    Client(const std::string &u, const std::string &p);

/**
 *  @brief Проверява дали потребителят е администратор.
 * 
 * @return Винаги false, защото Client няма администраторски права
 */
    bool isAdmin() const override;

/**
 * @brief Записва клиент във файл.
 * 
 * Клиентът се записва във формат:
 * USER| username| password|0
 * 
 * @param file Изходен поток към файла.
 */
    void writeToFile(std::ostream &file) const override;
};

/**
 * @class Admin
 * @brief Представлява администратор в библиотечната система.
 * 
 * Администраторът има права да добавя и премахва потребители, 
 * както и да добавя и премахва книги.
 */
class Admin : public User
{
public:
/**
 * @brief Създава администратор с потребителско име и парола.
 * 
 * @param u Потребителско име.
 * @param p Парола.
 */
    Admin(const std::string &u, const std::string &p);

/**
 *  @brief Проверява дали потребителят е администратор.
 * 
 * @return Винаги true, защото Admin има администраторски права
 */
    bool isAdmin() const override;

/**
 * @brief Записва администратор във файл.
 * 
 * Администраторът се записва във формат:
 * ADMIN| username| password|1
 * 
 * @param file Изходен поток към файла.
 */
    void writeToFile(std::ostream &file) const override;
};
