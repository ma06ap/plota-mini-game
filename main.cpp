#include <iostream>
// #include "Backend/Games/Board.h"
#include "Backend/Games/Connect-4/Connect-4.h"
#include "Backend/Games/Othello/Othello.h"
#include "Backend/Games/Checkers/Checkers.h"


int main() {
    // std::cout << "=== Othello Game Test ===" << std::endl;
    //
    //  Othello game;
    //
    //  std::cout << "Initial Board:" << std::endl;
    //  game.printBoard();
    //
    //  //Play a few moves
    //  for (int move = 0; move < 10; ++move) {
    //      std::cout << "Turn " << (move + 1) << " - Current Player: " << game.getCurrentPlayer() << std::endl;
    //
    //      // Get allowed moves
    //      std::vector<Location> allowedMoves = game.allowed();
    //
    //      std::cout << "Allowed moves (" << allowedMoves.size() << " total): ";
    //      for (const auto& loc : allowedMoves) {
    //          std::cout << "(" << loc.getI() << "," << loc.getJ() << ") ";
    //      }
    //      std::cout << std::endl;
    //
    //      if (allowedMoves.empty()) {
    //          std::cout << "No valid moves available for " << game.getCurrentPlayer() << std::endl;
    //          game.nextTurn();
    //
    //          // Check if other player has moves
    //          allowedMoves = game.allowed();
    //          if (allowedMoves.empty()) {
    //              std::cout << "Game Over! No moves for both players." << std::endl;
    //              break;
    //          }
    //          std::cout << "Switching to " << game.getCurrentPlayer() << std::endl;
    //          continue;
    //      }
    //
    //      // Make the first allowed move
    //      Location move_loc = allowedMoves[0];
    //      std::cout << "Playing move at (" << move_loc.getI() << "," << move_loc.getJ() << ")" << std::endl;
    //      game.addPiece(move_loc);
    //
    //      std::cout << "Board after move:" << std::endl;
    //      game.printBoard();
    //
    //      // Switch turn
    //      game.nextTurn();
    //      std::cout << "---" << std::endl;
    //  }
    //  Location l(5,3);
    //  game.addPiece(l); // Invalid move test
    //  game.printBoard();
    //  std::cout << "\n=== Game Test Complete ===" << std::endl;
    //
    //
    // // start checking checkers
    // std::cout << "\n\n=== Checkers Game Test ===" << std::endl;
    //
    // Checkers checkers;
    //
    // std::cout << "Initial Board:" << std::endl;
    // std::cout << "Legend: w=white piece, W=white king, b=black piece, B=black king" << std::endl;
    // checkers.printBoard();
    //
    // // Play several moves
    // for (int move = 0; move < 15; ++move) {
    //     std::cout << "\nTurn " << (move + 1) << " - Current Player: " << checkers.getCurrentPlayer() << std::endl;
    //
    //     // Get allowed moves
    //     auto allowedMoves = checkers.allowed();
    //
    //     std::cout << "Allowed moves (" << allowedMoves.size() << " total):" << std::endl;
    //     for (size_t i = 0; i < std::min(size_t(10), allowedMoves.size()); ++i) {
    //         const auto& move = allowedMoves[i];
    //         std::cout << "  From (" << move.first.getI() << "," << move.first.getJ()
    //                   << ") to (" << move.second.getI() << "," << move.second.getJ() << ")" << std::endl;
    //     }
    //     if (allowedMoves.size() > 10) {
    //         std::cout << "  ... and " << (allowedMoves.size() - 10) << " more moves" << std::endl;
    //     }
    //
    //     if (allowedMoves.empty()) {
    //         std::cout << "No valid moves available for " << checkers.getCurrentPlayer() << std::endl;
    //         std::cout << "Game Over! " << checkers.getCurrentPlayer() << " has no moves left." << std::endl;
    //         break;
    //     }
    //
    //     // Make the first allowed move
    //     auto selectedMove = allowedMoves[0];
    //     Location from = selectedMove.first;
    //     Location to = selectedMove.second;
    //
    //     std::cout << "Playing move: (" << from.getI() << "," << from.getJ()
    //               << ") -> (" << to.getI() << "," << to.getJ() << ")" << std::endl;
    //
    //     checkers.movePiece(from, to);
    //
    //     std::cout << "Board after move:" << std::endl;
    //     checkers.printBoard();
    //
    //     // Check if we need to continue jumping or switch turn
    //     checkers.nextTurn();
    //     std::cout << "---" << std::endl;
    // }
    //
    // std::cout << "\n=== Checkers Test Complete ===" << std::endl;
    //
    // ConnectFour f;
    // f.allowColumns();
    // f.addColumn(4);
    // f.printBoard();
    // f.nextTurn();
    // f.addColumn(4);
    // f.printBoard();

    Othello h;
    std::string prompt;
    h.printBoard();
    while (true) {
        std::getline(std::cin, prompt);
        std::string result = h.input(prompt);
        h.printBoard();
        std::cout << result << std::endl;
    }

    return 0;
}
