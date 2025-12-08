#ifndef WORDTICTACTOE_H
#define WORDTICTACTOE_H

#include "BoardGame_Classes.h"
#include <fstream>
#include <set>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <limits>
using namespace std;

// ============================================
/**
 * @file WordTicTacToe.h
 * @brief Word Tic-Tac-Toe game implementation with dictionary validation
 * @details This file contains the complete implementation of Word Tic-Tac-Toe
 *          where players try to form valid words on a 3x3 board
 */
// ============================================

// ============================================
/**
 * @class WordTicTacToe_Board
 * @brief Game board for Word Tic-Tac-Toe
 * @details Manages a 3x3 board where players place letters to form valid words.
 *          The board validates words against a dictionary loaded from file.
 * @extends Board<char>
 */
// ============================================
class WordTicTacToe_Board : public Board<char>
{
private:
    set<string> dictionary;  ///< Set containing all valid words from dictionary

    /**
     * @brief Loads the dictionary from dic.txt file
     * @details Reads words from file, converts to uppercase, and stores in set.
     *          Each line in file should contain one word.
     * @throws runtime_error If file cannot be opened
     * @throws runtime_error If dictionary is empty after loading
     * @note Words are automatically converted to uppercase for case-insensitive matching
     */
    void load_dictionary();

    /**
     * @brief Checks if a word exists in the dictionary
     * @param word The word to validate (case-insensitive)
     * @return true if word exists in dictionary, false otherwise
     * @note Converts input to uppercase before checking
     */
    bool is_valid_word(const string& word);

    /**
     * @brief Checks all possible directions for valid words
     * @details Scans horizontally, vertically, and diagonally (both directions)
     *          for 3-letter sequences that form valid words
     * @return true if any valid word is found in any direction, false otherwise
     */
    bool check_all_directions();

public:
    /**
     * @brief Constructor - Creates a 3x3 empty board
     * @details Initializes board dimensions to 3x3, sets all cells to '.',
     *          loads dictionary from file, and seeds random generator
     * @throws runtime_error If dictionary loading fails
     */
    WordTicTacToe_Board();

    /**
     * @brief Virtual destructor
     * @details Uses default destructor (no dynamic resources to clean)
     */
    virtual ~WordTicTacToe_Board() = default;

    /**
     * @brief Updates the board with a player's move
     * @param move Pointer to Move object containing position (x,y) and symbol
     * @return true if move was valid and applied, false if position already occupied
     * @details Validates move coordinates and checks if cell is empty before placing symbol
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Checks if the current player has won
     * @param player Pointer to current player (unused in this implementation)
     * @return true if any valid 3-letter word exists on the board, false otherwise
     * @details Win condition: forming any valid dictionary word in any direction
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Checks if the current player has lost
     * @param player Pointer to current player
     * @return Always returns false (no explicit lose condition in this game)
     * @note This game has no losing condition, only win or draw
     */
    bool is_lose(Player<char>* player) override;

    /**
     * @brief Checks if the game is a draw
     * @param player Pointer to current player
     * @return true if board is full with no valid words formed, false otherwise
     * @details Draw condition: all 9 cells filled and no player has won
     */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief Checks if the game has ended
     * @param player Pointer to current player
     * @return true if game is won or drawn, false if game continues
     * @details Combines win and draw checks to determine game state
     */
    bool game_is_over(Player<char>* player) override;

    /**
     * @brief Get reference to dictionary for AI use
     * @return const reference to the dictionary set
     */
    const set<string>& get_dictionary() const { return dictionary; }
};

// ============================================
/**
 * @class WordTicTacToe_UI
 * @brief User interface for Word Tic-Tac-Toe game
 * @details Handles displaying the board and getting player input
 * @extends UI<char>
 */
// ============================================
class WordTicTacToe_UI : public UI<char>
{
public:
    /**
     * @brief Constructor for UI
     * @details Initializes the user interface component
     */
    WordTicTacToe_UI();

    /**
     * @brief Gets a move from the player via console input
     * @param player Pointer to current player making the move
     * @return Pointer to Move object containing x, y coordinates and symbol
     * @retval nullptr If input fails or is invalid
     * @throws runtime_error If input fails repeatedly (excessive errors)
     * @details Prompts user for row (0-2), column (0-2), and letter (A-Z)
     * @note Validates input ranges and letter format before returning
     */
    Move<char>* get_move(Player<char>* player) override;

