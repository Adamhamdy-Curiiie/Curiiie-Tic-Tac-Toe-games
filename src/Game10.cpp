#include "Game10.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

Game10_Board::Game10_Board() : Board(6, 6) {
    
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank;

    srand((unsigned int)time(0));
}

bool Game10_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char sym = move->get_symbol();

    if (x < 0 || x >= 6 || y < 0 || y >= 6) return false;
    if (board[x][y] != blank) return false;
    board[x][y] = sym;
    n_moves++;

    if (n_moves % 2 == 0) {
        cout << "\n=== End of Round " << (n_moves / 2) << " ===\n";
        cout << "Adding 2 random obstacles...\n";
        add_obstacles();
    }

    return true;
}

bool Game10_Board::is_win(Player<char>* player) {
    char sym = player->get_symbol();

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j <= 2; j++) {
            if (board[i][j] == sym && board[i][j + 1] == sym &&
                board[i][j + 2] == sym && board[i][j + 3] == sym) {
                return true;
            }
        }
    }

    for (int j = 0; j < 6; j++) {
        for (int i = 0; i <= 2; i++) {
            if (board[i][j] == sym && board[i + 1][j] == sym &&
                board[i + 2][j] == sym && board[i + 3][j] == sym) {
                return true;
            }
        }
    }

    for (int i = 0; i <= 2; i++) {
        for (int j = 0; j <= 2; j++) {
            if (board[i][j] == sym && board[i + 1][j + 1] == sym &&
                board[i + 2][j + 2] == sym && board[i + 3][j + 3] == sym) {
                return true;
            }
        }
    }

    for (int i = 0; i <= 2; i++) {
        for (int j = 3; j < 6; j++) {
            if (board[i][j] == sym && board[i + 1][j - 1] == sym &&
                board[i + 2][j - 2] == sym && board[i + 3][j - 3] == sym) {
                return true;
            }
        }
    }

    return false;
}

bool Game10_Board::is_draw(Player<char>* player) {
    
    for (auto& row : board) {
        for (auto& cell : row) {
            if (cell == blank) return false;
        }
    }
    return true;
}

bool Game10_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

void Game10_Board::add_obstacles() {
    int obstacles_added = 0;
    int max_attempts = 100;
    int attempts = 0;

    while (obstacles_added < 2 && attempts < max_attempts) {
        int x = rand() % 6;
        int y = rand() % 6;
        if (board[x][y] == blank) {
            board[x][y] = obstacle;
            obstacles_added++;
            cout << "Obstacle added at (" << x << ", " << y << ")\n";
        }
        attempts++;
    }

    if (obstacles_added < 2) {
        cout << "Warning: Could not add 2 obstacles (board may be nearly full)\n";
    }
}

int Game10_Board::count_empty_cells() {
    int count = 0;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            if (board[i][j] == blank) {
                count++;
            }
        }
    }
    return count;
}

vector<Move<char>*> Game10_Player::get_valid_moves() {
    vector<Move<char>*> valid_moves;
    auto board_matrix = boardPtr->get_board_matrix();

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            if (board_matrix[i][j] == '.') {
                valid_moves.push_back(new Move<char>(i, j, symbol));
            }
        }
    }

    return valid_moves;
}

Move<char>* Game10_Player::get_random_move() {
    vector<Move<char>*> moves = get_valid_moves();

    if (!moves.empty()) {
        int random_index = rand() % moves.size();
        Move<char>* chosen = moves[random_index];

        for (int i = 0; i < moves.size(); i++) {
            if (i != random_index) delete moves[i];
        }

        cout << name << " (" << symbol << ") makes random move at: ("
            << chosen->get_x() << ", " << chosen->get_y() << ")\n";
        return chosen;
    }

    return nullptr;
}

Player<char>* Game10_UI::create_player(string& name, char symbol, PlayerType type) {
    return new Game10_Player(name, symbol, type);
}

Move<char>* Game10_UI::get_move(Player<char>* player) {
    if (player->get_type() == PlayerType::COMPUTER) {
        Game10_Player* g10player = static_cast<Game10_Player*>(player);
        return g10player->get_random_move();
    }
    else {
        int x, y;
        cout << player->get_name() << " (" << player->get_symbol() << ")\n";
        cout << "Enter position (x y): ";
        cin >> x >> y;
        return new Move<char>(x, y, player->get_symbol());
    }
}