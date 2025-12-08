#include "Pyramid_Classes.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

//-------------------------------- Pyramid_Board --------------------------------

/**
 * @brief Construct a Pyramid board with 3 rows and 5 columns.
 * Initializes the board and the winning lines.
 */
Pyramid_Board::Pyramid_Board() : Board<char>(3, 5) {
    init_board();
    init_winning_lines();
}

/**
 * @brief Initialize all board cells with the blank symbol and mark invalid cells.
 */
void Pyramid_Board::init_board() {
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < columns; c++)
            board[r][c] = blank_symbol;

    // Mark invalid cells to create the pyramid shape
    board[0][0] = invalid_blank;
    board[0][1] = invalid_blank;
    board[0][3] = invalid_blank;
    board[0][4] = invalid_blank;
    board[1][0] = invalid_blank;
    board[1][4] = invalid_blank;
}

/**
 * @brief Determine if a cell is part of the playable pyramid area.
 */
bool Pyramid_Board::is_playable_cell(int x, int y) const {
    int playable = 1 + 2 * x;       ///< Number of playable cells in the row.
    int center = columns / 2;       ///< Middle column index.
    int start = center - (playable / 2);
    int end = center + (playable / 2);
    return y >= start && y <= end;
}

/**
 * @brief Apply a move to the board if the cell is playable and empty.
 */
bool Pyramid_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();

    if (!is_playable_cell(x, y) || board[x][y] != blank_symbol)
        return false;

    board[x][y] = toupper(move->get_symbol());
    n_moves++;
    return true;
}

/**
 * @brief Initialize all possible winning lines for the pyramid board.
 */
void Pyramid_Board::init_winning_lines() {
    winning_lines = {
        { make_pair(1,1), make_pair(1,2), make_pair(1,3) },
        { make_pair(2,0), make_pair(2,1), make_pair(2,2) },
        { make_pair(2,1), make_pair(2,2), make_pair(2,3) },
        { make_pair(2,2), make_pair(2,3), make_pair(2,4) },
        { make_pair(0,2), make_pair(1,2), make_pair(2,2) },
        { make_pair(0,2), make_pair(1,1), make_pair(2,0) },
        { make_pair(0,2), make_pair(1,3), make_pair(2,4) }
    };
}

/**
 * @brief Check if the given player has a winning line.
 */
bool Pyramid_Board::is_win(Player<char>* player) {
    char sym = player->get_symbol();
    for (auto& line : winning_lines) {
        if (board[line[0].first][line[0].second] == sym &&
            board[line[1].first][line[1].second] == sym &&
            board[line[2].first][line[2].second] == sym)
            return true;
    }
    return false;
}

/**
 * @brief Check if the game ended in a draw.
 */
bool Pyramid_Board::is_draw(Player<char>* player) {
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < columns; c++)
            if (is_playable_cell(r, c) && board[r][c] == blank_symbol)
                return false;

    return !is_win(player);
}

/**
 * @brief Check if the game is over (win or draw).
 */
bool Pyramid_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

//-------------------------------- Pyramid_UI --------------------------------

/**
 * @brief Construct the Pyramid UI and print the welcome message.
 */
Pyramid_UI::Pyramid_UI()
    : UI<char>("Welcome to Pyramid Tic-Tac-Toe", 6)
{
    srand(static_cast<unsigned int>(time(nullptr)));
}

/**
 * @brief Create and return a player object.
 */
Player<char>* Pyramid_UI::create_player(string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

/**
 * @brief Get a move from a player (human or AI).
 */
Move<char>* Pyramid_UI::get_move(Player<char>* player) {
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        do {
            cout << "\nEnter row (0-2) and column (0-4): ";
            cin >> x >> y;
        } while (x < 0 || x > 2 || y < 0 || y > 4);
    }
    else {
        auto b = player->get_board_ptr()->get_board_matrix();
        vector<pair<int, int>> empty;

        for (int r = 0; r < 3; r++) {
            int playable = 1 + r * 2;
            int center = 2;
            int start = center - (playable / 2);
            int end = center + (playable / 2);

            for (int c = start; c <= end; c++)
                if (b[r][c] == '.')
                    empty.push_back({ r, c });
        }

        if (!empty.empty()) {
            int k = rand() % empty.size();
            x = empty[k].first;
            y = empty[k].second;
        }
        else {
            x = y = 0;
        }

        cout << "Computer played: (" << x << ", " << y << ")\n";
    }

    return new Move<char>(x, y, player->get_symbol());
}

/**
 * @brief Display the board in a pyramid shape.
 */
void Pyramid_UI::display_board_matrix(const vector<vector<char>>& matrix) const {
    cout << "\n";
    for (int r = 0; r < matrix.size(); r++) {
        int playable = 1 + 2 * r;
        int center = matrix[0].size() / 2;
        int spaces = center - (playable / 2);

        for (int i = 0; i < spaces; i++)
            cout << " ";

        int start = center - (playable / 2);
        int end = center + (playable / 2);

        for (int c = start; c <= end; c++)
            cout << matrix[r][c] << " ";

        cout << "\n";
    }
    cout << "\n";
}
