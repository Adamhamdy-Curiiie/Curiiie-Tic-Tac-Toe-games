
#include "InfinityTicTacToe.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;


InfinityBoard::InfinityBoard() : Board<char>(3, 3), move_counter(0) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = blank;
        }
    }

    srand(static_cast<unsigned>(time(0)));
}

bool InfinityBoard::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char symbol = move->get_symbol();

    if (x < 0 || x >= 3 || y < 0 || y >= 3) return false;
    if (board[x][y] != blank) return false;

    board[x][y] = symbol;
    n_moves++;

    // Add move to queue
    move_history.push({x, y});
    move_counter++;

    if (move_counter == 3) {
        cout << "\n>>> REMOVING OLDEST MOVE <<<\n";
        remove_oldest_move();
        move_counter = 0;
    }

    return true;
}

void InfinityBoard::remove_oldest_move() {
    if (move_history.empty()) return;

    // Get the oldest move
    auto [x, y] = move_history.front();
    move_history.pop();

    // Remove it from the board
    board[x][y] = blank;
    n_moves--;

    cout << ">>> Move at position (" << x << ", " << y << ") has vanished! <<<\n\n";
}

bool InfinityBoard::check_win_for_symbol(char symbol) {
    // Check rows
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == symbol && board[i][1] == symbol && board[i][2] == symbol) {
            return true;
        }
    }

    // Check columns
    for (int j = 0; j < 3; j++) {
        if (board[0][j] == symbol && board[1][j] == symbol && board[2][j] == symbol) {
            return true;
        }
    }

    // Check diagonals
    if (board[0][0] == symbol && board[1][1] == symbol && board[2][2] == symbol) {
        return true;
    }
    if (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol) {
        return true;
    }

    return false;
}

bool InfinityBoard::is_win(Player<char>* player) {
    return check_win_for_symbol(player->get_symbol());
}

bool InfinityBoard::is_lose(Player<char>*) {
    return false;
}

bool InfinityBoard::is_draw(Player<char>* player) {
    return false;
}

bool InfinityBoard::game_is_over(Player<char>* player) {
    return is_win(player);
}

// ===== InfinityPlayer Implementation =====

InfinityPlayer::InfinityPlayer(string n, char s, PlayerType t)
    : Player<char>(n, s, t) {}

Move<char>* InfinityPlayer::get_random_move() {
    InfinityBoard* ib = static_cast<InfinityBoard*>(this->boardPtr);
    vector<pair<int,int>> valid_moves;
    auto board_matrix = ib->get_board_matrix();

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board_matrix[i][j] == '.') {
                valid_moves.push_back({i, j});
            }
        }
    }

    if (valid_moves.empty()) return nullptr;

    auto chosen = valid_moves[rand() % valid_moves.size()];
    cout << name << " places '" << symbol << "' at ("
         << chosen.first << ", " << chosen.second << ")\n";

    return new Move<char>(chosen.first, chosen.second, symbol);
}

// ===== InfinityUI Implementation =====

InfinityUI::InfinityUI() : UI<char>("Infinity Tic-Tac-Toe", 3) {
    cout << "\n===== Infinity Tic-Tac-Toe Game =====\n";
    cout << "Rules:\n";
    cout << "- Standard 3x3 Tic-Tac-Toe board\n";
    cout << "- After every 3 moves, the OLDEST move vanishes!\n";
    cout << "- Win by getting 3 in a row BEFORE those marks vanish\n";
    cout << "- The game continues infinitely until someone wins!\n\n";
}

Player<char>* InfinityUI::create_player(string& name, char symbol, PlayerType type) {
    return new InfinityPlayer(name, symbol, type);
}

Move<char>* InfinityUI::get_move(Player<char>* player) {
    if (player->get_type() == PlayerType::COMPUTER) {
        InfinityPlayer* ip = static_cast<InfinityPlayer*>(player);
        return ip->get_random_move();
    }

    int x, y;
    cout << player->get_name() << " (" << player->get_symbol() << "), enter position (row col): ";
    cin >> x >> y;
    return new Move<char>(x, y, player->get_symbol());
}