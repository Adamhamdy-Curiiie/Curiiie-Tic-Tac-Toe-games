#include "Memory_Classes.h"
#include <cstdlib>
#include <ctime>

/////////////////////////////////////////////////////////////
//                     Memory_Board
/////////////////////////////////////////////////////////////

/**
 * @brief Construct a 3x3 Memory Tic-Tac-Toe board.
 *
 * Initializes all cells to the blank symbol.
 */
Memory_Board::Memory_Board() : Board(3, 3) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

/**
 * @brief Update the board with a move.
 *
 * Places a symbol on the board, or removes it if mark == 0.
 *
 * @param move Pointer to a Move object containing x, y, and symbol.
 * @return true if the update was valid and applied.
 * @return false if the move is invalid.
 */
bool Memory_Board::update_board(Move<char>* move) {
    int x = move->get_x(), y = move->get_y();
    char mark = move->get_symbol();

    if (x >= 0 && x < rows && y >= 0 && y < columns &&
        (board[x][y] == blank_symbol || mark == 0)) {

        if (mark == 0) {
            n_moves--;
            board[x][y] = blank_symbol;
        }
        else {
            n_moves++;
            board[x][y] = toupper(mark);
        }
        return true;
    }
    return false;
}

/**
 * @brief Check if a player has won.
 *
 * Verifies all rows, columns, and diagonals for three matching symbols.
 *
 * @param player Pointer to the player being checked.
 * @return true if the player has a winning pattern.
 * @return false otherwise.
 */
bool Memory_Board::is_win(Player<char>* player) {
    char sym = player->get_symbol();
    auto all_equal = [&](char a, char b, char c) {
        return a == b && b == c && a != blank_symbol;
        };

    // Rows & columns
    for (int i = 0; i < rows; i++)
        if ((all_equal(board[i][0], board[i][1], board[i][2]) && board[i][0] == sym) ||
            (all_equal(board[0][i], board[1][i], board[2][i]) && board[0][i] == sym))
            return true;

    // Diagonals
    if ((all_equal(board[0][0], board[1][1], board[2][2]) && board[1][1] == sym) ||
        (all_equal(board[0][2], board[1][1], board[2][0]) && board[1][1] == sym))
        return true;

    return false;
}

/**
 * @brief Check if the game is a draw.
 *
 * @param player The last played player (needed to ensure no win).
 * @return true if board full and no win.
 */
bool Memory_Board::is_draw(Player<char>* player) {
    return n_moves == 9 && !is_win(player);
}

/**
 * @brief Check if the game has ended (win or draw).
 *
 * @param player Pointer to the player to check.
 * @return true if game ended.
 */
bool Memory_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

/**
 * @brief Get hidden version of the board.
 *
 * - Before match completion: returns all cells as '.'
 * - After match completion: returns '#' for revealed cells.
 *
 * @param round_completed Whether the player successfully matched two cells.
 * @return A 2D vector representing masked board.
 */
vector<vector<char>> Memory_Board::get_hidden_board(bool round_completed) const {
    vector<vector<char>> hidden(rows, vector<char>(columns, '.'));

    if (round_completed) {
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < columns; j++)
                if (board[i][j] != blank_symbol)
                    hidden[i][j] = '#';
    }

    return hidden;
}

/////////////////////////////////////////////////////////////
//                     Memory_UI
/////////////////////////////////////////////////////////////

/**
 * @brief Construct the Memory UI.
 *
 * Displays welcome message and sets board size to 3.
 */
Memory_UI::Memory_UI()
    : UI<char>("Welcome to Memory Tic-Tac-Toe ", 3) {}

/**
 * @brief Create a new player.
 *
 * @param name The player's name.
 * @param symbol The character symbol (X or O).
 * @param type Indicates human or computer.
 * @return Pointer to a newly created Player object.
 */
Player<char>* Memory_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating "
        << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

/**
 * @brief Get a move from the player.
 *
 * Human: prompts user for x,y input
 * Computer: generates random x,y
 *
 * @param player Pointer to the player making the move.
 * @return Pointer to dynamically allocated Move object.
 */
Move<char>* Memory_UI::get_move(Player<char>* player) {
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\n" << player->get_name()
            << ", enter your move x y (0-2): ";
        cin >> x >> y;
    }
    else {
        x = rand() % 3;
        y = rand() % 3;
    }

    return new Move<char>(x, y, player->get_symbol());
}
