/**
* @file Memory_Classes.h
 * @brief Memory Tic-Tac-Toe where moves are hidden
 */
#ifndef MEMORY_CLASSES_H
#define MEMORY_CLASSES_H

#include "BoardGame_Classes.h"
#include <iostream>
#include <iomanip>
#include <cctype>
#include <vector>
using namespace std;

/**
 * @class Memory_Board
 * @brief Represents the Memory Tic-Tac-Toe board (3x3) with hidden cells.
 *
 * During the game, all cells show as '#'.
 * At the end, reveals all moves that were made.
 */
class Memory_Board : public Board<char> {
private:
    char blank_symbol = ' '; /**< Character for empty cell */
    vector<tuple<int, int, char>> move_history; /**< Stores all moves: (x, y, symbol) */

public:
    /**
     * @brief Default constructor initializes a 3x3 board
     */
    Memory_Board();

    /**
     * @brief Apply a player's move
     * @param move Pointer to the move object
     * @return True if move is valid and applied, false otherwise
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Check if a player has won
     * @param player Pointer to the player
     * @return True if the player has a winning line
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Not used for memory game
     * @return Always false
     */
    bool is_lose(Player<char>*) override { return false; }

    /**
     * @brief Check if the game ended in a draw
     * @param player Pointer to the player
     * @return True if draw
     */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief Check if the game is over (win or draw)
     * @param player Pointer to the player
     * @return True if game is over
     */
    bool game_is_over(Player<char>* player) override;
};

/**
 * @class Memory_UI
 * @brief User Interface for the Memory Tic-Tac-Toe game.
 */
class Memory_UI : public UI<char> {
    private:
    char substitute = '#';
public:
    Memory_UI();
    ~Memory_UI() {};
    /**
     * @brief Display board with hidden moves each round
     */
    void display_board_matrix(const vector<vector<char>> &matrix) const override;
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;
};

#endif // MEMORY_CLASSES_H