/**
 * @file NumericalTicTacToe.h
 * @brief Numerical Tic-Tac-Toe where lines must sum to 15
 */

#ifndef _NUMERICAL_TICTACTOE_H
#define _NUMERICAL_TICTACTOE_H

#include "BoardGame_Classes.h"
#include <set>
#include <vector>

using namespace std;

/**
 * @class NumericalBoard
 * @brief 3x3 board using numbers (odd: 1-9, even: 2-8)
 */
class NumericalBoard : public Board<int> {
private:
    bool currentMoveIsOdd;
    set<int> used_numbers_odd;
    set<int> used_numbers_even;
    int blank = 0;

    bool check_sum_15(int a, int b, int c);

public:
    NumericalBoard();

    /**
     * @brief Apply move with number validation
     */
    bool update_board(Move<int>* move) override;

    /**
     * @brief Check if any line sums to 15
     */
    bool is_win(Player<int>* player) override;

    bool is_lose(Player<int>* player) override;

    /**
     * @brief Draw when board is full (9 moves)
     */
    bool is_draw(Player<int>* player) override;

    bool game_is_over(Player<int>* player) override;

    /**
     * @brief Check if number has been used
     * @param num Number to check
     * @param isOdd Whether to check odd or even pool
     */
    bool is_number_used(int num, bool isOdd);

    void set_current_player_odd(bool odd) { currentMoveIsOdd = odd; }
};

/**
 * @class NumericalPlayer
 * @brief Player with odd or even number pool
 */
class NumericalPlayer : public Player<int> {
private:
    vector<int> available_numbers_odd;
    vector<int> available_numbers_even;
    int chosen_number;
    bool isOdd;

public:
    /**
     * @brief Constructor
     * @param oddPlayer True for odd numbers (1,3,5,7,9), false for even (2,4,6,8)
     */
    NumericalPlayer(string n, int s, PlayerType t, bool oddPlayer);

    vector<Move<int>*> get_valid_moves();

    /**
     * @brief Get unused numbers for this player
     */
    vector<int> get_available_numbers();

    /**
     * @brief Get random move for computer player
     */
    Move<int>* get_random_move();

    void set_chosen_number(int num) { chosen_number = num; }
    int get_chosen_number() const { return chosen_number; }
    bool is_odd_player() const { return isOdd; }
};

/**
 * @class NumericalUI
 * @brief UI handler for Numerical Tic-Tac-Toe
 */
class NumericalUI : public UI<int> {
public:
    NumericalUI();
    Move<int>* get_move(Player<int>* player) override;
    Player<int>* create_player(string& name, int symbol, PlayerType type) override;
};

#endif