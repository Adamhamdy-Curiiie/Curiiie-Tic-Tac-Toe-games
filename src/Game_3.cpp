/**
 * @file Game_3_Complete.cpp
 * @brief Complete implementation of 5x5 Tic-Tac-Toe with AI in a single file
 *
 * This file contains all classes and implementations for the 5x5 variant
 * including Smart AI using Minimax algorithm with Alpha-Beta pruning.
 */

#include "BoardGame_Classes.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <vector>
#include <stdexcept>
#include <string>
#include <algorithm>

using namespace std;

// ============================================================================
// EXCEPTION CLASSES
// ============================================================================

/**
 * @class OutOfBoardException
 * @brief Exception thrown when a move is attempted outside the board boundaries
 */
class OutOfBoardException : public runtime_error {
public:
    explicit OutOfBoardException(const string& msg = "Move is outside the board")
            : runtime_error(msg) {}
};

/**
 * @class CellOccupiedException
 * @brief Exception thrown when attempting to place a symbol in an already occupied cell
 */
class CellOccupiedException : public runtime_error {
public:
    explicit CellOccupiedException(const string& msg = "This cell is already used")
            : runtime_error(msg) {}
};

/**
 * @class TooManyMovesException
 * @brief Exception thrown when the number of moves exceeds the allowed limit
 */
class TooManyMovesException : public runtime_error {
public:
    explicit TooManyMovesException(const string& msg = "Too many moves for this turn")
            : runtime_error(msg) {}
};

/**
 * @class InvalidInputException
 * @brief Exception thrown when non-numeric input is provided where a number is expected
 */
class InvalidInputException : public runtime_error {
public:
    explicit InvalidInputException(const string& msg = "Expected a numeric position")
            : runtime_error(msg) {}
};

/**
 * @class InvalidSymbolException
 * @brief Exception thrown when an unsupported player symbol is used
 */
class InvalidSymbolException : public runtime_error {
public:
    explicit InvalidSymbolException(const string& msg = "Unsupported player symbol")
            : runtime_error(msg) {}
};

/**
 * @class BoardUpdateException
 * @brief Exception thrown when the board state cannot be updated
 */
class BoardUpdateException : public runtime_error {
public:
    explicit BoardUpdateException(const string& msg = "Could not update board state")
            : runtime_error(msg) {}
};

// ============================================================================
// BOARD CLASS
// ============================================================================

/**
 * @class FiveByFiveBoard
 * @brief Game board implementation for 5x5 Tic-Tac-Toe variant
 */
class FiveByFiveBoard : public Board<char> {
public:
    FiveByFiveBoard();
    bool update_board(Move<char>* move) override;
    int count_three_in_a_row(char symbol);
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
    void show_score();
};

/**
 * @brief Constructor for FiveByFiveBoard
 * Initializes a 5x5 game board with all cells set to '.' (empty).
 */
FiveByFiveBoard::FiveByFiveBoard() : Board<char>(5, 5) {
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
            board[i][j] = '.';
    n_moves = 0;
}

/**
 * @brief Updates the board with a new move
 * @param move Pointer to the move to be applied
 * @return true if the move was successfully applied, false otherwise
 */
bool FiveByFiveBoard::update_board(Move<char>* move) {
    try {
        if (!move)
            throw BoardUpdateException("Null move pointer");

        int x = move->get_x();
        int y = move->get_y();
        char symbol = move->get_symbol();

        if (symbol != 'X' && symbol != 'O')
            throw InvalidSymbolException("Symbol must be X or O");

        if (n_moves >= 24)
            throw TooManyMovesException("Maximum number of moves reached");

        if (x < 0 || x > 4 || y < 0 || y > 4)
            throw OutOfBoardException("Position out of range 0..4");

        if (board[x][y] != '.')
            throw CellOccupiedException("Cell already taken");

        board[x][y] = symbol;
        ++n_moves;
        return true;
    }
    catch (const runtime_error& e) {
        cerr << "[Update Error] " << e.what() << endl;
        return false;
    }
}

/**
 * @brief Counts the number of three-in-a-row sequences for a given symbol
 * @param symbol The player symbol to count sequences for ('X' or 'O')
 * @return The total number of three-in-a-row sequences found
 */
