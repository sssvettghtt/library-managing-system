#include "raylib.h"
#include "Library.hpp"

#include <string>

// g++ main_with_raylib.cpp Library.cpp Book.cpp User.cpp -o app.exe -IC:\raylib\raylib\src -LC:\raylib\raylib\build\raylib -lraylib -lopengl32 -lgdi32 -lwinmm

Color MY_BG = {245, 241, 226, 255};
Color MY_DARK = {35, 32, 25, 255};

Color MY_BROWN = {141, 116, 88, 255};
Color MY_BEIGE = {195, 181, 142, 255};
Color MY_LIGHT = {235, 228, 205, 255};
Color MY_BOX = {248, 245, 232, 255};
Color MY_PINK = {166, 133, 121, 255};
Color MY_BLUEINK = {70, 65, 105, 255};

enum Screen
{
    HOME_SCREEN,
    LOGIN_SCREEN
};

enum ActiveInput
{
    NONE_INPUT,
    USERNAME_INPUT,
    PASSWORD_INPUT
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

void DrawTextBG(Font font, const char* text, float x, float y, float size, Color color)
{
    DrawTextEx(font, text, {x, y}, size, 1, color);
}

void DrawTextBG(Font font, const std::string& text, float x, float y, float size, Color color)
{
    DrawTextEx(font, text.c_str(), {x, y}, size, 1, color);
}

void DrawColorPoint(float x, float y, Color color)
{
    DrawCircle((int)x, (int)y, 7, color);
    DrawCircleLines((int)x, (int)y, 7, MY_DARK);
}

bool Button(Rectangle rect, const char* text, Font font, Color buttonColor)
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

void HandleTextInput(std::string& target)
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

void DrawInputBox(Rectangle rect,
                  const std::string& realText,
                  const char* label,
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

    DrawColorPoint(100, 390, areaLeftColor);
    DrawColorPoint(705, 75, areaTopRightColor);
    DrawColorPoint(780, 205, areaTopRightColor);
    DrawColorPoint(575, 625, areaBottomColor);
}

void DrawHomePage(Font font, const std::string& message)
{
    ClearBackground(MY_BG);

    DrawColoredBackgroundAreas();

    Rectangle bigBox = {70, 55, 540, 150};

    DrawRectangleRec(bigBox, MY_BOX);
    DrawRectangleLinesEx(bigBox, 3, MY_DARK);

    DrawTextBG(font, "Library System", 100, 75, 30, MY_DARK);
    DrawTextBG(font, "Book management project", 100, 120, 24, MY_BLUEINK);
    DrawTextBG(font, "Home page", 100, 155, 24, MY_BLUEINK);

    DrawColorPoint(405, 130, MY_BOX);

    Rectangle welcomeBox = {305, 250, 350, 70};

    DrawRectangleRec(welcomeBox, MY_BOX);
    DrawRectangleLinesEx(welcomeBox, 3, MY_DARK);

    DrawTextBG(font, "Welcome!", 335, 270, 25, MY_DARK);
    DrawColorPoint(400, 270, MY_BOX);

    DrawTextBG(font, message, 40, 675, 22, MY_DARK);
}

void DrawLoginPage(Font font,
                   const std::string& username,
                   const std::string& password,
                   const std::string& message,
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

    while (!WindowShouldClose())
    {
        BeginDrawing();

        if (currentScreen == HOME_SCREEN)
        {
            DrawHomePage(font, message);

            if (Button({350, 350, 220, 45}, "1. Open file", font, MY_BOX))
            {
                try
                {
                    lib.open("library.txt");
                    message = "File library.txt opened.";
                }
                catch (...)
                {
                    message = "Error while opening file.";
                }
            }

            if (Button({350, 410, 220, 45}, "2. Log in", font, MY_BOX))
            {
                currentScreen = LOGIN_SCREEN;
                activeInput = USERNAME_INPUT;
                message = "Enter username and password.";
            }

            if (Button({350, 470, 220, 45}, "3. Books all", font, MY_BOX))
            {
                try
                {
                    lib.booksAll();
                    message = "Books are printed in the terminal.";
                }
                catch (...)
                {
                    message = "Open file first.";
                }
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
                        username = "";
                        password = "";
                        activeInput = NONE_INPUT;
                        currentScreen = HOME_SCREEN;
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

        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();

    return 0;
}