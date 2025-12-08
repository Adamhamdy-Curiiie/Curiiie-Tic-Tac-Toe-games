#include "Memory_Classes.h"
#include <cstdlib>
#include <ctime>

// ---------------- Memory_Board ----------------

Memory_Board::Memory_Board() : Board(3, 3) {
    for (auto& row : board)
        for (auto& cell : row) cell = blank_symbol;
}

bool Memory_Board::update_board(Move<char>* move) {
    int x = move->get_x(), y = move->get_y();
    char mark = move->get_symbol();

    if (x >= 0 && x < rows && y >= 0 && y < columns && board[x][y] == blank_symbol) {
        board[x][y] = toupper(mark);
        n_moves++;

        // Store the move in history
        move_history.push_back(make_tuple(x, y, toupper(mark)));

        return true;
    }
    return false;
}

void Memory_UI::display_board_matrix(const vector<vector<char> > &matrix) const {
    if (matrix.empty() || matrix[0].empty()) return;

    int rows = matrix.size();
    int cols = matrix[0].size();

    cout << "\n  ";
    for (int j = 0; j < cols; ++j)
        cout << setw(cell_width + 1) << j << "  ";
    cout << "\n  " << string((cell_width + 3) * cols, '-') << "-\n";

    for (int i = 0; i < rows; ++i)
    {
        cout << setw(2) << i << "|  ";
        for (int j = 0; j < cols; ++j)
        {
            cout << (matrix[i][j] == ' ' ? matrix[i][j] : substitute) << "  |  ";
        }
        cout << "\n  " << string((cell_width + 3) * cols, '-') << "-\n";
    }
    cout << endl;
}



bool Memory_Board::is_win(Player<char>* player) {
    char sym = player->get_symbol();
    auto all_equal = [&](char a, char b, char c) {
        return a == b && b == c && a != blank_symbol;
    };

    for (int i = 0; i < rows; i++)
        if ((all_equal(board[i][0], board[i][1], board[i][2]) && board[i][0] == sym) ||
            (all_equal(board[0][i], board[1][i], board[2][i]) && board[0][i] == sym))
            return true;

    if ((all_equal(board[0][0], board[1][1], board[2][2]) && board[1][1] == sym) ||
        (all_equal(board[0][2], board[1][1], board[2][0]) && board[1][1] == sym))
        return true;

    return false;
}

bool Memory_Board::is_draw(Player<char>* player) {
    return n_moves == 9 && !is_win(player);
}

bool Memory_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

// ---------------- Memory_UI ----------------

Memory_UI::Memory_UI() : UI<char>("Welcome to Memory Tic-Tac-Toe ", 3) {
    cout << "\n          MEMORY TIC-TAC-TOE GAME            \n";
    cout << "\nRules:\n";
    cout << "- All cells are hidden as '#' during the game\n";
    cout << "- You must remember where you and your opponent placed symbols\n";
    cout << "- At the end, all moves will be revealed\n";
    cout << "- Win by getting 3 in a row!\n\n";
}

Player<char>* Memory_UI::create_player(string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

Move<char>* Memory_UI::get_move(Player<char>* player) {
    int x, y;
    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\n" << player->get_name() << " (" << player->get_symbol()
             << "), enter your move x y (0-2): ";
        cin >> x >> y;
    }
    else {
        x = rand() % 3;
        y = rand() % 3;
    }
    return new Move<char>(x, y, player->get_symbol());
}
