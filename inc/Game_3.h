#ifndef F5_BOARD_H
#define F5_BOARD_H

#include "BoardGame_Classes.h"
#include <string>
#include <stdexcept>
#include <iostream>

using namespace std;

/**
 * @file Game_3.h
 * @brief Header file for 5x5 Tic-Tac-Toe game implementation with AI support
 *
 * This file contains the class definitions for a 5x5 variant of Tic-Tac-Toe
 * where the game ends after 24 moves and the winner is determined by who
 * has more three-in-a-row sequences. Now includes Smart AI player support.
 */

/*
    Exception classes
*/

/**
 * @class OutOfBoardException
 * @brief Exception thrown when a move is attempted outside the board boundaries
 *
 * This exception is raised when a player attempts to place a symbol at coordinates
 * that fall outside the valid board range.
 */
class OutOfBoardException : public runtime_error {
public:
    /**
     * @brief Constructor for OutOfBoardException
     * @param msg Error message describing the exception (default: "Move is outside the board")
     */
    explicit OutOfBoardException(const string& msg = "Move is outside the board")
            : runtime_error(msg) {
    }
};

/**
 * @class CellOccupiedException
 * @brief Exception thrown when attempting to place a symbol in an already occupied cell
 *
 * This exception is raised when a player tries to make a move in a cell that
 * already contains a symbol.
 */
class CellOccupiedException : public runtime_error {
public:
    /**
     * @brief Constructor for CellOccupiedException
     * @param msg Error message describing the exception (default: "This cell is already used")
     */
    explicit CellOccupiedException(const string& msg = "This cell is already used")
            : runtime_error(msg) {
    }
};

/**
 * @class TooManyMovesException
 * @brief Exception thrown when the number of moves exceeds the allowed limit
 *
 * This exception is raised when the game attempts to process more moves than
 * the maximum allowed for the current turn or game state.
 */
class TooManyMovesException : public runtime_error {
public:
    /**
     * @brief Constructor for TooManyMovesException
     * @param msg Error message describing the exception (default: "Too many moves for this turn")
     */
    explicit TooManyMovesException(const string& msg = "Too many moves for this turn")
            : runtime_error(msg) {
    }
};

/**
 * @class InvalidInputException
 * @brief Exception thrown when non-numeric input is provided where a number is expected
 *
 * This exception is raised when the user provides invalid input format,
 * such as letters instead of numbers for position coordinates.
 */
class InvalidInputException : public runtime_error {
public:
    /**
     * @brief Constructor for InvalidInputException
     * @param msg Error message describing the exception (default: "Expected a numeric position")
     */
    explicit InvalidInputException(const string& msg = "Expected a numeric position")
            : runtime_error(msg) {
    }
};

/**
 * @class InvalidSymbolException
 * @brief Exception thrown when an unsupported player symbol is used
 *
 * This exception is raised when attempting to use a symbol that is not
 * allowed in the game (e.g., symbols other than 'X' or 'O').
 */
class InvalidSymbolException : public runtime_error {
public:
    /**
     * @brief Constructor for InvalidSymbolException
     * @param msg Error message describing the exception (default: "Unsupported player symbol")
     */
    explicit InvalidSymbolException(const string& msg = "Unsupported player symbol")
            : runtime_error(msg) {
    }
};

/**
 * @class BoardUpdateException
 * @brief Exception thrown when the board state cannot be updated
 *
 * This exception is raised when an error occurs while attempting to update
 * the game board with a new move.
 */
class BoardUpdateException : public runtime_error {
public:
    /**
     * @brief Constructor for BoardUpdateException
     * @param msg Error message describing the exception (default: "Could not update board state")
     */
    explicit BoardUpdateException(const string& msg = "Could not update board state")
            : runtime_error(msg) {
    }
};


/**
 * @class FiveByFiveBoard
 * @brief Game board implementation for 5x5 Tic-Tac-Toe variant
 *
 * This class represents a 5x5 game board where the game ends after 24 moves
 * (board full except center cell). The winner is determined by counting who has
 * more three-in-a-row sequences (horizontal, vertical, or diagonal).
 *
 * Inherits from Board<char> template class.
 */
class FiveByFiveBoard : public Board<char> {
public:
    /**
     * @brief Constructor for FiveByFiveBoard
     *
     * Initializes a 5x5 game board with all cells empty.
     */
    FiveByFiveBoard();

