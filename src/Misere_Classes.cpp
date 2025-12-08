#include "Misere_Classes.h"
#include <iostream>
#include <iomanip>
#include <cstdlib> // for rand()

using namespace std;

/**
 * @brief Helper function to check if three symbols are equal and not blank.
 *
 * @param a First symbol
 * @param b Second symbol
 * @param c Third symbol
 * @param blank Character representing blank
 * @return true if all three symbols are equal and not blank
 * @return false otherwise
 */
static inline bool three_eq(char a, char b, char c, char blank) {
    return (a == b && b == c && a != blank);
}

// ---------------- Misere_TTT_Board ----------------

/**
 * @brief Constructor for Misère Tic-Tac-Toe board.
 * Initializes a 3x3 board and sets all cells to blank.
 */
Misere_TTT_Board::Misere_TTT_Board() : Board<char>(3, 3) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

/**
 * @brief Apply or undo a move on the board.
 *
 * @param move Pointer to the Move object
 * @return true if move is successfully applied/undone
 * @return false if move is invalid
 */
bool Misere_TTT_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    if (x < 0 || x >= rows || y < 0 || y >= columns)
        return false;

    // allow undo with mark == 0
    if (mark == 0) {
        if (board[x][y] != blank_symbol) {
            board[x][y] = blank_symbol;
            if (n_moves > 0) n_moves--;
        }
        return true;
    }

    // normal apply
    mark = toupper(mark);
    if (board[x][y] != blank_symbol) return false;

    board[x][y] = mark;
    n_moves++;
    return true;
}

/**
 * @brief Check if the given player has lost (3-in-row).
 *
 * @param player Pointer to Player object
 * @return true if player has 3-in-row (loses)
 * @return false otherwise
 */
bool Misere_TTT_Board::is_lose(Player<char>* player) {
    if (!player) return false;
    char sym = toupper(player->get_symbol());

    // rows & cols
    for (int i = 0; i < rows; ++i) {
        if (three_eq(board[i][0], board[i][1], board[i][2], blank_symbol) && board[i][0] == sym)
            return true;
        if (three_eq(board[0][i], board[1][i], board[2][i], blank_symbol) && board[0][i] == sym)
            return true;
    }

    // diagonals
    if (three_eq(board[0][0], board[1][1], board[2][2], blank_symbol) && board[1][1] == sym)
        return true;
    if (three_eq(board[0][2], board[1][1], board[2][0], blank_symbol) && board[1][1] == sym)
        return true;

    return false;
}

/**
 * @brief Check if the given player has won (opponent has lost).
 *
 * @param player Pointer to Player object
 * @return true if opponent has 3-in-row (current player wins)
 * @return false otherwise
 */
bool Misere_TTT_Board::is_win(Player<char>* player) {
    if (!player) return false;
    char sym = toupper(player->get_symbol());
    char opp = (sym == 'X') ? 'O' : 'X';

    // rows & cols
    for (int i = 0; i < rows; ++i) {
        if (three_eq(board[i][0], board[i][1], board[i][2], blank_symbol) && board[i][0] == opp)
            return true;
        if (three_eq(board[0][i], board[1][i], board[2][i], blank_symbol) && board[0][i] == opp)
            return true;
    }

    // diagonals
    if (three_eq(board[0][0], board[1][1], board[2][2], blank_symbol) && board[1][1] == opp)
        return true;
    if (three_eq(board[0][2], board[1][1], board[2][0], blank_symbol) && board[1][1] == opp)
        return true;

    return false;
}

/**
 * @brief Check if any player has 3-in-row.
 *
 * @return true if any symbol has 3-in-row
 * @return false otherwise
 */
bool Misere_TTT_Board::any_three_in_row() const {
    for (int i = 0; i < rows; ++i) {
        if (three_eq(board[i][0], board[i][1], board[i][2], blank_symbol)) return true;
        if (three_eq(board[0][i], board[1][i], board[2][i], blank_symbol)) return true;
    }
    if (three_eq(board[0][0], board[1][1], board[2][2], blank_symbol)) return true;
    if (three_eq(board[0][2], board[1][1], board[2][0], blank_symbol)) return true;
    return false;
}

/**
 * @brief Check if the game is a draw.
 *
 * @param player Pointer to Player object (not used)
 * @return true if board full and nobody has 3-in-row
 * @return false otherwise
 */
bool Misere_TTT_Board::is_draw(Player<char>* /*player*/) {
    return (n_moves == rows * columns) && !any_three_in_row();
}

/**
 * @brief Check if the game is over (win, lose, or draw).
 *
 * @param player Pointer to Player object
 * @return true if game is over
 * @return false otherwise
 */
bool Misere_TTT_Board::game_is_over(Player<char>* player) {
    if (player) {
        if (is_lose(player)) return true;
        if (is_win(player)) return true;
        if (is_draw(player)) return true;
        return false;
    }
    return any_three_in_row() || (n_moves == rows * columns);
}

// ---------------- Misere_UI ----------------

/**
 * @brief Constructor for Misère Tic-Tac-Toe UI.
 * Sets the game title and board size.
 */
Misere_UI::Misere_UI() : UI<char>("Welcome to FCAI Misere Tic-Tac-Toe", 3) {}

/**
 * @brief Create a new player (human or computer).
 *
 * @param name Player's name
 * @param symbol Player's symbol (X or O)
 * @param type Player type (HUMAN or COMPUTER)
 * @return Player<char>* Pointer to the created player
 */
Player<char>* Misere_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";
    return new Player<char>(name, symbol, type);
}

/**
 * @brief Get a move from the player.
 * For human: ask for input. For computer: choose random move.
 *
 * @param player Pointer to Player object
 * @return Move<char>* Pointer to the move
 */
Move<char>* Misere_UI::get_move(Player<char>* player) {
    int x, y;
    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlease enter your move x and y (0 to 2): ";
        cin >> x >> y;
    }
    else {
        Board<char>* b = player->get_board_ptr();
        x = rand() % b->get_rows();
        y = rand() % b->get_columns();
    }
    return new Move<char>(x, y, player->get_symbol());
}
