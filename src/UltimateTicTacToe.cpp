/**
 * @file UltimateTicTacToe.cpp
 * @brief Implementation of Ultimate Tic Tac Toe game classes
 * @details Contains implementations for board logic, UI display, and player behavior
 * @author Your Name
 * @date 2024
 */

#include "UltimateTicTacToe.h"

// ============================================================================
// Ultimate_TTT_Board Implementation
// ============================================================================

/**
 * @brief Constructor initializes the Ultimate Tic Tac Toe board
 * @details Creates a 9x9 board with 9 sub-boards (3x3 each)
 * @tparam T Type of symbol used (typically char)
 */
template <typename T>
Ultimate_TTT_Board<T>::Ultimate_TTT_Board() : Board<T>(9, 9) {
    sub_boards.resize(3, vector<vector<T>>(3, vector<T>(9, T())));
    main_board.resize(3, vector<T>(3, T()));
    sub_board_won.resize(3, vector<bool>(3, false));
    active_sub_board_x = -1;
    active_sub_board_y = -1;
    first_move = true;
}

/**
 * @brief Check if a 3x3 grid has three in a row
 * @param grid The 3x3 grid to check
 * @param symbol The player symbol to check for
 * @return true if player has won this grid
 */
template <typename T>
bool Ultimate_TTT_Board<T>::check_win_in_grid(const vector<vector<T>>& grid, T symbol) {
    // Check rows and columns
    for (int i = 0; i < 3; i++) {
        if (grid[i][0] == symbol && grid[i][1] == symbol && grid[i][2] == symbol)
            return true;
        if (grid[0][i] == symbol && grid[1][i] == symbol && grid[2][i] == symbol)
            return true;
    }
    // Check diagonals
    if (grid[0][0] == symbol && grid[1][1] == symbol && grid[2][2] == symbol)
        return true;
    if (grid[0][2] == symbol && grid[1][1] == symbol && grid[2][0] == symbol)
        return true;
    return false;
}

/**
 * @brief Check if a sub-board is completely filled
 * @param sub_x Sub-board row index (0-2)
 * @param sub_y Sub-board column index (0-2)
 * @return true if all cells in the sub-board are occupied
 */
template <typename T>
bool Ultimate_TTT_Board<T>::is_sub_board_full(int sub_x, int sub_y) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (sub_boards[sub_x][sub_y][i * 3 + j] == T())
                return false;
        }
    }
    return true;
}

/**
 * @brief Update main board if a sub-board has been won
 * @param sub_x Sub-board row index (0-2)
 * @param sub_y Sub-board column index (0-2)
 * @param symbol The player symbol that might have won
 */
template <typename T>
void Ultimate_TTT_Board<T>::update_main_board_if_won(int sub_x, int sub_y, T symbol) {
    if (sub_board_won[sub_x][sub_y])
        return;

    vector<vector<T>> temp_grid(3, vector<T>(3));
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            temp_grid[i][j] = sub_boards[sub_x][sub_y][i * 3 + j];
        }
    }

    if (check_win_in_grid(temp_grid, symbol)) {
        main_board[sub_x][sub_y] = symbol;
        sub_board_won[sub_x][sub_y] = true;
    }
}

/**
 * @brief Update the board with a player's move
 * @param move Pointer to move object containing position and symbol
 * @return true if move is valid and successfully applied, false otherwise
 * @details Validates move is within bounds, in correct sub-board, and cell is empty
 */
template <typename T>
bool Ultimate_TTT_Board<T>::update_board(Move<T>* move) {
    int x = move->get_x();
    int y = move->get_y();
    T symbol = move->get_symbol();

    if (x < 0 || x >= 9 || y < 0 || y >= 9)
        return false;

    int sub_x = x / 3;
    int sub_y = y / 3;
    int cell_x = x % 3;
    int cell_y = y % 3;

    if (!first_move) {
        if (active_sub_board_x != -1 && active_sub_board_y != -1) {
            if (!sub_board_won[active_sub_board_x][active_sub_board_y]) {
                if (sub_x != active_sub_board_x || sub_y != active_sub_board_y)
                    return false;
            }
        }
    }

    if (sub_board_won[sub_x][sub_y])
        return false;

    int sub_index = cell_x * 3 + cell_y;
    if (sub_boards[sub_x][sub_y][sub_index] != T())
        return false;

    sub_boards[sub_x][sub_y][sub_index] = symbol;
    this->board[x][y] = symbol;
    this->n_moves++;

    update_main_board_if_won(sub_x, sub_y, symbol);

    active_sub_board_x = cell_x;
    active_sub_board_y = cell_y;

    if (sub_board_won[active_sub_board_x][active_sub_board_y] ||
        is_sub_board_full(active_sub_board_x, active_sub_board_y)) {
        active_sub_board_x = -1;
        active_sub_board_y = -1;
    }

    first_move = false;
    return true;
}

