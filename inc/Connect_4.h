/**
 * @file Connect_4.h
 * @brief Defines the Connect 4 game classes that extend the generic board game framework.
 *
 * This file provides:
 * - `Connect_4`: A specialized board class for the Connect 4 game.
 * - `Connect_4_UI`: A user interface class tailored to Connect 4 game setup and player interaction.
 */

#ifndef CONNECT_4_H
#define CONNECT_4_H

#include "BoardGame_Classes.h"
using namespace std;

/**
 * @class Connect_4
 * @brief Represents the Connect 4 game board.
 *
 * This class inherits from the generic `Board<char>` class and implements
 * the specific logic required for the Connect 4 game, including
 * move updates, win/draw detection using four-in-a-row checking.
 *
 * @see Board
 */
class Connect_4 : public Board<char> {
private:
    char blank_symbol = '.'; ///< Character used to represent an empty cell on the board.

public:
    int a = 0; ///< Row position of the last placed token.
    int b = 0; ///< Column position of the last placed token.

    /**
     * @brief Default constructor that initializes a 6x7 Connect 4 board.
     *
     * All cells are initialized with the blank symbol.
     */
    Connect_4();

    /**
     * @brief Updates the board with a player's move.
     *
     * The token drops to the lowest available position in the selected column.
     *
     * @param move Pointer to a Move<char> object containing column index and symbol.
     * @return true if the move is valid and successfully applied, false otherwise.
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Checks if the given player has won the game.
     *
     * Checks for four consecutive symbols horizontally, vertically, or diagonally.
     *
     * @param player Pointer to the player being checked.
     * @return true if the player has four in a row, false otherwise.
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Checks if the given player has lost the game.
     *
     * @param player Pointer to the player being checked.
     * @return Always returns false (not used in Connect 4 logic).
     */
    bool is_lose(Player<char>*) override { return false; }

    /**
     * @brief Checks if the game has ended in a draw.
     *
     * A draw occurs when all 42 cells are filled and no player has won.
     *
     * @param player Pointer to the player being checked.
     * @return true if the board is full and no player has won, false otherwise.
     */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief Determines if the game is over (win or draw).
     *
     * @param player Pointer to the player to evaluate.
     * @return true if the game has ended, false otherwise.
     */
    bool game_is_over(Player<char>* player) override;
};

/**
 * @class Connect_4_UI
 * @brief User Interface class for the Connect 4 game.
 *
 * Inherits from the generic `UI<char>` base class and provides
 * Connect 4 specific functionality for player setup and move input.
 *
 * @see UI
 */
class Connect_4_UI : public UI<char> {
public:
    /**
     * @brief Constructs a Connect_4_UI object.
     *
     * Initializes the base `UI<char>` class with a welcome message.
     */
    Connect_4_UI();

    /**
     * @brief Destructor for Connect_4_UI.
     */
    ~Connect_4_UI() {}

    /**
     * @brief Creates a player of the specified type.
     *
     * @param name Name of the player.
     * @param symbol Character symbol ('X' or 'O') assigned to the player.
     * @param type The type of the player (Human or Computer).
     * @return Pointer to the newly created Player<char> instance.
     */
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    /**
     * @brief Retrieves the next move from a player.
     *
     * For human players, prompts for column input. For computer players,
     * generates a random valid column.
     *
     * @param player Pointer to the player whose move is being requested.
     * @return A pointer to a new `Move<char>` object representing the player's action.
     */
    Move<char>* get_move(Player<char>* player) override;
};

#endif // CONNECT_4_H