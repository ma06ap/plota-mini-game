#include <iostream>
// #include "Backend/Games/Board.h"
#include "Backend/Games/Connect-4/Connect-4.h"
#include "Backend/Games/Othello/Othello.h"


int main() {
    // std::cout << "=== Othello Game Test ===" << std::endl;
    //
    // Othello game;
    //
    // std::cout << "Initial Board:" << std::endl;
    // game.printBoard();
    //
    // // Play a few moves
    // for (int move = 0; move < 10; ++move) {
    //     std::cout << "Turn " << (move + 1) << " - Current Player: " << game.getCurrentPlayer() << std::endl;
    //
    //     // Get allowed moves
    //     std::vector<Location> allowedMoves = game.allowed();
    //
    //     std::cout << "Allowed moves (" << allowedMoves.size() << " total): ";
    //     for (const auto& loc : allowedMoves) {
    //         std::cout << "(" << loc.getI() << "," << loc.getJ() << ") ";
    //     }
    //     std::cout << std::endl;
    //
    //     if (allowedMoves.empty()) {
    //         std::cout << "No valid moves available for " << game.getCurrentPlayer() << std::endl;
    //         game.nextTurn();
    //
    //         // Check if other player has moves
    //         allowedMoves = game.allowed();
    //         if (allowedMoves.empty()) {
    //             std::cout << "Game Over! No moves for both players." << std::endl;
    //             break;
    //         }
    //         std::cout << "Switching to " << game.getCurrentPlayer() << std::endl;
    //         continue;
    //     }
    //
    //     // Make the first allowed move
    //     Location move_loc = allowedMoves[0];
    //     std::cout << "Playing move at (" << move_loc.getI() << "," << move_loc.getJ() << ")" << std::endl;
    //     game.addPiece(move_loc);
    //
    //     std::cout << "Board after move:" << std::endl;
    //     game.printBoard();
    //
    //     // Switch turn
    //     game.nextTurn();
    //     std::cout << "---" << std::endl;
    // }
    // Location l(5,3);
    // game.addPiece(l); // Invalid move test
    // game.printBoard();
    // std::cout << "\n=== Game Test Complete ===" << std::endl;

    return 0;
}