int FiveByFiveBoard::count_three_in_a_row(char symbol) {
    if (symbol != 'X' && symbol != 'O') {
        cerr << "Invalid symbol in count_three_in_a_row\n";
        return 0;
    }

    int count = 0;

    // Horizontal
    for (int r = 0; r < 5; ++r)
        for (int c = 0; c <= 2; ++c)
            if (board[r][c] == symbol &&
                board[r][c + 1] == symbol &&
                board[r][c + 2] == symbol)
                ++count;

    // Vertical
    for (int r = 0; r <= 2; ++r)
        for (int c = 0; c < 5; ++c)
            if (board[r][c] == symbol &&
                board[r + 1][c] == symbol &&
                board[r + 2][c] == symbol)
                ++count;

    // Diagonal down-right
    for (int r = 0; r <= 2; ++r)
        for (int c = 0; c <= 2; ++c)
            if (board[r][c] == symbol &&
                board[r + 1][c + 1] == symbol &&
                board[r + 2][c + 2] == symbol)
                ++count;

    // Diagonal down-left
    for (int r = 0; r <= 2; ++r)
        for (int c = 2; c < 5; ++c)
            if (board[r][c] == symbol &&
                board[r + 1][c - 1] == symbol &&
                board[r + 2][c - 2] == symbol)
                ++count;

    return count;
}

/**
 * @brief Checks if the specified player has won the game
 */
bool FiveByFiveBoard::is_win(Player<char>* player) {
    if (!player) return false;
    if (n_moves < 24) return false;

    int xScore = count_three_in_a_row('X');
    int oScore = count_three_in_a_row('O');
    char s = player->get_symbol();

    bool won = (s == 'X' && xScore > oScore) || (s == 'O' && oScore > xScore);

    // Display results if player won
    if (won) {
        cout << "\n================================\n";
        cout << "       GAME OVER - WIN!        \n";
        cout << "================================\n";
        cout << "\n  WINNER: Player " << s << "\n";
        cout << "\n  Final Score:\n";
        cout << "  Player X: " << xScore << " three-in-a-rows\n";
        cout << "  Player O: " << oScore << " three-in-a-rows\n";

        int margin = (s == 'X') ? (xScore - oScore) : (oScore - xScore);
        cout << "\n  Victory Margin: " << margin << " point";
        if (margin != 1) cout << "s";
        cout << "\n================================\n";
    }

    return won;
}

/**
 * @brief Checks if the specified player has lost the game
 */
bool FiveByFiveBoard::is_lose(Player<char>* player) {
    if (!player) return false;
    if (n_moves < 24) return false;

    int xScore = count_three_in_a_row('X');
    int oScore = count_three_in_a_row('O');
    char s = player->get_symbol();

    return (s == 'X' && oScore > xScore) ||
           (s == 'O' && xScore > oScore);
}

/**
 * @brief Checks if the game ended in a draw
 */
bool FiveByFiveBoard::is_draw(Player<char>* player) {
    if (!player) return false;
    if (n_moves < 24) return false;

    int xScore = count_three_in_a_row('X');
    int oScore = count_three_in_a_row('O');

    bool is_tie = (xScore == oScore);

    // Display results if game is draw
    if (is_tie) {
        cout << "\n================================\n";
        cout << "       GAME OVER - DRAW!        \n";
        cout << "================================\n";
        cout << "\n  Both players are tied!\n";
        cout << "\n  Final Score:\n";
        cout << "  Player X: " << xScore << " three-in-a-rows\n";
        cout << "  Player O: " << oScore << " three-in-a-rows\n";
        cout << "\n  Well played by both sides!\n";
        cout << "================================\n";
    }

    return is_tie;
}

/**
 * @brief Checks if the game has ended
 */
bool FiveByFiveBoard::game_is_over(Player<char>*) {
    return n_moves >= 24;
}

/**
 * @brief Displays the final score and winner
 */
void FiveByFiveBoard::show_score() {
    int xScore = count_three_in_a_row('X');
    int oScore = count_three_in_a_row('O');

    cout << "\n--- Final Result ---\n";
    cout << "X scored: " << xScore << "\n";
    cout << "O scored: " << oScore << "\n";

    if (xScore > oScore)
        cout << "Winner: Player X\n";
    else if (oScore > xScore)
        cout << "Winner: Player O\n";
    else
        cout << "It's a draw.\n";
}

// ============================================================================
// PLAYER CLASSES
// ============================================================================

/**
 * @class FiveByFivePlayer
 * @brief Human player implementation
 */
class FiveByFivePlayer : public Player<char> {
public:
    FiveByFivePlayer(string name, char symbol, PlayerType type)
            : Player<char>(name, symbol, type) {}
};

/**
 * @class FiveByFiveRandomPlayer
 * @brief Random AI player implementation
 */
class FiveByFiveRandomPlayer : public Player<char> {
public:
    FiveByFiveRandomPlayer(string name, char symbol, PlayerType type)
            : Player<char>(name, symbol, type) {}
};

/**
 * @class FiveByFiveAIPlayer
 * @brief Smart AI player using Minimax algorithm with Alpha-Beta pruning
 */
class FiveByFiveAIPlayer : public Player<char> {
private:
    static const int MAX_DEPTH = 3;

