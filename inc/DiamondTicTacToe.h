/**
 * @file DiamondTicTacToe.h
 * @brief Diamond Tic-Tac-Toe game with Minimax AI
 * @details This file contains the declarations for a Diamond-shaped Tic-Tac-Toe game
 *          where players must form both a 3-line and 4-line in different directions to win.
 *          The AI uses Minimax algorithm with Alpha-Beta pruning for optimal play.
 */

#ifndef _DIAMOND_TICTACTOE_H
#define _DIAMOND_TICTACTOE_H

#include "BoardGame_Classes.h"
#include <vector>
#include <utility>

using namespace std;

/**
 * @class DiamondBoard
 * @brief Board class for Diamond Tic-Tac-Toe game
 * @details Manages a 7x7 grid with a diamond-shaped playing area (37 cells).
 *          Implements win detection for simultaneous 3-line and 4-line formations
 *          in different directions. Includes Minimax AI with Alpha-Beta pruning.
 */
class DiamondBoard : public Board<char> {
private:
    char blank = '.';
    
    /**
     * @brief Initialize the diamond shape pattern on the 7x7 grid
     * @details Creates a diamond with 1, 3, 5, 7, 5, 3, 1 cells per row
     */
    void initialize_diamond_shape();
    
    /**
     * @brief Find all lines of specific length for a symbol
     * @param symbol The player's symbol ('X' or 'O')
     * @param length The length of lines to find (3 or 4)
     * @return Vector of all valid lines found
     */
    vector<vector<pair<int,int>>> find_lines_of_length(char symbol, int length);
    
    /**
     * @brief Check if two lines are in different directions
     * @param line1 First line to compare
     * @param line2 Second line to compare
     * @return True if lines have different direction vectors
     */
    bool lines_different_directions(const vector<pair<int,int>>& line1,
                                    const vector<pair<int,int>>& line2);
    
    /**
     * @brief Count shared cells between two lines
     * @param line1 First line
     * @param line2 Second line
     * @return Number of cells that appear in both lines
     */
    int count_shared_cells(const vector<pair<int,int>>& line1,
                          const vector<pair<int,int>>& line2);
    
    /**
     * @brief Check win condition for a specific symbol
     * @param symbol The symbol to check ('X' or 'O')
     * @return True if symbol has both 3-line and 4-line in different directions
     * @details Win requires lines sharing 0 or 1 cell maximum
     */
    bool check_win_for_symbol(char symbol);
    
    /**
     * @brief Minimax algorithm with Alpha-Beta Pruning
     * @param current_player Symbol of player making the current move
     * @param ai_symbol Symbol of the AI player
     * @param depth Current depth in the search tree
     * @param max_depth Maximum depth to search
     * @param alpha Best score the maximizer can guarantee
     * @param beta Best score the minimizer can guarantee
     * @return Evaluation score for the current position
     */
    int minimax(char current_player, char ai_symbol, int depth, int max_depth, int alpha, int beta);

public:
    /**
     * @brief 2D array marking valid cells in the diamond shape
     * @details True indicates cell is part of the playable diamond area
     */
    bool diamond_shape[7][7];

    /**
     * @brief Constructor - initializes 7x7 board with diamond shape
     */
    DiamondBoard();
    
    /**
     * @brief Update board with a move
     * @param move Pointer to the move to apply
     * @return True if move is valid and applied, false otherwise
     * @details Validates position is in bounds, in diamond shape, and empty
     */
    bool update_board(Move<char>* move) override;
    
    /**
     * @brief Check if player has won
     * @param player Pointer to player to check
     * @return True if player has both 3-line and 4-line in different directions
     */
    bool is_win(Player<char>* player) override;
    
    /**
     * @brief Check if player has lost
     * @param player Pointer to player (unused)
     * @return Always false (no lose condition in this game)
     */
    bool is_lose(Player<char> *) override;
    
    /**
     * @brief Check if game is a draw
     * @param player Pointer to player (unused)
     * @return True if all 37 diamond cells are filled
     */
    bool is_draw(Player<char>* player) override;
    
    /**
     * @brief Check if game is over
     * @param player Pointer to player to check
     * @return True if player won or board is full
     */
    bool game_is_over(Player<char>* player) override;
    
    /**
     * @brief Get best move using Minimax algorithm
     * @param ai_symbol Symbol of the AI player ('X' or 'O')
     * @param difficulty Search depth (higher = smarter but slower)
     * @return Pair of (row, col) coordinates for best move
     * @details Uses Alpha-Beta pruning for efficiency. Default difficulty is 3.
     */
    pair<int, int> get_best_move(char ai_symbol, int difficulty = 3);
};

/**
 * @class DiamondPlayer
 * @brief Player class for Diamond Tic-Tac-Toe
 * @details Handles both human and computer players. Computer uses Minimax AI.
 */
class DiamondPlayer : public Player<char> {
public:
    /**
     * @brief Constructor
     * @param n Player name
     * @param s Player symbol ('X' or 'O')
     * @param t Player type (HUMAN or COMPUTER)
     */
    DiamondPlayer(string n, char s, PlayerType t);
    
    /**
     * @brief Get move from player
     * @return Pointer to Move object with chosen position
     * @details Computer players use Minimax AI at difficulty level 7
     */
    Move<char>* get_move();
};

/**
 * @class DiamondUI
 * @brief UI class for Diamond Tic-Tac-Toe
 * @details Manages user interface, input/output, and game flow
 */
class DiamondUI : public UI<char> {
public:
    /**
     * @brief Constructor - displays game title and rules
     */
    DiamondUI();
    
    /**
     * @brief Get move from player through UI
     * @param player Pointer to current player
     * @return Pointer to Move object with player's choice
     * @details Handles input differently for human vs computer players
     */
    Move<char>* get_move(Player<char>* player) override;
    
    /**
     * @brief Create a player
     * @param name Player name
     * @param symbol Player symbol ('X' or 'O')
     * @param type Player type (HUMAN or COMPUTER)
     * @return Pointer to newly created DiamondPlayer
     */
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
};

#endif