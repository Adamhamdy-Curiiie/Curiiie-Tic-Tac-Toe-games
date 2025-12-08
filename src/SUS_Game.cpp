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

    board[x][y] = letter;
    n_moves++;
    last_player = (n_moves % 2 == 1) ? 'X' : 'O';
    check_and_count_sus(last_player);

    return true;
}

int SUS_Board::count_total_sus() {
    int c = 0;

    for (int i = 0; i < 3; i++)
        if (board[i][0] == 'S' && board[i][1] == 'U' && board[i][2] == 'S')
            c++;

    for (int j = 0; j < 3; j++)
        if (board[0][j] == 'S' && board[1][j] == 'U' && board[2][j] == 'S')
            c++;

    if (board[0][0] == 'S' && board[1][1] == 'U' && board[2][2] == 'S')
        c++;

    if (board[0][2] == 'S' && board[1][1] == 'U' && board[2][0] == 'S')
        c++;

    return c;
}

void SUS_Board::check_and_count_sus(char player_symbol) {
    int total_now = count_total_sus();
    int gained = 0;

    if (player_symbol == 'X') {
        gained = abs(total_now - Total_SUS);
        if (gained > 0) {
            score_X += gained;
            cout << "Player X formed " << gained << " new S-U-S! Total: "
                << score_X << "\n";
        }
    }
    else { 
        gained = abs(total_now - Total_SUS);
        if (gained > 0) {
            score_O += gained;
            cout << "Player O formed " << gained << "\n";
        }
    }
    Total_SUS += abs(total_now - Total_SUS);
	cout << "Total S-U-S on board: " << Total_SUS << "\n";
}

bool SUS_Board::is_win(Player<char>* player) {
    if (n_moves < 9) return false;

    char sym = player->get_symbol();
    if ((sym == 'X' && score_X > score_O) || (sym == 'O' && score_O > score_X)) {
		cout << display_message();
    return true;
}

    return false;
}

bool SUS_Board::is_lose(Player<char>* player) {
    return false;
}

bool SUS_Board::is_draw(Player<char>* player) {
    if (n_moves >= 9 && score_X == score_O) {
        return true;
        cout << display_message();
    }
    return false;
}

bool SUS_Board::game_is_over(Player<char>* player) {
    return n_moves >= 9;
}

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
    chosen_letter = (rand() % 2 == 0) ? 'S' : 'U';

    vector<Move<char>*> moves = get_valid_moves();

    if (!moves.empty()) {
        int random_index = rand() % moves.size();
        Move<char>* chosen = moves[random_index];

        for (int i = 0; i < moves.size(); i++) {
            if (i != random_index) delete moves[i];
        }

        cout << name << " places '" << chosen_letter << "' at ("
            << chosen->get_x() << ", " << chosen->get_y() << ")\n";
        return chosen;
    }

    return nullptr;
}

Player<char>* SUS_UI::create_player(string& name, char symbol, PlayerType type) {
    return new SUS_Player(name, symbol, type);
}

Move<char>* SUS_UI::get_move(Player<char>* player) {
    SUS_Player* sus_player = static_cast<SUS_Player*>(player);

    if (player->get_type() == PlayerType::COMPUTER) {
        return sus_player->get_random_move();
    }
    else {
        int x, y;
        char letter;

        cout << player->get_name() << "'s turn:\n";
        cout << "Choose letter (S/U): ";
        cin >> letter;
        letter = toupper(letter);

        while (letter != 'S' && letter != 'U') {
            cout << "Invalid! Choose S or U: ";
            cin >> letter;
            letter = toupper(letter);
        }

        sus_player->set_chosen_letter(letter);

        cout << "Enter position (x y): ";
        cin >> x >> y;

        return new Move<char>(x, y, letter);
    }
}
string SUS_Board::display_message() {
    string msg = "Scores - Player X: " + to_string(score_X) +
                 ", Player O: " + to_string(score_O) + "\n";
    return msg;
}