    /**
     * @brief Sets up players for the game (OVERRIDE)
     * @return Array of two Player pointers
     * @details Creates appropriate player types (Human, Random, or Smart AI)
     */
    Player<char>** setup_players() override;
};

// ============================================
/**
 * @class WordTicTacToe_RandomPlayer
 * @brief AI player that makes random valid moves
 * @details Computer player that randomly selects empty cells and letters
 * @extends Player<char>
 */
// ============================================
class WordTicTacToe_RandomPlayer : public Player<char>
{
public:
    /**
     * @brief Constructor for random player
     * @param name Player's name (e.g., "Computer")
     * @param symbol Player's symbol (unused in this variant, letters are dynamic)
     * @details Initializes player with given name and symbol
     */
    WordTicTacToe_RandomPlayer(const string& name, char symbol);

    /**
     * @brief Generates a random valid move
     * @return Pointer to Move object with random empty position and random letter (A-Z)
     * @throws runtime_error If no empty cells are available on the board
     * @details Scans board for empty cells, randomly selects one, and assigns random letter
     * @note Uses rand() for randomization - ensure srand() is called during initialization
     */
    Move<char>* get_random_move();
};

// ============================================
/**
 * @class WordTicTacToe_SmartPlayer
 * @brief Intelligent AI player using Minimax algorithm
 * @details Uses strategic thinking to find best moves and form valid words
 * @extends Player<char>
 */
// ============================================
class WordTicTacToe_SmartPlayer : public Player<char>
{
private:
    /**
     * @brief Evaluates board state and returns score
     * @param board Current board state
     * @param dictionary Reference to valid words dictionary
     * @return Score: positive if advantageous, negative if disadvantageous
     * @details Checks for winning words and strategic positions
     */
    int evaluate_board(const vector<vector<char>>& board, const set<string>& dictionary);

    /**
     * @brief Finds all possible words that can be formed with given letters
     * @param letters Vector of available letters
     * @param dictionary Reference to valid words dictionary
     * @return Vector of valid 3-letter words
     */
    vector<string> find_possible_words(const vector<char>& letters, const set<string>& dictionary);

    /**
     * @brief Checks if placing a letter at position can contribute to valid word
     * @param board Current board state
     * @param x Row position
     * @param y Column position
     * @param letter Letter to place
     * @param dictionary Reference to dictionary
     * @return Score indicating strategic value of the move
     */
    int calculate_move_value(const vector<vector<char>>& board, int x, int y,
                             char letter, const set<string>& dictionary);

public:
    /**
     * @brief Constructor for smart AI player
     * @param name Player's name (e.g., "Smart AI")
     * @param symbol Player's symbol
     * @details Initializes intelligent player with AI type
     */
    WordTicTacToe_SmartPlayer(const string& name, char symbol);

    /**
     * @brief Generates an intelligent move using strategic analysis
     * @return Pointer to Move object with best strategic position and letter
     * @details Analyzes board state, evaluates possible words, and selects optimal move
     * @note Much smarter than random player - tries to form valid words
     */
    Move<char>* get_smart_move();
};

// ============================================
/**
 * @brief Helper functions for Smart AI
 */
// ============================================

/**
 * @brief Checks if current board state is a winning position
 * @param board Current board configuration
 * @param dictionary Valid words dictionary
 * @return true if any valid word exists, false otherwise
 */
bool can_win_with_move(const vector<vector<char>>& board, const set<string>& dictionary);

/**
 * @brief Evaluates defensive value of a move
 * @param board Current board state
 * @param x Row position
 * @param y Column position
 * @param letter Letter being placed
 * @param dictionary Valid words dictionary
 * @return Blocking score (higher = better defensive move)
 */
int evaluate_blocking_move(const vector<vector<char>>& board, int x, int y,
                           char letter, const set<string>& dictionary);

/**
 * @brief Advanced strategic evaluation of a move
 * @param board Current board state
 * @param x Row position
 * @param y Column position
 * @param letter Letter being placed
 * @param dictionary Valid words dictionary
 * @return Strategic value score
 */
int calculate_advanced_move_value(const vector<vector<char>>& board, int x, int y,
                                  char letter, const set<string>& dictionary);

/**
 * @brief Evaluates word completion potential of a sequence
 * @param seq String sequence with '?' for empty positions
 * @param dictionary Valid words dictionary
 * @return Score based on completion potential
 */
int evaluate_sequence(const string& seq, const set<string>& dictionary);

#endif // WORDTICTACTOE_H