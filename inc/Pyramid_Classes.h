#ifndef PYRAMID_CLASSES_H
#define PYRAMID_CLASSES_H

#include "BoardGame_Classes.h"
#include <array>
#include <vector>
#include <utility>
#include <cctype>
using namespace std;

/**
 * @class Pyramid_Board
 * @brief Represents the Pyramid-shaped Tic-Tac-Toe board.
 *
 * The board has 3 rows and 5 columns. Certain cells are marked as invalid,
 * forming a pyramid shape. Winning lines are predefined triples of cells.
 */
class Pyramid_Board : public Board<char> {
private:
    char blank_symbol = '.';      ///< Character used to mark empty playable cells.
    char invalid_blank = '#';     ///< Character used to mark invalid cells.
    vector<array<pair<int, int>, 3>> winning_lines; ///< All possible winning triples.

    /**
     * @brief Initialize all board cells with blank and invalid symbols.
     */
    void init_board();

    /**
     * @brief Define all possible winning lines for the pyramid shape.
     */
    void init_winning_lines();

public:
    /**
     * @brief Construct a Pyramid board and initialize it.
     */
    Pyramid_Board();

    /**
     * @brief Apply a move to the board if valid.
     * @param move Pointer to the Move object.
     * @return true if the move was applied successfully.
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Check if the given player has won.
     * @param player Pointer to the player.
     * @return true if the player has a winning line.
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Check if the given player has lost.
     * @param player Pointer to the player.
     * @return false (not used in Pyramid Tic-Tac-Toe).
     */
    bool is_lose(Player<char>* player) override { return false; }

    /**
     * @brief Check if the game ended in a draw.
     * @param player Pointer to a player (used for symbol reference).
     * @return true if all playable cells are filled and no one won.
     */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief Check if the game is over (win or draw).
     * @param player Pointer to a player.
     * @return true if the game has ended.
     */
    bool game_is_over(Player<char>* player) override;

    /**
     * @brief Determine if a cell belongs to the playable pyramid area.
     * @param x Row index.
     * @param y Column index.
     * @return true if the cell is playable.
     */
    bool is_playable_cell(int x, int y) const;
};

/**
 * @class Pyramid_UI
 * @brief Handles user interaction for Pyramid Tic-Tac-Toe.
 *
 * Responsible for creating players, getting moves from human or AI,
 * and displaying the board in pyramid shape.
 */
class Pyramid_UI : public UI<char> {
public:
    /**
     * @brief Construct the Pyramid UI and display welcome message.
     */
    Pyramid_UI();

    ~Pyramid_UI() {}

    /**
     * @brief Create a player object.
     * @param name Player's name.
     * @param symbol Player's symbol (X or O).
     * @param type Player type (HUMAN or COMPUTER).
     * @return Pointer to the created Player object.
     */
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    /**
     * @brief Get a move from a player.
     * @param player Pointer to the player making the move.
     * @return Pointer to the created Move object.
     */
    Move<char>* get_move(Player<char>* player) override;

    /**
     * @brief Display the current board in pyramid shape.
     * @param matrix 2D board matrix to display.
     */
    void display_board_matrix(const vector<vector<char>>& matrix) const;
};

#endif
