/**
 * @file main_with_raylib.cpp
 * @brief Стартира  графичната версия на библиотечната система чрез Raylib.
 * 
 * Фаилът съдържа основния графичен интерфейс на проекта.
 * Чрез Raylib се визуализират различните екрани за вход, профил,
 * преглед на книги, търсене, сортиране, запазване и администраторски действия.
 * 
 * Графичен интерфейс използва обект от тип Library, който съдържа
 * основната логика на библиотечната система.
 */

#include "raylib.h"
#include "Library.hpp"

#include <string>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

// g++ main_with_raylib.cpp Library.cpp Book.cpp User.cpp -o app.exe -IC:\raylib\raylib\src -LC:\raylib\raylib\build\raylib -lraylib -lopengl32 -lgdi32 -lwinmm

Color MY_BG = {245, 241, 226, 255};     ///<Основен цвят на фона.
Color MY_DARK = {35, 32, 25, 255};      ///< Тъмен цвят за рамки  и текст. 
Color MY_BROWN = {141, 116, 88, 255};   ///< Кафяв цвят за бутони и фон. 
Color MY_BEIGE = {195, 181, 142, 255};  ///<Бежов цвят за бутони.
Color MY_LIGHT = {235, 228, 205, 255};  ///<Светъл цвят за декоративни области.
Color MY_BOX = {248, 245, 232, 255};    ///<Цвят за панели и полета.
Color MY_PINK = {166, 133, 121, 255};   ///<Допълнителен декоративен цвят.
Color MY_BLUEINK = {70, 65, 105, 255};  ///<Синкав цвят за вторичен текст.

/**
 * @enum Screen
 * @brief Описва различните екрани в графичния интерфейс.
 * 
 * Използва се а превключване между начална страница login страница,
 * профил, списък с книги, save as форма и администраторски форми.
 */
enum Screen
{
    HOME_SCREEN,
    LOGIN_SCREEN,
    PROFILE_SCREEN,
    BOOKS_SCREEN,
    SAVE_AS_SCREEN,
    ADD_USER_SCREEN,
    REMOVE_USER_SCREEN,
    ADD_BOOK_SCREEN,
    REMOVE_BOOK_SCREEN,
    BOOK_VIEW_SCREEN,
    BOOK_FIND_SCREEN,
    BOOK_SORT_SCREEN
};

/**
 * @enum ActiveInput
 * @brief Показва кое текстово поле е активно в login екрана.
 */
enum ActiveInput
{
    NONE_INPUT,         ///< Няма активно поле.
    USERNAME_INPUT,     ///<Активно е полето за потребителско име.
    PASSWORD_INPUT      ///<Активно е полето за парола.
};

Color MakeLighter(Color color, int amount)
{
    Color result;

    result.r = (unsigned char)((color.r + amount > 255) ? 255 : color.r + amount);
    result.g = (unsigned char)((color.g + amount > 255) ? 255 : color.g + amount);
    result.b = (unsigned char)((color.b + amount > 255) ? 255 : color.b + amount);
    result.a = 255;

    return result;
}

void DrawTextBG(Font font, const char *text, float x, float y, float size, Color color)
{
    DrawTextEx(font, text, {x, y}, size, 1, color);
}

void DrawTextBG(Font font, const std::string &text, float x, float y, float size, Color color)
{
    DrawTextEx(font, text.c_str(), {x, y}, size, 1, color);
}

/**
 * @brief Рисува бутон и проверява дали е натиснат.
 * 
 * Бутонът променя цвета си при посочване с мишката.
 * 
 * @param rect Позиция и размер на бутона.
 * @param text Текст, който се показва върху бътона.
 * @param font Шрифт за текста.
 * @param buttonColor основен цвят на бутона
 * @return true Ако бутонът е натиснат с левия бутон на мишката.
 * @return false Ако бутонът не е натиснат.
 */