/**
 * @brief Check if a player has won the game
 * @param player Pointer to player object
 * @return true if player has 3-in-a-row on the main board
 */
template <typename T>
bool Ultimate_TTT_Board<T>::is_win(Player<T>* player) {
    T symbol = player->get_symbol();
    return check_win_in_grid(main_board, symbol);
}

/**
 * @brief Check if a player has lost the game
 * @param player Pointer to player object
 * @return Always false (no explicit lose condition in this game)
 */
template <typename T>
bool Ultimate_TTT_Board<T>::is_lose(Player<T>* player) {
    return false;
}

/**
 * @brief Check if the game is a draw
 * @param player Pointer to player object
 * @return true if all sub-boards are decided but no winner exists
 */
template <typename T>
bool Ultimate_TTT_Board<T>::is_draw(Player<T>* player) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (!sub_board_won[i][j] && !is_sub_board_full(i, j))
                return false;
        }
    }
    return !is_win(player);
}

/**
 * @brief Check if the game is over
 * @param player Pointer to player object
 * @return true if there is a winner or the game is a draw
 */
template <typename T>
bool Ultimate_TTT_Board<T>::game_is_over(Player<T>* player) {
    return is_win(player) || is_draw(player);
}

/**
 * @brief Get the currently active sub-board coordinates
 * @param x Reference to store sub-board row (-1 if any board is valid)
 * @param y Reference to store sub-board column (-1 if any board is valid)
 */
template <typename T>
void Ultimate_TTT_Board<T>::get_active_sub_board(int& x, int& y) const {
    x = active_sub_board_x;
    y = active_sub_board_y;
}

/**
 * @brief Check if a specific sub-board has been won
 * @param sub_x Sub-board row index (0-2)
 * @param sub_y Sub-board column index (0-2)
 * @return true if the sub-board has been won by any player
 */
template <typename T>
bool Ultimate_TTT_Board<T>::is_sub_board_won_at(int sub_x, int sub_y) const {
    return sub_board_won[sub_x][sub_y];
}

/**
 * @brief Get the current state of the main board
 * @return 3x3 vector showing which player won each sub-board
 */
template <typename T>
vector<vector<T>> Ultimate_TTT_Board<T>::get_main_board() const {
    return main_board;
}

// ============================================================================
// Ultimate_TTT_UI Implementation
// ============================================================================

/**
 * @brief Constructor for Ultimate Tic Tac Toe UI
 * @details Initializes UI with welcome message and cell width
 * @tparam T Type of symbol used (typically char)
 */
template <typename T>
Ultimate_TTT_UI<T>::Ultimate_TTT_UI() : UI<T>("Welcome to Ultimate Tic Tac Toe!", 3) {}

/**
 * @brief Display the 9x9 game board with sub-board separators
 * @param matrix The complete 9x9 game board matrix
 * @details Shows column numbers, row numbers, and visual separators between sub-boards
 */
template <typename T>
void Ultimate_TTT_UI<T>::display_board_matrix(const vector<vector<T>>& matrix) const {
    if (matrix.empty() || matrix[0].empty()) return;

    cout << "\n     ";
    for (int j = 0; j < 9; ++j) {
        cout << j << "   ";
        if (j == 2 || j == 5) cout << " ";
    }
    cout << "\n   " << string(40, '-') << "\n";

    for (int i = 0; i < 9; ++i) {
        cout << " " << i << " |";
        for (int j = 0; j < 9; ++j) {
            if (matrix[i][j] == T())
                cout << "   ";
            else
                cout << " " << matrix[i][j] << " ";
            cout << "|";
            if (j == 2 || j == 5) cout << "|";
        }
        cout << "\n   " << string(40, '-') << "\n";
        if (i == 2 || i == 5) {
            cout << "   " << string(40, '=') << "\n";
        }
    }
    cout << endl;
}

