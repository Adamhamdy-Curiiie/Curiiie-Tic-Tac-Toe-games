#ifndef ULTIMATE_TTT_H
#define ULTIMATE_TTT_H

#include "BoardGame_Classes.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

// Forward declarations
template <typename T> class Ultimate_TTT_Board;
template <typename T> class Ultimate_TTT_UI;
template <typename T> class Ultimate_TTT_Random_Player;

/**
 * @brief Board class for Ultimate Tic Tac Toe game
 *
 * This game consists of a 3x3 main grid where each cell contains
 * a smaller 3x3 sub-grid. Players must win individual sub-grids
 * to claim cells on the main grid. First to get 3-in-a-row on
 * main grid wins.
 *
 * @tparam T The type of symbol used (typically char)
 */
template <typename T>
class Ultimate_TTT_Board : public Board<T> {
private:
    vector<vector<vector<T>>> sub_boards;  ///< 3x3 grid of 3x3 sub-boards
    vector<vector<T>> main_board;          ///< 3x3 main board showing winners
    vector<vector<bool>> sub_board_won;    ///< Track which sub-boards are won
    int active_sub_board_x;                ///< Current active sub-board row
    int active_sub_board_y;                ///< Current active sub-board column
    bool first_move;                       ///< Flag for first move of game

    /**
     * @brief Check if a 3x3 grid has three in a row
     * @param grid The 3x3 grid to check
     * @param symbol The player symbol to check for
     * @return true if player has won this grid
     */
    bool check_win_in_grid(const vector<vector<T>>& grid, T symbol);

    /**
     * @brief Check if a sub-board is full
     * @param sub_x Sub-board row index
     * @param sub_y Sub-board column index
     * @return true if sub-board is completely filled
     */
    bool is_sub_board_full(int sub_x, int sub_y);

    /**
     * @brief Update main board if sub-board is won
     * @param sub_x Sub-board row index
     * @param sub_y Sub-board column index
     * @param symbol The player symbol
     */
    void update_main_board_if_won(int sub_x, int sub_y, T symbol);

public:
    /**
     * @brief Constructor initializes 3x3 main grid with 3x3 sub-grids each
     */
    Ultimate_TTT_Board();

    /**
     * @brief Update board with player move
     * @param move Pointer to move object containing position and symbol
     * @return true if move is valid and applied successfully
     */
    bool update_board(Move<T>* move) override;

    /**
     * @brief Check if player has won the main board
     * @param player Pointer to player object
     * @return true if player has 3-in-a-row on main board
     */
    bool is_win(Player<T>* player) override;

    /**
     * @brief Check if player has lost
     * @param player Pointer to player object
     * @return Always false (no lose condition in this game)
     */
    bool is_lose(Player<T>* player) override;

    /**
     * @brief Check if game is a draw
     * @param player Pointer to player object
     * @return true if all sub-boards decided but no winner on main board
     */
    bool is_draw(Player<T>* player) override;

    /**
     * @brief Check if game is over
     * @param player Pointer to player object
     * @return true if there is a winner or draw
     */
    bool game_is_over(Player<T>* player) override;

    /**
     * @brief Get which sub-board is currently active
     * @param x Reference to store sub-board row
     * @param y Reference to store sub-board column
     */
    void get_active_sub_board(int& x, int& y) const;

    /**
     * @brief Check if sub-board at position is won
     * @param sub_x Sub-board row index
     * @param sub_y Sub-board column index
     * @return true if sub-board is won
     */
    bool is_sub_board_won_at(int sub_x, int sub_y) const;

    /**
     * @brief Get the main board state
     * @return 3x3 vector showing winners of each sub-board
     */
    vector<vector<T>> get_main_board() const;
};

/**
 * @brief UI class for Ultimate Tic Tac Toe game
 *
 * Handles display of the complex 9x9 grid and user input
 * Shows which sub-board is currently active
 *
 * @tparam T The type of symbol used (typically char)
 */
template <typename T>
class Ultimate_TTT_UI : public UI<T> {
public:
    /**
     * @brief Constructor with welcome message
     */
    Ultimate_TTT_UI();

    /**
     * @brief Display the 9x9 board with sub-board separators
     * @param matrix The full 9x9 game board
     */
    void display_board_matrix(const vector<vector<T>>& matrix) const override;

    /**
     * @brief Get move from player (human or computer)
     * @param player Pointer to current player
     * @return Move object with position and symbol
     */
    Move<T>* get_move(Player<T>* player) override;
};

/**
 * @brief Random computer player for Ultimate Tic Tac Toe
 *
 * Makes random valid moves within the active sub-board
 * or any available sub-board if no restriction
 *
 * @tparam T The type of symbol used (typically char)
 */
template <typename T>
class Ultimate_TTT_Random_Player : public Player<T> {
public:
    /**
     * @brief Constructor for random player
     * @param symbol Player symbol (X or O)
     */
    Ultimate_TTT_Random_Player(T symbol);
};

#endif