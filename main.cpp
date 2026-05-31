/**
 * @file main.cpp
 * @brief Стартира конзолната версия на библиотечната система.
 * 
 * Файлът съдържа главната функция main(), която настройва 
 * конзолата за UTF-8 и стартира работата на Library.
 */

#include <windows.h>
#include "Library.hpp"

/**
 * @brief Главна функция на конзолната версия на програмата.
 * 
 * Настройва конзолата да работи с UTF-8 кодировка, създава обект 
 * от тип Library и стартира командния цикъл чрез метода Library::run().
 * 
 * 
 * @return 0 при успешно приключване на програмата.
 */

int main(){
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

   Library lib;
   lib.run();

    return 0;

}