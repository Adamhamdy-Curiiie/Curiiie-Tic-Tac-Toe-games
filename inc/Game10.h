/**
 * @file Game10.h
 * @brief Obstacles Tic-Tac-Toe game on a 6x6 grid
 */

#ifndef GAME10_H
#define GAME10_H

#include "BoardGame_Classes.h"
#include <vector>

/**
 * @class Game10_Board
 * @brief Game board that adds obstacles after each round
 */
class Game10_Board : public Board<char> {
private:
    char blank = '.';        ///< Empty cell symbol
    char obstacle = '#';     ///< Obstacle symbol
    int round_count = 0;     ///< Round counter

public:
    /**
     * @brief Initializes a 6x6 board
     */
    Game10_Board();

    /**
     * @brief Places a move and adds obstacles every 2 moves
     * @param move The move to place
     * @return true if move is valid
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Checks for 4 in a row
     * @param player Player to check
     * @return true if player wins
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief No losing condition in this game
     * @return Always false
     */
    bool is_lose(Player<char>* player) override { return false; }

    /**
     * @brief Checks if board is full
     * @param player Player to check
     * @return true if draw
     */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief Checks if game ended
     * @param player Player to check
     * @return true if win or draw
     */
    bool game_is_over(Player<char>* player) override;

private:
    /**
     * @brief Adds 2 random obstacles to the board
     */
    void add_obstacles();

    /**
     * @brief Counts empty cells on board
     * @return Number of empty cells
     */
    int count_empty_cells();
};

/**
 * @class Game10_Player
 * @brief Player with random move generation
 */
class Game10_Player : public Player<char> {
public:
    /**
     * @brief Creates a player
     * @param n Name
     * @param s Symbol
     * @param t Player type
     */
    Game10_Player(string n, char s, PlayerType t)
        : Player<char>(n, s, t) {}

    /**
     * @brief Generates a random valid move
     * @return Random move or nullptr
     */
    Move<char>* get_random_move();

private:
    /**
     * @brief Gets all valid moves
     * @return Vector of valid moves
     */
    vector<Move<char>*> get_valid_moves();
};

/**
 * @class Game10_UI
 * @brief User interface for the game
 */
class Game10_UI : public UI<char> {
public:
    /**
     * @brief Initializes UI
     */
    Game10_UI() : UI<char>("Obstacles Tic-Tac-Toe (6x6 Grid)", 2) {}

    /**
     * @brief Creates a player
     * @param name Player name
     * @param symbol Player symbol
     * @param type Player type
     * @return New player instance
     */
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    /**
     * @brief Gets move from player
     * @param player The player
     * @return The move
     */
    Move<char>* get_move(Player<char>* player) override;
};

#endif