#include "../include/InputHandler.h"
#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
int _getch() {
 struct termios oldt, newt;
 int ch;
 tcgetattr(STDIN_FILENO, &oldt);
 newt = oldt;
 newt.c_lflag &= ~(ICANON | ECHO);
 tcsetattr(STDIN_FILENO, TCSANOW, &newt);
 ch = getchar();
 tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
 return ch;
}
#endif


InputType InputHandler::handleInput() {
    while (true) {
        int input = _getch();

        // Filter out unwanted characters like new line or space
        if (input == '\n' || input == '\r' || input == ' ') {
            continue; // Skip unwanted characters
        }

        // Cast input to unsigned char and convert to uppercase
        input = static_cast<unsigned char>(toupper(input));

        switch (input) {
            case 'W':
                return InputType::UP;
            case 'S':
                return InputType::DOWN;
            case 'A':
                return InputType::LEFT;
            case 'D':
                return InputType::RIGHT;
            case 'E':
                return InputType::START_GAME;
            case 'Q':
                return InputType::QUIT_GAME;
            case 'U':
                return InputType::UNDO;
            default:
                return InputType::INVALID;
        }
    }
}
