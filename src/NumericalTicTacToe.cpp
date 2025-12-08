#include "NumericalTicTacToe.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;


NumericalBoard::NumericalBoard() : Board<int>(3, 3) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = blank;
        }
    }
    srand(static_cast<unsigned>(time(0)));
}

bool NumericalBoard::check_sum_15(int a, int b, int c) {
    return (a != 0 && b != 0 && c != 0 && a + b + c == 15);
}

bool NumericalBoard::update_board(Move<int>* move) {
    int x = move->get_x();
    int y = move->get_y();
    int num = move->get_symbol();

    if (x < 0 || x >= 3 || y < 0 || y >= 3) {
        return false;
    }

    if (board[x][y] != blank) {
        return false;
    }

    bool numIsOdd = (num % 2 == 1);
    if (currentMoveIsOdd && !numIsOdd) {
        return false;
    }
    if (!currentMoveIsOdd && numIsOdd) {
        return false;
    }

    if (currentMoveIsOdd) {
        if (used_numbers_odd.count(num)) return false;
        used_numbers_odd.insert(num);
    } else {
        if (used_numbers_even.count(num)) return false;
        used_numbers_even.insert(num);
    }

    board[x][y] = num;
    n_moves++;
    return true;
}

bool NumericalBoard::is_win(Player<int>* player) {

    for (int i = 0; i < 3; i++) {
        if (check_sum_15(board[i][0], board[i][1], board[i][2])) {
            return true;
        }
    }

    for (int j = 0; j < 3; j++) {
        if (check_sum_15(board[0][j], board[1][j], board[2][j])) {
            return true;
        }
    }

    if (check_sum_15(board[0][0], board[1][1], board[2][2])) {
        return true;
    }

    if (check_sum_15(board[0][2], board[1][1], board[2][0])) {
        return true;
    }

    return false;
}

bool NumericalBoard::is_lose(Player<int>* player) {
    return false;
}

bool NumericalBoard::is_draw(Player<int>* player) {
    if (n_moves >= 9) {
        return true;
    }
    return false;
}

bool NumericalBoard::game_is_over(Player<int>* player) {
    return n_moves >= 9 || is_win(player);
}

bool NumericalBoard::is_number_used(int num, bool isOdd) {
    if (isOdd) {
        return used_numbers_odd.count(num) > 0;
    } else {
        return used_numbers_even.count(num) > 0;
    }
}

NumericalPlayer::NumericalPlayer(string n, int s, PlayerType t, bool oddPlayer)
    : Player<int>(n, s, t), isOdd(oddPlayer), chosen_number(0)
{
    if (isOdd) {
        available_numbers_odd = {1, 3, 5, 7, 9};
        available_numbers_even = {};
    } else {
        available_numbers_odd = {};
        available_numbers_even = {2, 4, 6, 8};
    }
}

vector<int> NumericalPlayer::get_available_numbers() {
    NumericalBoard* numBoard = static_cast<NumericalBoard*>(this->boardPtr);
    vector<int> available;

    if (isOdd) {
        for (int num : available_numbers_odd) {
            if (!numBoard->is_number_used(num, true)) {
                available.push_back(num);
            }
        }
    } else {
        for (int num : available_numbers_even) {
            if (!numBoard->is_number_used(num, false)) {
                available.push_back(num);
            }
        }
    }

    return available;
}

vector<Move<int>*> NumericalPlayer::get_valid_moves() {
    vector<Move<int>*> valid_moves;
    auto board_matrix = this->boardPtr->get_board_matrix();

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board_matrix[i][j] == 0) {
                valid_moves.push_back(new Move<int>(i, j, chosen_number));
            }
        }
    }
    return valid_moves;
}

Move<int>* NumericalPlayer::get_random_move() {
    vector<int> available = get_available_numbers();

    if (available.empty()) {
        cout << name << " has no available numbers left!\n";
        return nullptr;
    }
    chosen_number = available[rand() % available.size()];

    vector<Move<int>*> moves = get_valid_moves();

    if (moves.empty()) {
        cout << name << " has no valid moves left!\n";
        return nullptr;
    }

    int random_index = rand() % moves.size();
    Move<int>* chosen = moves[random_index];

    // Clean up other moves
    for (int i = 0; i < moves.size(); i++) {
        if (i != random_index) delete moves[i];
    }

    cout << name << " places '" << chosen_number << "' at ("
         << chosen->get_x() << ", " << chosen->get_y() << ")\n";
    return chosen;
}

NumericalUI::NumericalUI() : UI<int>("Numerical Tic-Tac-Toe - Make lines sum to 15!", 3) {
    cout << "\nRules:\n";
    cout << "- Player 1 uses odd numbers (1,3,5,7,9)\n";
    cout << "- Player 2 uses even numbers (2,4,6,8)\n";
    cout << "- Players alternate placing one number in an empty cell\n";
    cout << "- Each number can only be used once\n";
    cout << "- Win by forming a line (row, column, or diagonal) that sums to 15\n";
    cout << "- If all cells are filled without a winner, it's a draw!\n\n";
}

Player<int>* NumericalUI::create_player(string& name, int symbol, PlayerType type) {
    bool playerIsOdd = (symbol == 'X');

    return new NumericalPlayer(name, symbol, type, playerIsOdd);
}

Move<int>* NumericalUI::get_move(Player<int>* player)
{
    NumericalPlayer* np = static_cast<NumericalPlayer*>(player);

    NumericalBoard* nb = static_cast<NumericalBoard*>(player->get_board_ptr());

    nb->set_current_player_odd(np->is_odd_player());

    if (player->get_type() == PlayerType::COMPUTER)
        return np->get_random_move();

    vector<int> available = np->get_available_numbers();

    if (available.empty()) {
        cout << player->get_name() << " has no available numbers left!\n";
        return nullptr;
    }

    cout << player->get_name() << " (";
    cout << (np->is_odd_player() ? "Odd" : "Even") << ") turn.\n";

    cout << "Available numbers: ";
    for (int num : available) cout << num << " ";
    cout << "\n";

    int number;
    cin >> number;

    while (find(available.begin(), available.end(), number) == available.end()) {
        cout << "Invalid. Choose again: ";
        cin >> number;
    }

    np->set_chosen_number(number);

    int x, y;
    cout << "Enter position (x y): ";
    cin >> x >> y;

    return new Move<int>(x, y, number);
}