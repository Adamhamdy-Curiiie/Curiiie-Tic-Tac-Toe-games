#ifndef MEMORY_CLASSES_H
#define MEMORY_CLASSES_H

#include "BoardGame_Classes.h"
#include <iostream>
#include <iomanip>
#include <cctype>
using namespace std;

/**
 * @class Memory_Board
 * @brief Represents the Memory Tic-Tac-Toe board (3x3) with hidden cells.
 *
 * Inherits from Board<char> and handles moves, win/draw checks,
 * and hidden board display for memory-based gameplay.
 */
class Memory_Board : public Board<char> {
private:
    char blank_symbol = '.'; /**< Character for empty cell */

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

    /**
     * @brief Get a hidden version of the board
     * @param round_completed If true, show '#' for occupied cells
     * @return 2D vector representing hidden board
     */
    vector<vector<char>> get_hidden_board(bool round_completed = false) const;
};

/**
 * @class Memory_UI
 * @brief User Interface for the Memory Tic-Tac-Toe game.
 *
 * Handles player creation, move input, and board display.
 */
class Memory_UI : public UI<char> {
public:
    /**
     * @brief Constructor with welcome message
     */
    Memory_UI();

    /**
     * @brief Destructor
     */
    ~Memory_UI() {};

    /**
     * @brief Create a player with name, symbol, and type
     * @param name Player name
     * @param symbol Player symbol ('X' or 'O')
     * @param type Player type (HUMAN or COMPUTER)
     * @return Pointer to created Player object
     */
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    /**
     * @brief Get the next move from a player
     * @param player Pointer to the player
     * @return Pointer to new Move object
     */
    Move<char>* get_move(Player<char>* player) override;
};

#endif // MEMORY_CLASSES_H
