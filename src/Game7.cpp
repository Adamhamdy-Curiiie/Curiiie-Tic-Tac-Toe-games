#include "Game7.h"
#include <iostream>
#include <cstdlib>
#include <cctype>
#include <algorithm>
using namespace std;

Game7_Board::Game7_Board() : Board(4, 4) {
    board[0][0] = 'O'; board[0][1] = 'X'; board[0][2] = 'O'; board[0][3] = 'X';
    board[3][0] = 'X'; board[3][1] = 'O'; board[3][2] = 'X'; board[3][3] = 'O';
    for (int i = 1; i < 3; i++)
        for (int j = 0; j < 4; j++)
            board[i][j] = blank;
}

bool Game7_Board::update_board(Move<char>* move) {
    Game7_Move* g7move = static_cast<Game7_Move*>(move);
    if (!g7move) return false;

    int from_x = g7move->get_from_x();
    int from_y = g7move->get_from_y();
    int to_x = move->get_x();
    int to_y = move->get_y();
    char sym = move->get_symbol();

    if (from_x < 0 || from_x >= 4 || from_y < 0 || from_y >= 4) return false;
    if (to_x < 0 || to_x >= 4 || to_y < 0 || to_y >= 4) return false;
    if (board[from_x][from_y] != sym) return false;
    if (board[to_x][to_y] != blank) return false;
    if (abs(from_x - to_x) + abs(from_y - to_y) != 1) return false;

    board[from_x][from_y] = blank;
    board[to_x][to_y] = sym;

    return true;
}

bool Game7_Board::check_win_for_symbol(char sym) {
    for (int i = 1; i <= 2; i++) {
        for (int j = 0; j < 2; j++)
            if (board[i][j] == sym && board[i][j + 1] == sym && board[i][j + 2] == sym)
                return true;
    }

    for (int j = 0; j < 4; j++)
        for (int i = 0; i < 2; i++)
            if (board[i][j] == sym && board[i + 1][j] == sym && board[i + 2][j] == sym)
                return true;

    if (board[0][0] == sym && board[1][1] == sym && board[2][2] == sym) return true;
    if (board[1][1] == sym && board[2][2] == sym && board[3][3] == sym) return true;
    if (board[0][1] == sym && board[1][2] == sym && board[2][3] == sym) return true;
    if (board[1][0] == sym && board[2][1] == sym && board[3][2] == sym) return true;
    if (board[0][3] == sym && board[1][2] == sym && board[2][1] == sym) return true;
    if (board[1][2] == sym && board[2][1] == sym && board[3][0] == sym) return true;
    if (board[0][2] == sym && board[1][1] == sym && board[2][0] == sym) return true;
    if (board[1][3] == sym && board[2][2] == sym && board[3][1] == sym) return true;

    return false;
}

bool Game7_Board::is_win(Player<char>* player) {
    return check_win_for_symbol(player->get_symbol());
}

bool Game7_Board::is_draw(Player<char>* player) {
    return false;
}

bool Game7_Board::game_is_over(Player<char>* player) {
    return is_win(player);
}

int Game7_Board::minimax(char current_player, char ai_symbol, int depth, int max_depth,
                         int alpha, int beta) {
    char opponent_symbol = (ai_symbol == 'X') ? 'O' : 'X';

    if (depth >= max_depth) return 0;
    if (check_win_for_symbol(ai_symbol)) return 10 - depth;
    if (check_win_for_symbol(opponent_symbol)) return -10 + depth;

    vector<Game7_Move*> valid_moves;
    int dx[] = { -1, 1, 0, 0 };
    int dy[] = { 0, 0, -1, 1 };

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (board[i][j] == current_player) {
                for (int k = 0; k < 4; k++) {
                    int new_i = i + dx[k];
                    int new_j = j + dy[k];
                    if (new_i >= 0 && new_i < 4 && new_j >= 0 && new_j < 4) {
                        if (board[new_i][new_j] == blank) {
                            valid_moves.push_back(new Game7_Move(i, j, new_i, new_j, current_player));
                        }
                    }
                }
            }
        }
    }

    if (valid_moves.empty()) {
        return 0;
    }

    bool is_maximizing = (current_player == ai_symbol);
    int best_score = is_maximizing ? -1000000 : 1000000;

    for (auto move : valid_moves) {
        int from_x = move->get_from_x();
        int from_y = move->get_from_y();
        int to_x = move->get_x();
        int to_y = move->get_y();

        board[from_x][from_y] = blank;
        board[to_x][to_y] = current_player;

        char next_player = is_maximizing ? opponent_symbol : ai_symbol;
        int score = minimax(next_player, ai_symbol, depth + 1, max_depth, alpha, beta);

        board[from_x][from_y] = current_player;
        board[to_x][to_y] = blank;

        if (is_maximizing) {
            best_score = max(best_score, score);
            alpha = max(alpha, score);
        } else {
            best_score = min(best_score, score);
            beta = min(beta, score);
        }

        if (beta <= alpha) break;
    }

    for (auto move : valid_moves) delete move;
    return best_score;
}

Game7_Move* Game7_Board::get_best_move(char ai_symbol, int difficulty) {
    char opponent_symbol = (ai_symbol == 'X') ? 'O' : 'X';
    int best_score = -1000000;
    Game7_Move* best_move = nullptr;
    int alpha = -1000000;
    int beta = 1000000;

    cout << "AI is thinking with (difficulty: " << difficulty << ")...\n";

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (board[i][j] == ai_symbol) {
                int dx[] = { -1, 1, 0, 0 };
                int dy[] = { 0, 0, -1, 1 };

                for (int k = 0; k < 4; k++) {
                    int new_i = i + dx[k];
                    int new_j = j + dy[k];

                    if (new_i >= 0 && new_i < 4 && new_j >= 0 && new_j < 4) {
                        if (board[new_i][new_j] == blank) {
                            board[i][j] = blank;
                            board[new_i][new_j] = ai_symbol;

                            int score = minimax(opponent_symbol, ai_symbol, 0, difficulty,
                                              alpha, beta);

                            board[i][j] = ai_symbol;
                            board[new_i][new_j] = blank;

                            if (score > best_score) {
                                best_score = score;
                                if (best_move) delete best_move;
                                best_move = new Game7_Move(i, j, new_i, new_j, ai_symbol);
                            }

                            alpha = max(alpha, best_score);
                        }
                    }
                }
            }
        }
    }

    if (best_move) {
        cout << "AI chooses (" << best_move->get_from_x() << ","
             << best_move->get_from_y() << ") -> ("
             << best_move->get_x() << "," << best_move->get_y()
             << ") with score: " << best_score << "\n";
    }

    return best_move;
}

Move<char>* Game7_Player::get_move() {
    Game7_Board* g7board = static_cast<Game7_Board*>(this->boardPtr);
    Game7_Move* move = g7board->get_best_move(symbol, 5);
    return move;
}

Player<char>* Game7_UI::create_player(string& name, char symbol, PlayerType type) {
    return new Game7_Player(name, symbol, type);
}

Move<char>* Game7_UI::get_move(Player<char>* player) {
    if (player->get_type() == PlayerType::COMPUTER) {
        Game7_Player* g7player = static_cast<Game7_Player*>(player);
        return g7player->get_move();
    }
    else {
        int from_x, from_y, to_x, to_y;

        cout << player->get_name() << " (" << player->get_symbol() << ")\n";
        cout << "Enter source position (x y): ";
        cin >> from_x >> from_y;
        cout << "Enter destination position (x y): ";
        cin >> to_x >> to_y;

        return new Game7_Move(from_x, from_y, to_x, to_y, player->get_symbol());
    }
}