bool Button(Rectangle rect, const char *text, Font font, Color buttonColor)
{
    Vector2 mouse = GetMousePosition();
    bool hover = CheckCollisionPointRec(mouse, rect);

    Color drawColor = hover ? MakeLighter(buttonColor, 20) : buttonColor;

    DrawRectangleRec(rect, drawColor);
    DrawRectangleLinesEx(rect, 2, MY_DARK);

    DrawCircle((int)(rect.x + 16), (int)(rect.y + rect.height / 2), 5, buttonColor);
    DrawCircleLines((int)(rect.x + 16), (int)(rect.y + rect.height / 2), 5, MY_DARK);

    DrawTextEx(font, text, {rect.x + 38, rect.y + 8}, 20, 1, MY_DARK);

    return hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

/**
 *  @brief Обработа въвеждане на текст от клавиатурата.
 * 
 * Добавя въведените символи към подадения низ и обработва Backspace
 * за изтриване на последния символ.
 * 
 * @param target Низът, в който се записва въведеният текст.
 */
void HandleTextInput(std::string &target)
{
    int key = GetCharPressed();

    while (key > 0)
    {
        if (key >= 32 && key <= 125)
        {
            target += (char)key;
        }

        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && !target.empty())
    {
        target.pop_back();
    }
}

/**
 * @brief Рисува текстово поле за въвеждане.
 * 
 * Ако полето е за парола, реалният текст се скрива и се показват звездички.
 * 
 * @param rect Позиция и размер на текстовото поле.
 * @param realText Реалният текст, записан в полето.
 * @param label Надпис над полето.
 * @param font Шрифт затекста.
 * @param active Показва дали полето е активно.
 * @param passwordField Показва дали полето е поле за парола.
 */
void DrawInputBox(Rectangle rect,
                  const std::string &realText,
                  const char *label,
                  Font font,
                  bool active,
                  bool passwordField)
{
    DrawTextEx(font, label, {rect.x, rect.y - 30}, 22, 1, MY_DARK);

    DrawRectangleRec(rect, MY_BOX);
    DrawRectangleLinesEx(rect, active ? 3 : 2, active ? MY_BROWN : MY_DARK);

    std::string textToShow;

    if (passwordField)
    {
        for (size_t i = 0; i < realText.size(); i++)
        {
            textToShow += "*";
        }
    }
    else
    {
        textToShow = realText;
    }

    DrawTextEx(font, textToShow.c_str(), {rect.x + 10, rect.y + 10}, 22, 1, MY_DARK);
}

void DrawColoredBackgroundAreas()
{
    Vector2 topLeft = {5, 5};
    Vector2 topRight = {895, 5};
    Vector2 bottomLeft = {5, 715};
    Vector2 bottomRight = {895, 715};
    Vector2 rightMiddle = {895, 500};

    Color areaLeftColor = MY_LIGHT;
    Color areaTopRightColor = MY_BROWN;
    Color areaBottomColor = MY_BEIGE;

    DrawTriangle(bottomLeft, topRight, topLeft, areaLeftColor);
    DrawTriangle(bottomLeft, rightMiddle, topRight, areaTopRightColor);
    DrawTriangle(bottomLeft, bottomRight, rightMiddle, areaBottomColor);

    DrawRectangleLinesEx({5, 5, 890, 710}, 3, MY_DARK);

    DrawLineV(bottomLeft, topRight, MY_DARK);
    DrawLineV(bottomLeft, rightMiddle, MY_DARK);
}

/**
 * @brief Рисува началната страница на графичния интерфейс.
 * 
 * Показва заглавие на проекта, приветствен текст и текущо съобщение.
 * 
 * @param font Шрифт за текста.
 * @param message Съобщение, което се показва в долната част на екрана.
 */
void DrawHomePage(Font font, const std::string &message)
{
    ClearBackground(MY_BG);

    DrawColoredBackgroundAreas();

    Rectangle bigBox = {70, 55, 540, 150};

    DrawRectangleRec(bigBox, MY_BOX);
    DrawRectangleLinesEx(bigBox, 3, MY_DARK);

    DrawTextBG(font, "Library System", 100, 75, 30, MY_DARK);
    DrawTextBG(font, "Book management project", 100, 120, 24, MY_BLUEINK);
    DrawTextBG(font, "Home page", 100, 155, 24, MY_BLUEINK);

    Rectangle welcomeBox = {305, 250, 350, 70};

    DrawRectangleRec(welcomeBox, MY_BOX);
    DrawRectangleLinesEx(welcomeBox, 3, MY_DARK);

    DrawTextBG(font, "Welcome!", 335, 270, 25, MY_DARK);

    DrawTextBG(font, message, 40, 675, 22, MY_DARK);
}

/**
 * @brief Рисува екрана за вход в системата.
 * 
 * Показва полета за потребителско име и парола.
 * 
 * @param font Шрифт за текста.
 * @param username Въведено потребителско име.
 * @param password Въведена парола.
 * @param message Съобщение към потребителя.
 * @param activeInput Активното поле за въвеждане.
 */
void DrawLoginPage(Font font,
                   const std::string &username,
                   const std::string &password,
                   const std::string &message,
                   ActiveInput activeInput)
{
    ClearBackground(MY_BG);

    DrawColoredBackgroundAreas();

    Rectangle loginPanel = {210, 105, 480, 430};

    DrawRectangleRec(loginPanel, MY_BOX);
    DrawRectangleLinesEx(loginPanel, 3, MY_DARK);

    DrawTextBG(font, "Log in", 390, 135, 34, MY_DARK);

    DrawInputBox({290, 245, 320, 45},
                 username,
                 "Username:",
                 font,
                 activeInput == USERNAME_INPUT,
                 false);

    DrawInputBox({290, 345, 320, 45},
                 password,
                 "Password:",
                 font,
                 activeInput == PASSWORD_INPUT,
                 true);

    DrawTextBG(font, message, 230, 575, 22, MY_DARK);
}

void DrawPopup(Font font, const std::string &text)
{
    Rectangle popup = {220, 270, 460, 150};

    DrawRectangleRec(popup, MY_BOX);
    DrawRectangleLinesEx(popup, 3, MY_DARK);

    DrawTextEx(font, text.c_str(), {popup.x + 35, popup.y + 45}, 22, 1, MY_DARK);
    DrawTextEx(font, "Click OK to continue", {popup.x + 120, popup.y + 80}, 18, 1, MY_BLUEINK);
}
/**
 * @brief Рисува профилната страница на влезналия потребител.
 * 
 * Показва приветсвие, примерна зона за профилна снимка
 * и съобщение за текущото състояние на програмата.
 * 
 * @param font Шрифт на текста.
 * @param username Име на текущо влезналия потребител.
 * @param message Съобщение към потребителя.
 */
void DrawProfilePage(Font font, const std::string &username, const std::string &message)
{
    ClearBackground(MY_BG);

    DrawColoredBackgroundAreas();

    Rectangle profilePanel = {120, 100, 660, 460};

    DrawRectangleRec(profilePanel, MY_BOX);
    DrawRectangleLinesEx(profilePanel, 3, MY_DARK);

    std::string welcomeText = "Welcome, " + username + "!";
    DrawTextBG(font, welcomeText, 260, 135, 34, MY_DARK);

    Rectangle profilePicture = {180, 230, 180, 210};

    DrawRectangleRec(profilePicture, MY_BEIGE);
    DrawRectangleLinesEx(profilePicture, 3, MY_DARK);

    DrawTextBG(font, "Profile", 230, 305, 24, MY_DARK);
    DrawTextBG(font, "picture", 225, 335, 24, MY_DARK);

    DrawTextBG(font, username, 205, 465, 22, MY_BLUEINK);

    DrawTextBG(font, message, 130, 610, 22, MY_DARK);
}

/**
 * @brief Връща текста от booksAll() като низ.
 * 
 * Методът временно пренасочва std::cout към string stream,
 * за да може резултатът от конзолната функция booksAll()
 * да бъде показан в Raylib прозореца.
 * 
 * @param lib Обектът Library, от който се взима информацията.
 * @return Текст с всички книги или съобщение за грешка.
 */
std::string GetBooksAllText(Library &lib)
{
    std::ostringstream buffer;

    std::streambuf *oldCout = std::cout.rdbuf(buffer.rdbuf());

    try
    {
        lib.booksAll();
    }
    catch (...)
    {
        std::cout.rdbuf(oldCout);
        return "Error: open file and log in first.";
    }

    std::cout.rdbuf(oldCout);

    return buffer.str();
}

void DrawBooksPage(Font font, const std::string &booksText, int scrollY)
{
    ClearBackground(MY_BG);

    DrawColoredBackgroundAreas();

    Rectangle panel = {60, 60, 780, 560};

    DrawRectangleRec(panel, MY_BOX);
    DrawRectangleLinesEx(panel, 3, MY_DARK);

    DrawTextBG(font, "All books", 90, 85, 32, MY_DARK);

    Rectangle textArea = {90, 140, 720, 430};

    DrawRectangleRec(textArea, {255, 252, 240, 255});
    DrawRectangleLinesEx(textArea, 2, MY_DARK);

    BeginScissorMode((int)textArea.x, (int)textArea.y, (int)textArea.width, (int)textArea.height);

    DrawTextEx(font, booksText.c_str(), {textArea.x + 10, textArea.y + 10 + scrollY}, 18, 1, MY_DARK);

    EndScissorMode();

    DrawTextBG(font, "Use mouse wheel to scroll", 90, 590, 18, MY_BLUEINK);
}

std::vector<std::string> SplitTags(const std::string &text)
{
    std::vector<std::string> result;
    std::stringstream ss(text);
    std::string tag;

    while (ss >> tag)
    {
        result.push_back(tag);
    }

    return result;
}

void DrawSimpleInputPage(Font font,
                         const std::string &title,
                         const std::string &message)
{
    ClearBackground(MY_BG);
    DrawColoredBackgroundAreas();

    Rectangle panel = {180, 80, 540, 540};

    DrawRectangleRec(panel, MY_BOX);
    DrawRectangleLinesEx(panel, 3, MY_DARK);

    DrawTextBG(font, title, 260, 110, 32, MY_DARK);
    DrawTextBG(font, message, 210, 580, 20, MY_DARK);
}

std::string CaptureBooksAll(Library &lib)
{
    std::ostringstream buffer;
    std::streambuf *oldCout = std::cout.rdbuf(buffer.rdbuf());

    try
    {
        lib.booksAll();
    }
    catch (...)
    {
        std::cout.rdbuf(oldCout);
        return "Error: cannot show books.";
    }

    std::cout.rdbuf(oldCout);
    return buffer.str();
}

/**
 * @brief Връща резултата от booksView() като текст.
 * 
 * @param lib  Обектът Library.
 * @param isbn ISBN на книгата, която се търси.
 * @return Текст с информация за книгата или съобщение за грешка.
 */
std::string CaptureBooksView(Library &lib, const std::string &isbn)
{
    std::ostringstream buffer;
    std::streambuf *oldCout = std::cout.rdbuf(buffer.rdbuf());

    try
    {
        lib.booksView(isbn);
    }
    catch (...)
    {
        std::cout.rdbuf(oldCout);
        return "Error: open file and log in first.";
    }

    std::cout.rdbuf(oldCout);
    return buffer.str();
}

/**
 * @brief Връща резултата от booksFind() като текст.
 * 
 * @param lib  Обектът Library.
 * @param option Критерий за търсене.
 * @param keyword Търсена стойност.
 * @return Текст с намерените книги или съобщение за грешка.
 */
std::string CaptureBooksFind(Library &lib, const std::string &option, const std::string &keyword)
{
    std::ostringstream buffer;
    std::streambuf *oldCout = std::cout.rdbuf(buffer.rdbuf());

    try
    {
        lib.booksFind(option, keyword);
    }
    catch (...)
    {
        std::cout.rdbuf(oldCout);
        return "Error: open file and log in first.";
    }

    std::cout.rdbuf(oldCout);
    return buffer.str();
}

/**
 * @brief Сортира книгите и връща резултата като текст. 
 * 
 * След сортиране извиква booksAll(), за да се покажат книгите 
 * в новия ред.
 * 
 * @param lib  Обектът Library.
 * @param option Критерий за сортиране.
 * @param orer Ред на сортиране: asc или desc.
 * @return Текст със сортираните книги или съобщение за грешка.
 */
std::string CaptureBooksSort(Library &lib, const std::string &option, const std::string &order)
{
    std::ostringstream buffer;
    std::streambuf *oldCout = std::cout.rdbuf(buffer.rdbuf());

    try
    {
        lib.booksSort(option, order);
        std::cout << "\n\n";
        lib.booksAll();
    }
    catch (...)
    {
        std::cout.rdbuf(oldCout);
        return "Error: open file and log in first.";
    }

    std::cout.rdbuf(oldCout);
    return buffer.str();
}

/**
 * @brief Главна функция на Raylib версията на програмата.
 * 
 * Създава графичен прозорец, зарежда шрифт, и управлява основния 
 * цикъл на приложението. В зависимост от текущия Screen се рисува 
 * различна страница и се обработват действията на потребителя.
 * 
 * @return 0 при успешно приключване на програмата.
 */
int main()
{
    Library lib;

    InitWindow(900, 720, "Library System");
    SetTargetFPS(60);

    Font font = LoadFontEx("fonts/arial.ttf", 36, 0, 2048);

    Screen currentScreen = HOME_SCREEN;
    ActiveInput activeInput = NONE_INPUT;

    std::string message = "Home page";
    std::string username = "";
    std::string password = "";

    bool showPopup = false;
    std::string popupText = "";

    std::string loggedUsername = "";

    std::string booksText = "";
    int booksScrollY = 0;

    std::string viewISBN = "";

    std::string findOption = "";
    std::string findKeyword = "";

    std::string sortOption = "";
    std::string sortOrder = "";

    int activeBookField = 0;

    std::string saveAsPath = "";

    int activeAdminField = 0;

    std::string newUserName = "";
    std::string newUserPassword = "";
    std::string removeUserName = "";

    std::string bookTitle = "";
    std::string bookAuthor = "";
    std::string bookGenre = "";
    std::string bookYear = "";
    std::string bookRating = "";
    std::string bookISBN = "";
    std::string bookDescription = "";
    std::string bookTags = "";

    std::string removeBookISBN = "";

    while (!WindowShouldClose())
    {
        BeginDrawing();

        if (currentScreen == HOME_SCREEN)
        {
            DrawHomePage(font, message);

            // Бутон за отваряне на файл
            if (Button({350, 350, 220, 45}, "1. Open file", font, MY_BOX))
            {
                try
                {
                    std::string fileName = "library.txt";
                    lib.open(fileName);

                    popupText = "Successfully opened file " + fileName;
                    message = popupText;
                    showPopup = true;
                }
                catch (...)
                {
                    popupText = "Error while opening file.";
                    message = popupText;
                    showPopup = true;
                }
            }

            // close
            if (Button({350, 470, 220, 45}, "Close file", font, MY_BOX))
            {
                try
                {
                    // std::string fileName = "library.txt";
                    lib.close();

                    popupText = "Successfully closed file ";
                    message = popupText;
                    showPopup = true;
                }
                catch (...)
                {
                    popupText = "Error while opening file.";
                    message = popupText;
                    showPopup = true;
                }
            }
            ///

            // Бутон за вход
            if (Button({350, 410, 220, 45}, "2. Log in", font, MY_BOX))
            {
                currentScreen = LOGIN_SCREEN;
                activeInput = USERNAME_INPUT;
                message = "Enter username and password.";
            }

            if (Button({740, 630, 130, 35}, "Exit", font, MY_BROWN))
            {
                break;
            }

            if (Button({740, 675, 130, 35}, "Help", font, MY_BEIGE))
            {
                lib.help();
                message = "Help is printed in the terminal.";
            }

            if (showPopup)
            {
                DrawPopup(font, popupText);

                if (Button({385, 385, 130, 35}, "OK", font, MY_BEIGE))
                {
                    showPopup = false;
                }
            }
        }
        else if (currentScreen == LOGIN_SCREEN)
        {
            DrawLoginPage(font, username, password, message, activeInput);

            Vector2 mouse = GetMousePosition();

            Rectangle usernameBox = {290, 245, 320, 45};
            Rectangle passwordBox = {290, 345, 320, 45};

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (CheckCollisionPointRec(mouse, usernameBox))
                {
                    activeInput = USERNAME_INPUT;
                }
                else if (CheckCollisionPointRec(mouse, passwordBox))
                {
                    activeInput = PASSWORD_INPUT;
                }
                else
                {
                    activeInput = NONE_INPUT;
                }
            }

            if (activeInput == USERNAME_INPUT)
            {
                HandleTextInput(username);
            }
            else if (activeInput == PASSWORD_INPUT)
            {
                HandleTextInput(password);
            }

            if (IsKeyPressed(KEY_TAB))
            {
                if (activeInput == USERNAME_INPUT)
                {
                    activeInput = PASSWORD_INPUT;
                }
                else
                {
                    activeInput = USERNAME_INPUT;
                }
            }

            if (Button({290, 435, 140, 45}, "Login", font, MY_BEIGE) || IsKeyPressed(KEY_ENTER))
            {
                try
                {
                    bool success = lib.logInCredentials(username, password, message);

                    if (success)
                    {
                        loggedUsername = username;

                        username = "";
                        password = "";
                        activeInput = NONE_INPUT;

                        currentScreen = PROFILE_SCREEN;
                        message = "Login successful.";
                    }
                }
                catch (...)
                {
                    message = "Open file first.";
                }
            }

            if (Button({470, 435, 140, 45}, "Back", font, MY_BROWN))
            {
                username = "";
                password = "";
                activeInput = NONE_INPUT;
                currentScreen = HOME_SCREEN;
                message = "Back to home page.";
            }
        }
        else if (currentScreen == PROFILE_SCREEN)
        {
            DrawProfilePage(font, loggedUsername, message);

            if (Button({430, 190, 190, 32}, "Books all", font, MY_BEIGE))
            {
                booksText = GetBooksAllText(lib);
                booksScrollY = 0;
                currentScreen = BOOKS_SCREEN;
                message = "Books loaded.";
            }

            if (Button({430, 230, 190, 30}, "Save", font, MY_BEIGE))
            {
                try
                {
                    lib.save();
                    message = "File saved successfully.";
                }
                catch (...)
                {
                    message = "Open file first.";
                }
            }

            if (Button({430, 270, 190, 30}, "Save as", font, MY_BEIGE))
            {
                saveAsPath = "";
                activeInput = USERNAME_INPUT;
                currentScreen = SAVE_AS_SCREEN;
                message = "Enter new file name.";
            }

            if (Button({430, 310, 190, 32}, "Book view", font, MY_BEIGE))
            {
                viewISBN = "";
                activeBookField = 1;
                currentScreen = BOOK_VIEW_SCREEN;
                message = "Enter ISBN.";
            }

            if (Button({430, 350, 190, 32}, "Book find", font, MY_BEIGE))
            {
                findOption = "";
                findKeyword = "";
                activeBookField = 1;
                currentScreen = BOOK_FIND_SCREEN;
                message = "Option: title, author or tags.";
            }

            if (Button({430, 390, 190, 32}, "Book sort", font, MY_BEIGE))
            {
                sortOption = "";
                sortOrder = "";
                activeBookField = 1;
                currentScreen = BOOK_SORT_SCREEN;
                message = "Option: title, author, year or rating.";
            }

            if (Button({740, 575, 138, 35}, "Back home", font, MY_BROWN))
            {
                currentScreen = HOME_SCREEN;
                message = "Back to home page.";
            }

            if (Button({740, 605, 138, 35}, "Exit", font, MY_BROWN))
            {
                break;
            }
            if (Button({740, 635, 138, 35}, "Log out", font, MY_BROWN))
            {
                try
                {
                    lib.logOut();

                    loggedUsername = "";
                    username = "";
                    password = "";
                    activeInput = NONE_INPUT;

                    currentScreen = HOME_SCREEN;
                    message = "Logged out successfully.";
                }
                catch (...)
                {
                    message = "No user is logged in.";
                }
            }
            if (Button({740, 665, 138, 35}, "Help", font, MY_BEIGE))
            {
                lib.help();
                message = "Help is printed in the terminal.";
            }

            if (lib.isCurrentUserAdmin())
            {
                //DrawTextBG(font, "Admin options", 470, 455, 22, MY_DARK);

                if (Button({430, 385, 190, 32}, "Add user", font, MY_BEIGE))
                {
                    newUserName = "";
                    newUserPassword = "";
                    activeAdminField = 1;
                    currentScreen = ADD_USER_SCREEN;
                    message = "Enter new user data.";
                }

                if (Button({430, 425, 190, 32}, "Remove user", font, MY_BEIGE))
                {
                    removeUserName = "";
                    activeAdminField = 1;
                    currentScreen = REMOVE_USER_SCREEN;
                    message = "Enter username to remove.";
                }

                if (Button({430, 465, 190, 32}, "Book add", font, MY_BEIGE))
                {
                    bookTitle = "";
                    bookAuthor = "";
                    bookGenre = "";
                    bookYear = "";
                    bookRating = "";
                    bookISBN = "";
                    bookDescription = "";
                    bookTags = "";

                    activeAdminField = 1;
                    currentScreen = ADD_BOOK_SCREEN;
                    message = "Enter book data.";
                }

                if (Button({430, 465, 190, 32}, "Book remove", font, MY_BEIGE))
                {
                    removeBookISBN = "";
                    activeAdminField = 1;
                    currentScreen = REMOVE_BOOK_SCREEN;
                    message = "Enter ISBN to remove.";
                }
            }
        }
        else if (currentScreen == BOOKS_SCREEN)
        {
            booksScrollY += (int)(GetMouseWheelMove() * 25);

            if (booksScrollY > 0)
            {
                booksScrollY = 0;
            }

            DrawBooksPage(font, booksText, booksScrollY);

            if (Button({90, 635, 150, 40}, "Back", font, MY_BROWN))
            {
                currentScreen = PROFILE_SCREEN;
                message = "Back to profile.";
            }

            if (Button({680, 635, 150, 40}, "Exit", font, MY_BROWN))
            {
                break;
            }
        }
        else if (currentScreen == SAVE_AS_SCREEN)
        {
            ClearBackground(MY_BG);
            DrawColoredBackgroundAreas();

            Rectangle panel = {220, 160, 460, 300};

            DrawRectangleRec(panel, MY_BOX);
            DrawRectangleLinesEx(panel, 3, MY_DARK);

            DrawTextBG(font, "Save as", 370, 190, 34, MY_DARK);

            DrawInputBox({290, 300, 320, 45},
                         saveAsPath,
                         "File name:",
                         font,
                         true,
                         false);

            HandleTextInput(saveAsPath);

            DrawTextBG(font, message, 250, 500, 22, MY_DARK);

            if (Button({290, 390, 140, 45}, "Save", font, MY_BEIGE) || IsKeyPressed(KEY_ENTER))
            {
                try
                {
                    if (saveAsPath.empty())
                    {
                        message = "Enter file name first.";
                    }
                    else
                    {
                        lib.saveAs(saveAsPath);
                        message = "File saved as " + saveAsPath + ".";
                        currentScreen = PROFILE_SCREEN;
                    }
                }
                catch (...)
                {
                    message = "Open file first.";
                }
            }

            if (Button({470, 390, 140, 45}, "Back", font, MY_BROWN))
            {
                saveAsPath = "";
                currentScreen = PROFILE_SCREEN;
                message = "Back to profile.";
            }
        }
        else if (currentScreen == ADD_USER_SCREEN)
        {
            DrawSimpleInputPage(font, "Add user", message);

            Rectangle userBox = {290, 230, 320, 45};
            Rectangle passBox = {290, 330, 320, 45};

            DrawInputBox(userBox, newUserName, "Username:", font, activeAdminField == 1, false);
            DrawInputBox(passBox, newUserPassword, "Password:", font, activeAdminField == 2, false);

            Vector2 mouse = GetMousePosition();

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (CheckCollisionPointRec(mouse, userBox))
                    activeAdminField = 1;
                else if (CheckCollisionPointRec(mouse, passBox))
                    activeAdminField = 2;
            }

            if (activeAdminField == 1)
                HandleTextInput(newUserName);
            else if (activeAdminField == 2)
                HandleTextInput(newUserPassword);

            if (IsKeyPressed(KEY_TAB))
            {
                activeAdminField = activeAdminField == 1 ? 2 : 1;
            }

            if (Button({290, 430, 140, 45}, "Add", font, MY_BEIGE))
            {
                try
                {
                    lib.addUser(newUserName, newUserPassword);
                    message = "User added successfully.";
                    currentScreen = PROFILE_SCREEN;
                }
                catch (...)
                {
                    message = "Admin access required.";
                }
            }

            if (Button({470, 430, 140, 45}, "Back", font, MY_BROWN))
            {
                currentScreen = PROFILE_SCREEN;
                message = "Back to profile.";
            }
        }
        else if (currentScreen == REMOVE_USER_SCREEN)
        {
            DrawSimpleInputPage(font, "Remove user", message);

            Rectangle userBox = {290, 280, 320, 45};

            DrawInputBox(userBox, removeUserName, "Username:", font, true, false);
            HandleTextInput(removeUserName);

            if (Button({290, 390, 140, 45}, "Remove", font, MY_BEIGE))
            {
                try
                {
                    lib.removeUser(removeUserName);
                    message = "User removed successfully.";
                    currentScreen = PROFILE_SCREEN;
                }
                catch (...)
                {
                    message = "Admin access required.";
                }
            }

            if (Button({470, 390, 140, 45}, "Back", font, MY_BROWN))
            {
                currentScreen = PROFILE_SCREEN;
                message = "Back to profile.";
            }
        }
        else if (currentScreen == REMOVE_BOOK_SCREEN)
        {
            DrawSimpleInputPage(font, "Remove book", message);

            Rectangle isbnBox = {290, 280, 320, 45};

            DrawInputBox(isbnBox, removeBookISBN, "ISBN:", font, true, false);
            HandleTextInput(removeBookISBN);

            if (Button({290, 390, 140, 45}, "Remove", font, MY_BEIGE))
            {
                try
                {
                    lib.booksRemove(removeBookISBN);
                    message = "Book removed successfully.";
                    currentScreen = PROFILE_SCREEN;
                }
                catch (...)
                {
                    message = "Admin access required.";
                }
            }

            if (Button({470, 390, 140, 45}, "Back", font, MY_BROWN))
            {
                currentScreen = PROFILE_SCREEN;
                message = "Back to profile.";
            }
        }
        else if (currentScreen == ADD_BOOK_SCREEN)
        {
            ClearBackground(MY_BG);
            DrawColoredBackgroundAreas();

            Rectangle panel = {80, 40, 740, 620};

            DrawRectangleRec(panel, MY_BOX);
            DrawRectangleLinesEx(panel, 3, MY_DARK);

            DrawTextBG(font, "Add book", 360, 65, 32, MY_DARK);

            Rectangle titleBox = {140, 130, 260, 40};
            Rectangle authorBox = {500, 130, 260, 40};
            Rectangle genreBox = {140, 220, 260, 40};
            Rectangle yearBox = {500, 220, 260, 40};
            Rectangle ratingBox = {140, 310, 260, 40};
            Rectangle isbnBox = {500, 310, 260, 40};
            Rectangle descBox = {140, 400, 260, 40};
            Rectangle tagsBox = {500, 400, 260, 40};

            DrawInputBox(titleBox, bookTitle, "Title:", font, activeAdminField == 1, false);
            DrawInputBox(authorBox, bookAuthor, "Author:", font, activeAdminField == 2, false);
            DrawInputBox(genreBox, bookGenre, "Genre:", font, activeAdminField == 3, false);
            DrawInputBox(yearBox, bookYear, "Year:", font, activeAdminField == 4, false);
            DrawInputBox(ratingBox, bookRating, "Rating:", font, activeAdminField == 5, false);
            DrawInputBox(isbnBox, bookISBN, "ISBN:", font, activeAdminField == 6, false);
            DrawInputBox(descBox, bookDescription, "Description:", font, activeAdminField == 7, false);
            DrawInputBox(tagsBox, bookTags, "Tags:", font, activeAdminField == 8, false);

            Vector2 mouse = GetMousePosition();

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (CheckCollisionPointRec(mouse, titleBox))
                    activeAdminField = 1;
                else if (CheckCollisionPointRec(mouse, authorBox))
                    activeAdminField = 2;
                else if (CheckCollisionPointRec(mouse, genreBox))
                    activeAdminField = 3;
                else if (CheckCollisionPointRec(mouse, yearBox))
                    activeAdminField = 4;
                else if (CheckCollisionPointRec(mouse, ratingBox))
                    activeAdminField = 5;
                else if (CheckCollisionPointRec(mouse, isbnBox))
                    activeAdminField = 6;
                else if (CheckCollisionPointRec(mouse, descBox))
                    activeAdminField = 7;
                else if (CheckCollisionPointRec(mouse, tagsBox))
                    activeAdminField = 8;
            }

            if (activeAdminField == 1)
                HandleTextInput(bookTitle);
            else if (activeAdminField == 2)
                HandleTextInput(bookAuthor);
            else if (activeAdminField == 3)
                HandleTextInput(bookGenre);
            else if (activeAdminField == 4)
                HandleTextInput(bookYear);
            else if (activeAdminField == 5)
                HandleTextInput(bookRating);
            else if (activeAdminField == 6)
                HandleTextInput(bookISBN);
            else if (activeAdminField == 7)
                HandleTextInput(bookDescription);
            else if (activeAdminField == 8)
                HandleTextInput(bookTags);

            if (IsKeyPressed(KEY_TAB))
            {
                activeAdminField++;
                if (activeAdminField > 8)
                    activeAdminField = 1;
            }

            DrawTextBG(font, message, 120, 610, 20, MY_DARK);

            if (Button({260, 500, 150, 45}, "Add book", font, MY_BEIGE))
            {
                try
                {
                    int year = std::stoi(bookYear);
                    double rating = std::stod(bookRating);
                    std::vector<std::string> tags = SplitTags(bookTags);

                    bool added = lib.booksAddGui(bookTitle,
                                                 bookAuthor,
                                                 bookGenre,
                                                 bookDescription,
                                                 year,
                                                 tags,
                                                 rating,
                                                 bookISBN,
                                                 message);

                    if (added)
                    {
                        currentScreen = PROFILE_SCREEN;
                    }
                }
                catch (...)
                {
                    message = "Invalid book data.";
                }
            }

            if (Button({470, 500, 150, 45}, "Back", font, MY_BROWN))
            {
                currentScreen = PROFILE_SCREEN;
                message = "Back to profile.";
            }
        }
        else if (currentScreen == BOOK_VIEW_SCREEN)
        {
            ClearBackground(MY_BG);
            DrawColoredBackgroundAreas();

            Rectangle panel = {220, 160, 460, 300};
            DrawRectangleRec(panel, MY_BOX);
            DrawRectangleLinesEx(panel, 3, MY_DARK);

            DrawTextBG(font, "Book view", 350, 190, 32, MY_DARK);

            Rectangle isbnBox = {290, 300, 320, 45};
            DrawInputBox(isbnBox, viewISBN, "ISBN:", font, true, false);

            HandleTextInput(viewISBN);

            DrawTextBG(font, message, 230, 500, 20, MY_DARK);

            if (Button({290, 390, 140, 45}, "View", font, MY_BEIGE) || IsKeyPressed(KEY_ENTER))
            {
                booksText = CaptureBooksView(lib, viewISBN);
                booksScrollY = 0;
                currentScreen = BOOKS_SCREEN;
                message = "Book view result.";
            }

            if (Button({470, 390, 140, 45}, "Back", font, MY_BROWN))
            {
                currentScreen = PROFILE_SCREEN;
                message = "Back to profile.";
            }
        }
        else if (currentScreen == BOOK_FIND_SCREEN)
        {
            ClearBackground(MY_BG);
            DrawColoredBackgroundAreas();

            Rectangle panel = {190, 120, 520, 400};
            DrawRectangleRec(panel, MY_BOX);
            DrawRectangleLinesEx(panel, 3, MY_DARK);

            DrawTextBG(font, "Book find", 350, 150, 32, MY_DARK);

            Rectangle optionBox = {290, 250, 320, 45};
            Rectangle keywordBox = {290, 350, 320, 45};

            DrawInputBox(optionBox, findOption, "Option:", font, activeBookField == 1, false);
            DrawInputBox(keywordBox, findKeyword, "Keyword:", font, activeBookField == 2, false);

            Vector2 mouse = GetMousePosition();

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (CheckCollisionPointRec(mouse, optionBox))
                    activeBookField = 1;
                else if (CheckCollisionPointRec(mouse, keywordBox))
                    activeBookField = 2;
            }

            if (activeBookField == 1)
                HandleTextInput(findOption);
            else if (activeBookField == 2)
                HandleTextInput(findKeyword);

            if (IsKeyPressed(KEY_TAB))
            {
                activeBookField = activeBookField == 1 ? 2 : 1;
            }

            DrawTextBG(font, "Use: title, author or tags", 290, 410, 18, MY_BLUEINK);
            DrawTextBG(font, message, 210, 560, 20, MY_DARK);

            if (Button({430, 385, 190, 32}, "Find", font, MY_BEIGE) || IsKeyPressed(KEY_ENTER))
            {
                booksText = CaptureBooksFind(lib, findOption, findKeyword);
                booksScrollY = 0;
                currentScreen = BOOKS_SCREEN;
                message = "Book find result.";
            }

            if (Button({470, 455, 140, 45}, "Back", font, MY_BROWN))
            {
                currentScreen = PROFILE_SCREEN;
                message = "Back to profile.";
            }
        }
        EndDrawing();
    }
    UnloadFont(font);
    CloseWindow();

    return 0;
}