    /**
     * @brief Updates the board with a new move
     * @param move Pointer to the move to be applied to the board
     * @return true if the move was successfully applied, false otherwise
     *
     * This method validates and applies the given move to the board,
     * checking for out-of-bounds positions and occupied cells.
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Counts the number of three-in-a-row sequences for a given symbol
     * @param symbol The player symbol to count sequences for ('X' or 'O')
     * @return The number of three-in-a-row sequences found
     *
     * This helper function scans the board horizontally, vertically, and diagonally
     * to count all instances of three consecutive symbols.
     */
    int count_three_in_a_row(char symbol);

    /**
     * @brief Checks if the given player has won the game
     * @param player Pointer to the player to check for winning condition
     * @return true if the player has won, false otherwise
     *
     * A player wins if they have more three-in-a-row sequences than their opponent
     * when the game ends.
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Checks if the given player has lost the game
     * @param player Pointer to the player to check for losing condition
     * @return true if the player has lost, false otherwise
     */
    bool is_lose(Player<char>* player) override;

    /**
     * @brief Checks if the game ended in a draw
     * @param player Pointer to a player (used for game state context)
     * @return true if the game is a draw, false otherwise
     *
     * A draw occurs when both players have the same number of three-in-a-row sequences.
     */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief Checks if the game has ended
     * @param player Pointer to a player (used for game state context)
     * @return true if the game is over (24 moves made), false otherwise
     */
    bool game_is_over(Player<char>* player) override;

    /**
     * @brief Displays the current score for both players
     *
     * Shows the number of three-in-a-row sequences each player has achieved.
     */
    void show_score();
};

/**
 * @class FiveByFivePlayer
 * @brief Human player implementation for the 5x5 Tic-Tac-Toe game
 *
 * This class is a wrapper for human players, inheriting from Player<char>.
 * It represents a player who manually inputs their moves.
 */
class FiveByFivePlayer : public Player<char> {
public:
    /**
     * @brief Constructor for FiveByFivePlayer
     * @param name The player's name
     * @param symbol The player's symbol ('X' or 'O')
     * @param type The type of player (human, AI, etc.)
     */
    FiveByFivePlayer(string name, char symbol, PlayerType type)
            : Player<char>(name, symbol, type) {}
};

/**
 * @class FiveByFiveRandomPlayer
 * @brief Random AI player implementation for the 5x5 Tic-Tac-Toe game
 *
 * This class represents an AI player that makes random valid moves by
 * selecting random empty cells on the board. This implementation is
 * primarily for testing purposes and does not employ strategic gameplay.
 */
class FiveByFiveRandomPlayer : public Player<char> {
public:
    /**
     * @brief Constructor for FiveByFiveRandomPlayer
     * @param name The AI player's name
     * @param symbol The AI player's symbol ('X' or 'O')
     * @param type The type of player (should be AI type)
     */
    FiveByFiveRandomPlayer(string name, char symbol, PlayerType type)
            : Player<char>(name, symbol, type) {}
};


/**
 * @class FiveByFiveUI
 * @brief User interface handler for the 5x5 Tic-Tac-Toe game
 *
 * This class manages user interaction, including getting moves from players
 * and creating player instances. Now supports Human, Smart AI, and Random AI players.
 * Inherits from UI<char> template class.
 */
class FiveByFiveUI : public UI<char> {
public:
    /**
     * @brief Constructor for FiveByFiveUI
     *
     * Initializes the user interface for the game.
     */
    FiveByFiveUI();

    /**
     * @brief Gets a move from the specified player
     * @param player Pointer to the player making the move
     * @return Pointer to the Move object containing the player's chosen position
     *
     * This method handles input validation and prompts the player to enter
     * their desired move coordinates. It supports Human, Smart AI, and Random AI players.
     */
    Move<char>* get_move(Player<char>* player) override;

    /**
     * @brief Creates a new player instance
     * @param name Reference to the player's name
     * @param symbol The symbol assigned to the player ('X' or 'O')
     * @param type The type of player (human, smart AI, or random AI)
     * @return Pointer to the newly created Player object
     *
     * Factory method for creating appropriate player instances based on the type.
     * Supports three types: HUMAN, AI (smart), and RANDOM.
     */
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    /**
     * @brief Sets up both players for the game
     * @return Array of two Player pointers
     *
     * This method prompts the user to configure both players, allowing them
     * to choose between Human, Smart AI, and Random AI for each player.
     * The user can create any combination (e.g., Human vs AI, AI vs AI, etc.)
     */
    Player<char>** setup_players() override;
};

#endif // F5_BOARD_H