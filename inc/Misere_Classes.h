#ifndef MISERE_CLASSES_H
#define MISERE_CLASSES_H

/**
 * @file Misere_Classes.h
 * @brief Mis�re Tic-Tac-Toe (3-in-a-row loses) specific classes.
 *
 * This file contains the class definitions for:
 * - Misere_TTT_Board: handles the board logic for Mis�re Tic-Tac-Toe
 * - Misere_UI: handles user interface, player creation, and moves
 */

#include "BoardGame_Classes.h"
#include <cctype>
using namespace std;

/**
 * @class Misere_TTT_Board
 * @brief Represents a 3x3 Mis�re Tic-Tac-Toe board.
 *
 * In Mis�re Tic-Tac-Toe, making 3-in-a-row means the player loses.
 */
class Misere_TTT_Board : public Board<char> {
private:
    char blank_symbol = '.';  ///< Symbol used for empty cells

public:
    /**
     * @brief Constructor that initializes a 3x3 board with blank symbols.
     */
    Misere_TTT_Board();

    /**
     * @brief Apply or undo a move on the board.
     * @param move Pointer to the Move object
     * @return true if the move is successfully applied or undone
     * @return false if the move is invalid
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Check if the given player has won (opponent lost).
     * @param player Pointer to the player
     * @return true if the opponent has 3-in-row (current player wins)
     * @return false otherwise
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Check if the given player has lost (made 3-in-row).
     * @param player Pointer to the player
     * @return true if the player has 3-in-row (loses)
     * @return false otherwise
     */
    bool is_lose(Player<char>* player) override;

    /**
     * @brief Check if the game is a draw.
     * @param player Pointer to the player (not used)
     * @return true if board is full and no 3-in-row exists
     * @return false otherwise
     */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief Check if the game is over (win, lose, or draw).
     * @param player Pointer to the player
     * @return true if the game is over
     * @return false otherwise
     */
    bool game_is_over(Player<char>* player) override;

    /**
     * @brief Check if any symbol has 3-in-row.
     * @return true if any player has 3-in-row
     * @return false otherwise
     */
    bool any_three_in_row() const;
};

/**
 * @class Misere_UI
 * @brief Handles the user interface for Mis�re Tic-Tac-Toe.
 *
 * Responsible for setting up players, getting moves from humans or AI,
 * and displaying prompts.
 */
class Misere_UI : public UI<char> {
public:
    /**
     * @brief Constructor for Mis�re Tic-Tac-Toe UI.
     */
    Misere_UI();

    /**
     * @brief Destructor
     */
    ~Misere_UI() {};

    /**
     * @brief Create a new player.
     * @param name Player's name
     * @param symbol Player's symbol (X or O)
     * @param type Player type (HUMAN or COMPUTER)
     * @return Player<char>* Pointer to the created player
     */
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    /**
     * @brief Get a move from the player.
     * For human players, input is requested. For AI players, a random move is generated.
     * @param player Pointer to the player
     * @return Move<char>* Pointer to the move
     */
    Move<char>* get_move(Player<char>* player) override;
};

#endif // MISERE_CLASSES_H
