/**
 * @file SUS_Game.h
 * @brief Header file for the SUS Game (S-U-S sequence formation game)
 */

#ifndef SUS_GAME_H
#define SUS_GAME_H

#include "BoardGame_Classes.h"
#include <vector>

using namespace std;

/**
 * @class SUS_Board
 * @brief Represents the SUS game board (3x3)
 *
 * Players place 'S' or 'U' letters on the board to form S-U-S sequences.
 * Sequences can be formed horizontally, vertically, or diagonally.
 * Each player earns points for completing S-U-S patterns.
 */
class SUS_Board : public Board<char> {
private:
    char blank = '.'; /**< Character representing empty cell */
    char last_player; /**< Track which player made the last move */
    int Total_SUS = 0; /**< Total number of S-U-S sequences on the board */
    int score_X = 0;   /**< Score for player X */
    int score_O = 0;   /**< Score for player O */

    /**
     * @brief Count total number of S-U-S sequences on the board
     * @return Total count of complete S-U-S patterns
     */
    int count_total_sus();

    /**
     * @brief Check for new S-U-S sequences and update player score
     * @param player_symbol The symbol of the player who just moved ('X' or 'O')
     */
    void check_and_count_sus(char player_symbol);

public:
    /**
     * @brief Default constructor - initializes a 3x3 empty board
     */
    SUS_Board();

    /**
     * @brief Apply a player's move to the board
     * @param move Pointer to the move object containing position and letter
     * @return True if move is valid and applied; false otherwise
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Check if a player has won the game
     * @param player Pointer to the player to check
     * @return True if the player has the highest score when board is full
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Check if a player has lost (not used in SUS game)
     * @param player Pointer to the player
     * @return Always false
     */
    bool is_lose(Player<char>* player) override;

    /**
     * @brief Check if the game ended in a draw
     * @param player Pointer to the player
     * @return True if board is full and scores are equal
     */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief Check if the game is over
     * @param player Pointer to the player
     * @return True if all 9 cells are filled
     */
    bool game_is_over(Player<char>* player) override;

    /**
     * @brief Get player X's current score
     * @return Score of player X
     */
    int get_score_X() const { return score_X; }

    /**
     * @brief Get player O's current score
     * @return Score of player O
     */
    int get_score_O() const { return score_O; }
};

/**
 * @class SUS_Player
 * @brief Represents a player in the SUS game
 *
 * Handles player moves including choosing which letter (S or U) to place
 */
class SUS_Player : public Player<char> {
private:
    char chosen_letter; /**< The letter chosen for the current move */

    /**
     * @brief Get all valid move positions on the board
     * @return Vector of pointers to valid Move objects
     */
    vector<Move<char>*> get_valid_moves();

public:
    /**
     * @brief Construct a SUS player
     * @param n Player name
     * @param s Player symbol ('X' or 'O')
     * @param t Player type (HUMAN or COMPUTER)
     */
    SUS_Player(string n, char s, PlayerType t);

    /**
     * @brief Generate a random valid move for computer player
     * @return Pointer to a randomly selected Move object
     */
    Move<char>* get_random_move();

    /**
     * @brief Set the letter to be placed in the next move
     * @param letter The letter to place ('S' or 'U')
     */
    void set_chosen_letter(char letter) { chosen_letter = letter; }

    /**
     * @brief Get the currently chosen letter
     * @return The letter chosen for placement
     */
    char get_chosen_letter() const { return chosen_letter; }
};

/**
 * @class SUS_UI
 * @brief User Interface for the SUS game
 *
 * Handles game display, player creation, and move input
 */
class SUS_UI : public UI<char> {
public:
    /**
     * @brief Constructor - displays game title and rules
     */
    SUS_UI();

    /**
     * @brief Create a player for the game
     * @param name Player's name
     * @param symbol Player's symbol ('X' or 'O')
     * @param type Player type (HUMAN or COMPUTER)
     * @return Pointer to the created SUS_Player object
     */
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    /**
     * @brief Get the next move from a player
     * @param player Pointer to the player making the move
     * @return Pointer to a Move object representing the player's choice
     */
    Move<char>* get_move(Player<char>* player) override;
};

#endif // SUS_GAME_H