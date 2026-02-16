#include <iostream>

#include "Backend/Games/Connect-4/Connect-4.h"
#include "Backend/Games/Othello/Othello.h"
#include "Backend/Games/Checkers/Checkers.h"


int main() {
    ConnectFour f;
    Checkers checkers;
    std::string prompt;
    checkers.printBoard();
    while (true) {
        std::getline(std::cin, prompt);
        std::string result = checkers.input(prompt);
        checkers.printBoard();
        std::cout << result << std::endl;
        if (result.find("Win") != std::string::npos) {
            break;
        }
    }

    return 0;
}