    /**
     * @brief Count all three-in-a-row patterns for a given symbol
     */
    int count_patterns(const vector<vector<char>>& board, char symbol) {
        int count = 0;

        // Horizontal
        for (int r = 0; r < 5; ++r)
            for (int c = 0; c <= 2; ++c)
                if (board[r][c] == symbol &&
                    board[r][c+1] == symbol &&
                    board[r][c+2] == symbol)
                    ++count;

        // Vertical
        for (int r = 0; r <= 2; ++r)
            for (int c = 0; c < 5; ++c)
                if (board[r][c] == symbol &&
                    board[r+1][c] == symbol &&
                    board[r+2][c] == symbol)
                    ++count;

        // Diagonal down-right
        for (int r = 0; r <= 2; ++r)
            for (int c = 0; c <= 2; ++c)
                if (board[r][c] == symbol &&
                    board[r+1][c+1] == symbol &&
                    board[r+2][c+2] == symbol)
                    ++count;

        // Diagonal down-left
        for (int r = 0; r <= 2; ++r)
            for (int c = 2; c < 5; ++c)
                if (board[r][c] == symbol &&
                    board[r+1][c-1] == symbol &&
                    board[r+2][c-2] == symbol)
                    ++count;

        return count;
    }

    /**
     * @brief Evaluate board position
     */
    int evaluate_board(const vector<vector<char>>& board, char symbol) {
        char opponent = (symbol == 'X') ? 'O' : 'X';
        int myScore = count_patterns(board, symbol);
        int oppScore = count_patterns(board, opponent);
        return myScore - oppScore;
    }

    /**
     * @brief Get all empty cells on the board
     */
    vector<pair<int, int>> get_empty_cells(const vector<vector<char>>& board) {
        vector<pair<int, int>> empty;
        for (int i = 0; i < 5; ++i)
            for (int j = 0; j < 5; ++j)
                if (board[i][j] == '.')
                    empty.push_back({i, j});
        return empty;
    }

    /**
     * @brief Minimax algorithm with Alpha-Beta pruning
     */
    int minimax(vector<vector<char>> board, int depth, int alpha, int beta,
                bool is_maximizing, int moves_made) {

        if (moves_made >= 24 || depth == 0) {
            return evaluate_board(board, this->symbol);
        }

        vector<pair<int, int>> empty = get_empty_cells(board);
        if (empty.empty()) {
            return evaluate_board(board, this->symbol);
        }

        if (is_maximizing) {
            int max_eval = numeric_limits<int>::min();

            for (auto& cell : empty) {
                board[cell.first][cell.second] = this->symbol;
                int eval = minimax(board, depth - 1, alpha, beta, false, moves_made + 1);
                board[cell.first][cell.second] = '.';

                max_eval = max(max_eval, eval);
                alpha = max(alpha, eval);

                if (beta <= alpha)
                    break;
            }
            return max_eval;

        } else {
            int min_eval = numeric_limits<int>::max();
            char opponent = (this->symbol == 'X') ? 'O' : 'X';

            for (auto& cell : empty) {
                board[cell.first][cell.second] = opponent;
                int eval = minimax(board, depth - 1, alpha, beta, true, moves_made + 1);
                board[cell.first][cell.second] = '.';

                min_eval = min(min_eval, eval);
                beta = min(beta, eval);

                if (beta <= alpha)
                    break;
            }
            return min_eval;
        }
    }

    /**
     * @brief Find the best move using Minimax
     */
    pair<int, int> find_best_move() {
        Board<char>* board_ptr = this->get_board_ptr();
        if (!board_ptr) {
            cerr << "[AI ERROR] Board pointer is null\n";
            return {-1, -1};
        }

        vector<vector<char>> board = board_ptr->get_board_matrix();
        vector<pair<int, int>> empty = get_empty_cells(board);

        if (empty.empty()) {
            cerr << "[AI ERROR] No empty cells\n";
            return {-1, -1};
        }

        int best_score = numeric_limits<int>::min();
        pair<int, int> best_move = empty[0];

        // Count current moves from board
        int moves_made = 0;
        for (int i = 0; i < 5; ++i)
            for (int j = 0; j < 5; ++j)
                if (board[i][j] != '.')
                    ++moves_made;

        cout << "\n[AI] Thinking";
        cout.flush();

        for (auto& cell : empty) {
            board[cell.first][cell.second] = this->symbol;

            int score = minimax(board, MAX_DEPTH - 1,
                                numeric_limits<int>::min(),
                                numeric_limits<int>::max(),
                                false, moves_made + 1);

            board[cell.first][cell.second] = '.';

            cout << ".";
            cout.flush();

            if (score > best_score) {
                best_score = score;
                best_move = cell;
            }
        }

        cout << " Done! (Score: " << best_score << ")\n";
        return best_move;
    }

public:
    /**
     * @brief Constructor for AI player
     */
    FiveByFiveAIPlayer(string name, char symbol, PlayerType type)
            : Player<char>(name, symbol, type) {}

