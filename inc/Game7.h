/**
 * @file Game7.h
 * @brief 4x4 Tic-Tac-Toe with moving tokens and Minimax AI
 * @details This file contains declarations for a variant of Tic-Tac-Toe where players
 *          move their tokens on a 4x4 board. Players start with alternating tokens
 *          at opposite ends and must form a line of 3 to win. The AI uses Minimax
 *          algorithm with Alpha-Beta pruning for strategic play.
 */

#ifndef GAME7_H
#define GAME7_H

#include "BoardGame_Classes.h"
#include <vector>

/**
 * @class Game7_Move
 * @brief Extended Move class that includes source position
 * @details Represents a move from one position to another, storing both
 *          the starting position and destination position.
 */
class Game7_Move : public Move<char> {
private:
    int from_x, from_y;
public:
    /**
     * @brief Constructor
     * @param fx Source row position
     * @param fy Source column position
     * @param tx Destination row position
     * @param ty Destination column position
     * @param sym Player's symbol ('X' or 'O')
     */
    Game7_Move(int fx, int fy, int tx, int ty, char sym)
        : Move<char>(tx, ty, sym), from_x(fx), from_y(fy) {}

    /**
     * @brief Get source row position
     * @return Row index of source position
     */
    int get_from_x() const { return from_x; }

    /**
     * @brief Get source column position
     * @return Column index of source position
     */
    int get_from_y() const { return from_y; }
};

/**
 * @class Game7_Board
 * @brief Board class for 4x4 moving token Tic-Tac-Toe
 * @details Manages a 4x4 board where players move their tokens to adjacent cells.
 *          Initial setup: Row 0 = O X O X, Row 3 = X O X O, middle rows empty.
 *          Win by forming 3 in a row horizontally, vertically, or diagonally.
 *          Includes Minimax AI with Alpha-Beta pruning for computer players.
 */
class Game7_Board : public Board<char> {
private:
    char blank = '.';

    /**
     * @brief Minimax algorithm with Alpha-Beta Pruning
     * @param current_player Symbol of player making the current move
     * @param ai_symbol Symbol of the AI player
     * @param depth Current depth in the search tree
     * @param max_depth Maximum depth to search
     * @param alpha Best score the maximizer can guarantee
     * @param beta Best score the minimizer can guarantee
     * @return Evaluation score for the current position
     * @details Recursively evaluates moves, pruning branches that cannot
     *          affect the final decision to improve performance.
     */
    int minimax(char current_player, char ai_symbol, int depth, int max_depth,
                int alpha, int beta);

    /**
     * @brief Helper to check win for a specific symbol
     * @param symbol The symbol to check ('X' or 'O')
     * @return True if symbol forms 3 in a row (any direction)
     * @details Checks all horizontal, vertical, and diagonal lines
     */
    bool check_win_for_symbol(char symbol);

public:
    /**
     * @brief Constructor - initializes 4x4 board with starting positions
     * @details Sets up board with alternating patterns:
     *          Row 0: O X O X
     *          Row 1-2: empty (blank)
     *          Row 3: X O X O
     */
    Game7_Board();

    /**
     * @brief Update board with a move
     * @param move Pointer to Game7_Move containing source and destination
     * @return True if move is valid and applied, false otherwise
     * @details Validates: bounds, source has player's symbol, destination is empty,
     *          move is to adjacent cell (Manhattan distance = 1)
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Check if player has won
     * @param player Pointer to player to check
     * @return True if player has 3 in a row (horizontal, vertical, or diagonal)
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Check if player has lost
     * @param player Pointer to player (unused)
     * @return Always false (no lose condition separate from opponent win)
     */
    bool is_lose(Player<char>* player) override { return false; }

    /**
     * @brief Check if game is a draw
     * @param player Pointer to player (unused)
     * @return Always false (game continues until someone wins)
     */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief Check if game is over
     * @param player Pointer to player to check
     * @return True if player has won
     */
    bool game_is_over(Player<char>* player) override;

    /**
     * @brief Get best move using Minimax algorithm
     * @param ai_symbol Symbol of the AI player ('X' or 'O')
     * @param difficulty Search depth (higher = smarter but slower)
     * @return Pointer to Game7_Move with optimal move
     * @details Uses Alpha-Beta pruning for efficiency. Default difficulty is 5.
     *          Returns nullptr if no valid moves available.
     */
    Game7_Move* get_best_move(char ai_symbol, int difficulty = 5);
};

/**
 * @class Game7_Player
 * @brief Player class for Game7
 * @details Handles both human and computer players. Computer uses Minimax AI.
 */
class Game7_Player : public Player<char> {
public:
    /**
     * @brief Constructor
     * @param n Player name
     * @param s Player symbol ('X' or 'O')
     * @param t Player type (HUMAN or COMPUTER)
     */
    Game7_Player(string n, char s, PlayerType t)
        : Player<char>(n, s, t) {}

    /**
     * @brief Get move from player
     * @return Pointer to Game7_Move object with chosen positions
     * @details Computer players use Minimax AI at difficulty level 5
     */
    Move<char>* get_move();
};

/**
 * @class Game7_UI
 * @brief UI class for Game7
 * @details Manages user interface, input/output, and game flow for the moving token game
 */
class Game7_UI : public UI<char> {
public:
    /**
     * @brief Constructor - displays game title
     */
    Game7_UI() : UI<char>("4x4 Tic Tac Toe (Moving Tokens) - Smart AI!", 2) {}

    /**
     * @brief Create a player
     * @param name Player name
     * @param symbol Player symbol ('X' or 'O')
     * @param type Player type (HUMAN or COMPUTER)
     * @return Pointer to newly created Game7_Player
     */
    Player<char>* create_player(string& name, char symbol, PlayerType type);

    /**
     * @brief Get move from player through UI
     * @param player Pointer to current player
     * @return Pointer to Game7_Move object with player's choice
     * @details For humans: prompts for source and destination positions
     *          For computers: uses Minimax AI to determine best move
     */
    Move<char>* get_move(Player<char>* player) override;
};

#endif