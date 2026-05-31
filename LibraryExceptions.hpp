#pragma once
#include <exception>
#include <string>

/**
 * @class LibraryExceptions
 * @brief Клас за изключения в библиотечната система.
 * 
 * Класът наследява std::exception и се използва за хвърляне на грешки, 
 * свързани с работата на библиотеката- например липса на отворен файл, 
 * липса на влязъл потребител или липса на администраторски права.
 */

class LibraryExceptions : public std::exception
{
    std::string msg; ///<Съобщение, описващо възникналата грешка.

public:

/**
 * @brief Създава изключение със зададено съобщение.
 * 
 * @param m Съобщение, което описва причината за грешката/
 */
    LibraryExceptions(const std::string &m) : msg(m) {}

/**
 * @brief Връща съобщението за грешка.
 * 
 * Методът override-ва std::exception::what().
 * 
 * @return C-style низ със съобщението за грешка.
 */
    const char *what() const noexcept override
    {
        return msg.c_str();
    }
};