    /**
     * @brief Get the AI's next move
     */
    Move<char>* get_move() {
        pair<int, int> move = find_best_move();

        if (move.first == -1) {
            cerr << "[AI ERROR] No valid move found\n";
            return nullptr;
        }

        cout << this->get_name() << " (AI) plays ("
             << move.first << ", " << move.second << ")\n";

        return new Move<char>(move.first, move.second, this->symbol);
    }
};

// ============================================================================
// UI CLASS
// ============================================================================

/**
 * @class FiveByFiveUI
 * @brief User interface handler for the game
 */
class FiveByFiveUI : public UI<char> {
public:
    FiveByFiveUI();
    Move<char>* get_move(Player<char>* player) override;
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Player<char>** setup_players() override;
};

/**
 * @brief Constructor for FiveByFiveUI
 */
FiveByFiveUI::FiveByFiveUI()
        : UI<char>(
        "\n=== Welcome to 5x5 Tic-Tac-Toe ===\n"
        "Goal: Get more 3-in-a-row patterns!\n"
        "Game ends after 24 moves.\n",
        3) {}

/**
 * @brief Gets a move from the specified player
 */
Move<char>* FiveByFiveUI::get_move(Player<char>* player) {
    if (!player) {
        cerr << "Player pointer is null\n";
        return nullptr;
    }

    // Check if Smart AI
    FiveByFiveAIPlayer* aiPlayer = dynamic_cast<FiveByFiveAIPlayer*>(player);
    if (aiPlayer) {
        return aiPlayer->get_move();
    }

    // Human player
    if (player->get_type() == PlayerType::HUMAN) {
        int r, c;
        cout << "\n" << player->get_name() << " ("
             << player->get_symbol() << ") enter row and col: ";

        if (!(cin >> r >> c)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cerr << "Invalid input. Enter two integers.\n";
            return nullptr;
        }

        return new Move<char>(r, c, player->get_symbol());
    }

    // Random AI
    Board<char>* board = player->get_board_ptr();
    if (!board) {
        cerr << "Board pointer is null\n";
        return nullptr;
    }

    vector<vector<char>> boardMatrix = board->get_board_matrix();
    vector<pair<int, int>> emptyCells;

    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            if (boardMatrix[i][j] == '.')
                emptyCells.push_back({i, j});

    if (emptyCells.empty()) {
        cerr << "No empty cells\n";
        return nullptr;
    }

    int randomIndex = rand() % emptyCells.size();
    int r = emptyCells[randomIndex].first;
    int c = emptyCells[randomIndex].second;

    cout << player->get_name() << " (Random) plays (" << r << ", " << c << ")\n";
    return new Move<char>(r, c, player->get_symbol());
}

/**
 * @brief Factory method to create a player
 */
Player<char>* FiveByFiveUI::create_player(string& name, char symbol, PlayerType type) {
    if (symbol != 'X' && symbol != 'O')
        throw InvalidSymbolException("Symbol must be X or O");

    if (type == PlayerType::HUMAN)
        return new FiveByFivePlayer(name, symbol, type);

    if (type == PlayerType::AI)
        return new FiveByFiveAIPlayer(name, symbol, type);

    return new FiveByFiveRandomPlayer(name, symbol, type);
}

/**
 * @brief Sets up both players
 */
Player<char>** FiveByFiveUI::setup_players() {
    Player<char>** players = new Player<char>*[2];

    // Player X
    string nameX = get_player_name("Player X");
    cout << "Choose Player X type:\n";
    cout << "1. Human\n";
    cout << "2. Smart AI (Minimax)\n";
    cout << "3. Random AI\n";
    cout << "Enter choice: ";
    int choiceX;
    cin >> choiceX;

    PlayerType typeX = (choiceX == 1) ? PlayerType::HUMAN :
                       (choiceX == 2) ? PlayerType::AI : PlayerType::RANDOM;
    players[0] = create_player(nameX, static_cast<char>('X'), typeX);

    // Player O
    string nameO = get_player_name("Player O");
    cout << "Choose Player O type:\n";
    cout << "1. Human\n";
    cout << "2. Smart AI (Minimax)\n";
    cout << "3. Random AI\n";
    cout << "Enter choice: ";
    int choiceO;
    cin >> choiceO;

    PlayerType typeO = (choiceO == 1) ? PlayerType::HUMAN :
                       (choiceO == 2) ? PlayerType::AI : PlayerType::RANDOM;
    players[1] = create_player(nameO, static_cast<char>('O'), typeO);

    return players;
}