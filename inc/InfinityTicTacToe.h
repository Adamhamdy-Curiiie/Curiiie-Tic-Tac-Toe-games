/**
* @file InfinityTicTacToe.h
 * @brief Infinity Tic-Tac-Toe where moves vanish after every 3 turns
 */

#ifndef _INFINITY_TICTACTOE_H
#define _INFINITY_TICTACTOE_H

#include "BoardGame_Classes.h"
#include <queue>
#include <utility>

using namespace std;

/**
 * @class InfinityBoard
 * @brief 3x3 board where the oldest move vanishes every 3 turns
 */
class InfinityBoard : public Board<char> {
private:
    char blank = '.';
    queue<pair<int, int>> move_history;
    int move_counter;

    void remove_oldest_move();
    bool check_win_for_symbol(char symbol);

public:
    InfinityBoard();

    /**
     * @brief Apply move and remove oldest if 3 moves have passed
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Check if player has 3 in a row
     */
    bool is_win(Player<char>* player) override;

    bool is_lose(Player<char>* player) override;

    /**
     * @brief Always false (game continues indefinitely)
     */
    bool is_draw(Player<char>* player) override;

    bool game_is_over(Player<char>* player) override;
};

/**
 * @class InfinityPlayer
 * @brief Player for Infinity Tic-Tac-Toe
 */
class InfinityPlayer : public Player<char> {
public:
    InfinityPlayer(string n, char s, PlayerType t);

    /**
     * @brief Get random valid move
     */
    Move<char>* get_random_move();
};

/**
 * @class InfinityUI
 * @brief UI handler for Infinity Tic-Tac-Toe
 */
class InfinityUI : public UI<char> {
public:
    InfinityUI();
    Move<char>* get_move(Player<char>* player) override;
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
};

#endif