/**
 * @brief Get a move from the current player
 * @param player Pointer to current player (human or computer)
 * @return Pointer to Move object containing position and symbol
 * @details For human players: prompts for input and validates against active sub-board
 *          For computer players: generates random valid move
 */
template <typename T>
Move<T>* Ultimate_TTT_UI<T>::get_move(Player<T>* player) {
    Ultimate_TTT_Board<T>* board = dynamic_cast<Ultimate_TTT_Board<T>*>(player->get_board_ptr());

    int active_x, active_y;
    board->get_active_sub_board(active_x, active_y);

    if (active_x != -1 && active_y != -1) {
        cout << "\n[REQUIRED] Must play in sub-board (" << active_x << "," << active_y << ")\n";
        cout << "That means rows " << (active_x * 3) << "-" << (active_x * 3 + 2)
             << ", columns " << (active_y * 3) << "-" << (active_y * 3 + 2) << "\n";
    } else {
        cout << "\n[INFO] You can play in any available sub-board!\n";
    }

    if (player->get_type() == PlayerType::HUMAN) {
        int x, y;
        cout << player->get_name() << " (" << player->get_symbol() << "), enter row and column (0-8): ";
        cin >> x >> y;

        // Validate input and provide helpful error message
        if (x < 0 || x >= 9 || y < 0 || y >= 9) {
            cout << "[ERROR] Invalid coordinates! Please enter values between 0 and 8.\n";
        } else if (active_x != -1 && active_y != -1) {
            int sub_x = x / 3;
            int sub_y = y / 3;
            if (sub_x != active_x || sub_y != active_y) {
                cout << "[ERROR] Invalid move! You MUST play in the required sub-board.\n";
                cout << "        Required rows: " << (active_x * 3) << "-" << (active_x * 3 + 2)
                     << ", columns: " << (active_y * 3) << "-" << (active_y * 3 + 2) << "\n";
            } else if (board->get_cell(x, y) != T()) {
                cout << "[ERROR] Cell already occupied! Choose an empty cell.\n";
            }
        } else if (board->get_cell(x, y) != T()) {
            cout << "[ERROR] Cell already occupied! Choose an empty cell.\n";
        } else {
            int sub_x = x / 3;
            int sub_y = y / 3;
            if (board->is_sub_board_won_at(sub_x, sub_y)) {
                cout << "[ERROR] This sub-board is already won! Choose another sub-board.\n";
            }
        }

        return new Move<T>(x, y, player->get_symbol());
    } else {
        // Computer player logic
        int x, y;
        if (active_x != -1 && active_y != -1) {
            do {
                int local_x = rand() % 3;
                int local_y = rand() % 3;
                x = active_x * 3 + local_x;
                y = active_y * 3 + local_y;
            } while (board->get_cell(x, y) != T());
        } else {
            do {
                x = rand() % 9;
                y = rand() % 9;
                int sub_x = x / 3;
                int sub_y = y / 3;
                if (board->is_sub_board_won_at(sub_x, sub_y))
                    continue;
                if (board->get_cell(x, y) == T())
                    break;
            } while (true);
        }
        cout << "[COMPUTER] " << player->get_name() << " plays at (" << x << "," << y << ")\n";
        return new Move<T>(x, y, player->get_symbol());
    }
}

// ============================================================================
// Ultimate_TTT_Random_Player Implementation
// ============================================================================

/**
 * @brief Constructor for random computer player
 * @param symbol Player symbol (X or O)
 * @details Creates a computer player that makes random valid moves
 * @tparam T Type of symbol used (typically char)
 */
template <typename T>
Ultimate_TTT_Random_Player<T>::Ultimate_TTT_Random_Player(T symbol)
        : Player<T>("Computer", symbol, PlayerType::RANDOM) {
    srand(time(0));
}

// ============================================================================
// Explicit Template Instantiations
// ============================================================================

template class Ultimate_TTT_Board<char>;
template class Ultimate_TTT_UI<char>;
template class Ultimate_TTT_Random_Player<char>;