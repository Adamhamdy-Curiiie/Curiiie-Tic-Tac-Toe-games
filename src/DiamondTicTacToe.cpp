#include "DiamondTicTacToe.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

DiamondBoard::DiamondBoard() : Board<char>(7, 7) {
    initialize_diamond_shape();

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            board[i][j] = diamond_shape[i][j] ? blank : '#';
        }
    }
    srand(static_cast<unsigned>(time(0)));
}

void DiamondBoard::initialize_diamond_shape() {
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            diamond_shape[i][j] = false;
        }
    }

    int cols_per_row[7] = {1, 3, 5, 7, 5, 3, 1};

    for (int i = 0; i < 7; i++) {
        int start = (7 - cols_per_row[i]) / 2;
        for (int j = 0; j < cols_per_row[i]; j++) {
            diamond_shape[i][start + j] = true;
        }
    }
}

bool DiamondBoard::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();

    if (x < 0 || x >= 7 || y < 0 || y >= 7) return false;
    if (!diamond_shape[x][y]) return false;
    if (board[x][y] != blank) return false;

    board[x][y] = move->get_symbol();
    n_moves++;
    return true;
}

vector<vector<pair<int,int>>> DiamondBoard::find_lines_of_length(char symbol, int length) {
    vector<vector<pair<int,int>>> lines;
    int directions[4][2] = {{0,1}, {1,0}, {1,1}, {1,-1}};

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            for (int d = 0; d < 4; d++) {
                vector<pair<int,int>> line;
                bool valid = true;

                for (int k = 0; k < length; k++) {
                    int row = i + k * directions[d][0];
                    int col = j + k * directions[d][1];

                    if (row < 0 || row >= 7 || col < 0 || col >= 7 ||
                        !diamond_shape[row][col] || board[row][col] != symbol) {
                        valid = false;
                        break;
                    }
                    line.push_back({row, col});
                }

                if (valid) {
                    lines.push_back(line);
                }
            }
        }
    }

    return lines;
}

bool DiamondBoard::lines_different_directions(const vector<pair<int,int>>& line1,
                                              const vector<pair<int,int>>& line2) {
    if (line1.size() < 2 || line2.size() < 2) return false;

    int dx1 = line1[1].first - line1[0].first;
    int dy1 = line1[1].second - line1[0].second;
    int dx2 = line2[1].first - line2[0].first;
    int dy2 = line2[1].second - line2[0].second;

    return !(dx1 == dx2 && dy1 == dy2);
}

int DiamondBoard::count_shared_cells(const vector<pair<int,int>>& line1,
                                     const vector<pair<int,int>>& line2) {
    int count = 0;
    for (const auto& cell1 : line1) {
        for (const auto& cell2 : line2) {
            if (cell1 == cell2) {
                count++;
            }
        }
    }
    return count;
}

bool DiamondBoard::check_win_for_symbol(char symbol) {
    auto lines_3 = find_lines_of_length(symbol, 3);
    auto lines_4 = find_lines_of_length(symbol, 4);

    for (const auto& line3 : lines_3) {
        for (const auto& line4 : lines_4) {
            if (!lines_different_directions(line3, line4)) continue;

            int shared = count_shared_cells(line3, line4);
            if (shared <= 1) {
                return true;
            }
        }
    }

    return false;
}

bool DiamondBoard::is_win(Player<char>* player) {
    return check_win_for_symbol(player->get_symbol());
}

bool DiamondBoard::is_lose(Player<char> *) {
    return false;
}

bool DiamondBoard::is_draw(Player<char>* player) {
    return n_moves >= 37;
}

bool DiamondBoard::game_is_over(Player<char>* player) {
    return is_win(player) || n_moves >= 37;
}

int DiamondBoard::minimax(char current_player, char ai_symbol, int depth, int max_depth, int alpha, int beta) {
    char opponent_symbol = (ai_symbol == 'X') ? 'O' : 'X';

    if (depth >= max_depth) return 0;
    if (check_win_for_symbol(ai_symbol)) return 10 - depth;
    if (check_win_for_symbol(opponent_symbol)) return -10 + depth;
    if (n_moves >= 37) return 0;

    bool is_maximizing = (current_player == ai_symbol);
    int best_score = is_maximizing ? -1000000 : 1000000;

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            if (diamond_shape[i][j] && board[i][j] == blank) {
                board[i][j] = current_player;
                n_moves++;

                char next_player = is_maximizing ? opponent_symbol : ai_symbol;
                int score = minimax(next_player, ai_symbol, depth + 1, max_depth, alpha, beta);

                board[i][j] = blank;
                n_moves--;

                if (is_maximizing) {
                    best_score = max(best_score, score);
                    alpha = max(alpha, score);
                } else {
                    best_score = min(best_score, score);
                    beta = min(beta, score);
                }

                if (beta <= alpha) return best_score;
            }
        }
    }
    return best_score;
}

pair<int, int> DiamondBoard::get_best_move(char ai_symbol, int difficulty) {
    char opponent_symbol = (ai_symbol == 'X') ? 'O' : 'X';
    int best_score = -1000000;
    pair<int, int> best_move = {-1, -1};
    int alpha = -1000000;
    int beta = 1000000;

    cout << "AI is thinking with (difficulty: " << difficulty << ")...\n";

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            if (diamond_shape[i][j] && board[i][j] == blank) {
                board[i][j] = ai_symbol;
                n_moves++;

                int score = minimax(opponent_symbol, ai_symbol, 0, difficulty, alpha, beta);

                board[i][j] = blank;
                n_moves--;

                if (score > best_score) {
                    best_score = score;
                    best_move = {i, j};
                }

                alpha = max(alpha, best_score);
            }
        }
    }

    cout << "AI chooses (" << best_move.first << ", " << best_move.second
         << ") with score: " << best_score << "\n";

    return best_move;
}

DiamondPlayer::DiamondPlayer(string n, char s, PlayerType t)
    : Player<char>(n, s, t) {}

Move<char>* DiamondPlayer::get_move() {
    DiamondBoard* db = static_cast<DiamondBoard*>(this->boardPtr);
    auto [row, col] = db->get_best_move(symbol, 5);
    cout << name << " places '" << symbol << "' at (" << row << ", " << col << ")\n";
    return new Move<char>(row, col, symbol);
}

DiamondUI::DiamondUI() : UI<char>("Diamond Tic-Tac-Toe - Smart AI!", 7) {
    cout << "\n=== Diamond Tic-Tac-Toe with Minimax AI ===\n";
    cout << "Rules:\n";
    cout << "- Board is a diamond shape on a 7x7 grid\n";
    cout << "- Win by completing BOTH:\n";
    cout << "  * A line of 3 symbols\n";
    cout << "  * A line of 4 symbols\n";
    cout << "  * In different directions (horizontal/vertical/diagonal)\n";
    cout << "- Lines may share 0 or 1 cell (optional)\n";
    cout << "- Computer uses smart Minimax algorithm!\n\n";
}

Player<char>* DiamondUI::create_player(string& name, char symbol, PlayerType type) {
    return new DiamondPlayer(name, symbol, type);
}

Move<char>* DiamondUI::get_move(Player<char>* player) {
    DiamondPlayer* dp = static_cast<DiamondPlayer*>(player);

    if (player->get_type() == PlayerType::COMPUTER) {
        return dp->get_move();
    }

    int x, y;
    cout << player->get_name() << " (" << player->get_symbol()
         << "), enter position (row col): ";
    cin >> x >> y;
    return new Move<char>(x, y, player->get_symbol());
}