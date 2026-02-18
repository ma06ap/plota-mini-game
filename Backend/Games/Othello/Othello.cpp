#include "Othello.h"
#include <iostream>

Othello::Othello() : Game(), turn("Black", "White") {
    src = Board(8, 8);
    // چیدمان اولیه استاندارد اتلو
    // 3,3 و 4,4 سفید --- 3,4 و 4,3 سیاه
    src.Add(new Piece("White"), 3, 3);
    src.Add(new Piece("Black"), 3, 4);
    src.Add(new Piece("Black"), 4, 3);
    src.Add(new Piece("White"), 4, 4);
}

std::string Othello::getName() const { return "Othello"; }
std::string Othello::getCurrentPlayer() const { return turn.get(); }
void Othello::printBoard() const {}

// بررسی جهت خاص برای اعتبار سنجی حرکت
bool Othello::checkDirection(Location &l, int dr, int dc) const {
    int r = l.getI() + dr;
    int c = l.getJ() + dc;
    std::string myColor = turn.get();
    std::string oppColor = (myColor == "Black") ? "White" : "Black";

    // 1. قدم اول: باید حتماً مهره حریف باشد
    if (r < 0 || r >= 8 || c < 0 || c >= 8) return false;
    Piece* p = src.getPiece(r, c);

    // اگر خالی است یا مهره خودمان است -> حرکت نامعتبر (چون ساندویچی شکل نمی‌گیرد)
    if (!p || p->getColor() != oppColor) return false;

    // 2. قدم‌های بعدی: باید به مهره خودمان برسیم
    while (true) {
        r += dr;
        c += dc;

        // اگر به بیرون رسیدیم
        if (r < 0 || r >= 8 || c < 0 || c >= 8) return false;

        p = src.getPiece(r, c);

        // اگر به جای خالی رسیدیم -> نامعتبر
        if (!p) return false;

        // اگر به مهره خودمان رسیدیم -> معتبر (محاصره کامل شد)
        if (p->getColor() == myColor) return true;

        // اگر هنوز مهره حریف است -> ادامه بده
    }
}

bool Othello::checkArround(Location &l) const {
    // بررسی تمام 8 جهت
    int dirs[8][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}, {-1,-1}, {-1,1}, {1,-1}, {1,1}};
    for(auto d : dirs) {
        if (checkDirection(l, d[0], d[1])) return true;
    }
    return false;
}

void Othello::flipDirection(int r, int c, int dr, int dc) {
    Location l(r, c);
    // فقط در صورتی تغییر رنگ می‌دهیم که آن جهت معتبر باشد
    if (!checkDirection(l, dr, dc)) return;

    int currR = r + dr;
    int currC = c + dc;
    std::string myColor = turn.get();

    while (true) {
        Piece* p = src.getPiece(currR, currC);
        // اگر به مهره خودمان رسیدیم، پایان عملیات
        if (!p || p->getColor() == myColor) break;

        // تغییر رنگ مهره حریف
        src.Delete(currR, currC);
        src.Add(new Piece(myColor), currR, currC);

        currR += dr;
        currC += dc;
    }
}

void Othello::addPiece(Location &l) {
    // گذاشتن مهره جدید
    src.Add(new Piece(turn.get()), l.getI(), l.getJ());

    // اعمال تغییر رنگ در تمام جهات معتبر
    int dirs[8][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}, {-1,-1}, {-1,1}, {1,-1}, {1,1}};
    for(auto d : dirs) {
        flipDirection(l.getI(), l.getJ(), d[0], d[1]);
    }
}

void Othello::nextTurn() {
    turn.switchTurn();
}

std::string Othello::getWinner() const {
    int black = 0, white = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Piece* p = src.getPiece(i, j);
            if (p) {
                if (p->getColor() == "Black") black++;
                else white++;
            }
        }
    }
    if (black > white) return "Black Wins (" + std::to_string(black) + "-" + std::to_string(white) + ")";
    if (white > black) return "White Wins (" + std::to_string(white) + "-" + std::to_string(black) + ")";
    return "Draw (" + std::to_string(black) + "-" + std::to_string(white) + ")";
}

std::vector<Location> Othello::allowed() {
    std::vector<Location> moves;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            // فقط خانه‌های خالی را چک کن
            if (src.getPiece(i, j) == nullptr) {
                Location l(i, j);
                if (checkArround(l)) {
                    moves.push_back(l);
                }
            }
        }
    }
    return moves;
}

std::string Othello::input(std::string prompt) {
    if (prompt == "start") {
        auto moves = allowed();
        std::string result = turn.get() + " ";
        for (const auto& m : moves) result += std::to_string(m.getI()) + " " + std::to_string(m.getJ()) + " ";
        return result;
    }

    if (prompt == "getboard") {
        std::string result;
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                Piece* p = src.getPiece(i, j);
                if (p) result += std::to_string(i) + " " + std::to_string(j) + " " + p->getColor() + " ";
            }
        }
        return result;
    }

    // ورودی: select r c
    if (prompt.size() < 10) return "Invalid Command";
    char ii = prompt[7];
    char jj = prompt[9];
    int i = ii - '0';
    int j = jj - '0';
    Location l(i, j);

    // 1. بررسی اعتبار حرکت
    bool isValid = false;
    auto validMoves = allowed();
    for(const auto& vm : validMoves) {
        if(vm.getI() == i && vm.getJ() == j) { isValid = true; break; }
    }
    if (!isValid) return "Invalid Move";

    // 2. انجام حرکت
    addPiece(l);
    nextTurn();

    // 3. بررسی ادامه بازی
    auto nextMoves = allowed();
    if (nextMoves.empty()) {
        // نوبت حریف رد می‌شود (Pass)
        nextTurn();
        nextMoves = allowed();
        if (nextMoves.empty()) {
            // هر دو حرکت ندارند -> پایان بازی
            return getWinner();
        }
    }

    std::string result = turn.get() + " ";
    for (const auto& m : nextMoves) result += std::to_string(m.getI()) + " " + std::to_string(m.getJ()) + " ";
    return result;
}
