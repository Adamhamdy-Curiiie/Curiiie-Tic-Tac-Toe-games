/**
 * @file SUS_Game.cpp
 * @brief Implementation file for the SUS Game
 */

#include "SUS_Game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;


SUS_Board::SUS_Board() : Board(3, 3), last_player('X') {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = blank;
        }
    }
    srand(static_cast<unsigned>(time(0)));
}

bool SUS_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char letter = move->get_symbol();

    if (x < 0 || x >= 3 || y < 0 || y >= 3) return false;

    if (board[x][y] != blank) return false;

    if (letter != 'S' && letter != 'U') return false;

    // Place letter
    board[x][y] = letter;
    n_moves++;

    last_player = (n_moves % 2 == 1) ? 'X' : 'O';

    check_and_count_sus(last_player);

    return true;
}

int SUS_Board::count_total_sus() {
    int count = 0;

    // Check horizontal S-U-S
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == 'S' && board[i][1] == 'U' && board[i][2] == 'S') {
            count++;
        }
    }

    // Check vertical S-U-S
    for (int j = 0; j < 3; j++) {
        if (board[0][j] == 'S' && board[1][j] == 'U' && board[2][j] == 'S') {
            count++;
        }
    }

    // Check main diagonal S-U-S
    if (board[0][0] == 'S' && board[1][1] == 'U' && board[2][2] == 'S') {
        count++;
    }

    // Check anti-diagonal S-U-S
    if (board[0][2] == 'S' && board[1][1] == 'U' && board[2][0] == 'S') {
        count++;
    }

    return count;
}

void SUS_Board::check_and_count_sus(char player_symbol) {
    int total_now = count_total_sus();
    int gained = abs(total_now - Total_SUS);

    if (gained > 0) {
        if (player_symbol == 'X') {
            score_X += gained;
            cout << "Player X formed " << gained << " new S-U-S! Total score: " << score_X << "\n";
        } else {
            score_O += gained;
            cout << "Player O formed " << gained << " new S-U-S! Total score: " << score_O << "\n";
        }
        Total_SUS = total_now;
    }

    cout << "Total S-U-S sequences on board: " << Total_SUS << "\n";
}

bool SUS_Board::is_win(Player<char>* player) {
    if (n_moves < 9) return false;

    char sym = player->get_symbol();

    if (sym == 'X' && score_X > score_O) {
        cout << "\nFinal Scores:\n";
        cout << "Player X: " << score_X << "\n";
        cout << "Player O: " << score_O << "\n";
        return true;
    }

    if (sym == 'O' && score_O > score_X) {
        cout << "\nFinal Scores:\n";
        cout << "Player X: " << score_X << "\n";
        cout << "Player O: " << score_O << "\n";
        return true;
    }

    return false;
}

bool SUS_Board::is_lose(Player<char>* player) {
    return false;
}

bool SUS_Board::is_draw(Player<char>* player) {
    if (n_moves >= 9) {
        cout << "\nFinal Scores:\n";
        cout << "Player X: " << score_X << "\n";
        cout << "Player O: " << score_O << "\n";
        return true;
    }
    return false;
}

bool SUS_Board::game_is_over(Player<char>* player) {
    return false;
}


SUS_Player::SUS_Player(string n, char s, PlayerType t)
    : Player<char>(n, s, t), chosen_letter('S') {}

vector<Move<char>*> SUS_Player::get_valid_moves() {
    vector<Move<char>*> valid_moves;
    auto board_matrix = boardPtr->get_board_matrix();

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board_matrix[i][j] == '.') {
                valid_moves.push_back(new Move<char>(i, j, chosen_letter));
            }
        }
    }

    return valid_moves;
}

Move<char>* SUS_Player::get_random_move() {
    // Randomly choose S or U
    chosen_letter = (rand() % 2 == 0) ? 'S' : 'U';

    vector<Move<char>*> moves = get_valid_moves();

    if (!moves.empty()) {
        int random_index = rand() % moves.size();
        Move<char>* chosen = moves[random_index];

        // Clean up other moves
        for (int i = 0; i < moves.size(); i++) {
            if (i != random_index) delete moves[i];
        }

        cout << name << " places '" << chosen_letter << "' at ("
             << chosen->get_x() << ", " << chosen->get_y() << ")\n";
        return chosen;
    }

    return nullptr;
}


SUS_UI::SUS_UI() : UI<char>("SUS Game - Form S-U-S sequences to win!", 3) {
    cout << "\n             SUS GAME                        \n";
    cout << "\nRules:\n";
    cout << "- Players place either 'S' or 'U' on the board\n";
    cout << "- Each turn, choose which letter to place\n";
    cout << "- Score points by forming 'S-U-S' sequences\n";
    cout << "- Sequences can be horizontal, vertical, or diagonal\n";
    cout << "- Player with most sequences wins when board is full!\n\n";
}

Player<char>* SUS_UI::create_player(string& name, char symbol, PlayerType type) {
    return new SUS_Player(name, symbol, type);
}

Move<char>* SUS_UI::get_move(Player<char>* player) {
    SUS_Player* sus_player = static_cast<SUS_Player*>(player);

    if (player->get_type() == PlayerType::COMPUTER) {
        return sus_player->get_random_move();
    }

    // Human player
    int x, y;
    char letter;

    cout << "\n" << player->get_name() << " (" << player->get_symbol() << ") turn:\n";
    cout << "Choose letter (S/U): ";
    cin >> letter;
    letter = toupper(letter);

    while (letter != 'S' && letter != 'U') {
        cout << "Invalid! Choose S or U: ";
        cin >> letter;
        letter = toupper(letter);
    }

    sus_player->set_chosen_letter(letter);

    cout << "Enter position (row col): ";
    cin >> x >> y;

    return new Move<char>(x, y, letter);
}