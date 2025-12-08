/**
 * @file WordTicTacToe.cpp
 * @brief Implementation of Word Tic-Tac-Toe game classes
 * @details Contains the complete implementation of the game board, UI, random player, and smart AI
 *          for Word Tic-Tac-Toe where players form valid dictionary words
 * @author Game Development Team
 * @date 2024
 * @version 2.0
 */

#include "WordTicTacToe.h"

// ============================================
// WordTicTacToe_Board Implementation
// ============================================

/**
 * @brief Constructs a new Word Tic-Tac-Toe board
 * @details Initializes a 3x3 board with empty spaces (' '), loads dictionary from file,
 *          and prepares the game for play
 * @throws runtime_error If dictionary file cannot be opened or is empty (via load_dictionary)
 * @see load_dictionary()
 */
WordTicTacToe_Board::WordTicTacToe_Board() : Board<char>(3, 3) {
    // Initialize board with empty spaces
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            this->board[i][j] = ' ';
        }
    }
    // Load dictionary from file
    load_dictionary();
}

/**
 * @brief Loads valid words from dictionary file into memory
 * @details Attempts to open dic.txt from multiple paths:
 *          1. Current directory (./dic.txt)
 *          2. Parent directory (../dic.txt)
 *          3. Explicit path (./dic.txt)
 *
 *          Reads words line by line, converts to uppercase, stores in set
 * @throws runtime_error Exits program if file not found or dictionary is empty
 * @note All words are automatically converted to uppercase for case-insensitive matching
 * @post dictionary set is populated with valid words
 * @warning Program terminates with exit(1) if dictionary loading fails
 */
void WordTicTacToe_Board::load_dictionary() {
    // Try different file paths
    ifstream file("dic.txt");
    if (!file.is_open()) file.open("../dic.txt");
    if (!file.is_open()) file.open("./dic.txt");

    // Check if file opened successfully
    if (!file.is_open()) {
        cerr << "Cannot open dic.txt file!" << endl;
        exit(1);
    }

    cout << "Dictionary file opened successfully!" << endl;
    string word;

    // Read words from file
    while (file >> word) {
        transform(word.begin(), word.end(), word.begin(), ::toupper);
        dictionary.insert(word);
    }
    file.close();

    // Verify dictionary is not empty
    if (dictionary.empty()) {
        cerr << "Dictionary is empty!" << endl;
        exit(1);
    }

    cout << "Dictionary loaded: " << dictionary.size() << " words!\n";
}

/**
 * @brief Checks if a word exists in the loaded dictionary
 * @param word The word to validate (any case accepted)
 * @return true if word exists in dictionary, false otherwise
 * @details Converts input word to uppercase before searching in dictionary set
 * @note Case-insensitive comparison (CAT = cat = CaT)
 * @pre Dictionary must be loaded (load_dictionary called)
 *
 * @code
 * if (is_valid_word("cat")) {
 *     cout << "CAT is valid!";
 * }
 * @endcode
 */
bool WordTicTacToe_Board::is_valid_word(const string& word) {
    // Convert to uppercase
    string upper_word = word;
    transform(upper_word.begin(), upper_word.end(), upper_word.begin(), ::toupper);
    // Search in dictionary
    return dictionary.find(upper_word) != dictionary.end();
}

/**
 * @brief Updates the board with a player's move
 * @param move Pointer to Move object containing x, y coordinates and letter symbol
 * @return true if move is valid and successfully applied, false otherwise
 * @retval true Move was valid and board updated
 * @retval false Move was invalid (null pointer, out of bounds, cell occupied, invalid letter)
 *
 * @details Performs comprehensive validation:
 *          - Checks if move pointer is not null
 *          - Validates coordinates are within 0-2 range
 *          - Ensures target cell is empty (' ')
 *          - Validates letter is A-Z (converts to uppercase)
 *          - Increments move counter on success
 *
 * @pre Board must be initialized
 * @post If successful, board[x][y] contains the letter and n_moves is incremented
 * @note Automatically converts letter to uppercase before validation
 *
 * @code
 * Move<char>* m = new Move<char>(1, 1, 'A');
 * if (board->update_board(m)) {
 *     cout << "Move successful!";
 * }
 * @endcode
 */
bool WordTicTacToe_Board::update_board(Move<char>* move) {
    if (move == nullptr) {
        cerr << "[ERROR] Move is null!" << endl;
        return false;
    }

    int x = move->get_x();
    int y = move->get_y();
    char letter = toupper(move->get_symbol());

    // Check: Valid coordinates?
    if (x < 0 || x >= 3 || y < 0 || y >= 3) {
        cout << "[INVALID] Position out of bounds!" << endl;
        return false;
    }

    // Check: Cell is empty?
    if (board[x][y] != ' ') {
        cout << "[INVALID] Cell already occupied!" << endl;
        return false;
    }

    // Check: Letter is A-Z?
    if (letter < 'A' || letter > 'Z') {
        cout << "[INVALID] Letter must be A-Z!" << endl;
        return false;
    }

    // All checks passed - place the letter
    board[x][y] = letter;
    n_moves++;
    return true;
}

/**
 * @brief Scans all possible directions for valid 3-letter words
 * @return true if any valid word found in any direction, false otherwise
 *
 * @details Checks the following directions:
 *          1. All rows (horizontal) - left to right
 *          2. All columns (vertical) - top to bottom
 *          3. Main diagonal (\) - top-left to bottom-right
 *          4. Anti-diagonal (/) - top-right to bottom-left
 *
 *          For each direction, extracts 3-letter sequence and validates against dictionary
 *
 * @note Only checks complete words (no spaces allowed in sequence)
 * @note Prints found word location to console for debugging
 * @pre Dictionary must be loaded and board must be initialized
 *
 * @see is_valid_word()
 *
 * @par Example Board:
 * @code
 * // Board contains:
 * // C A T
 * // D O G
 * // X Y Z
 * // Will find "CAT" in row 0 and "DOG" in row 1
 * @endcode
 */
bool WordTicTacToe_Board::check_all_directions() {
    string word;

    // Check rows (horizontal)
    for (int i = 0; i < 3; i++) {
        word = "";
        for (int j = 0; j < 3; j++) {
            word += (board[i][j] == ' ') ? ' ' : board[i][j];
        }
        if (word.find(' ') == string::npos && is_valid_word(word)) {
            cout << "[WIN] Valid word in row " << i << ": " << word << endl;
            return true;
        }
    }

    // Check columns (vertical)
    for (int j = 0; j < 3; j++) {
        word = "";
        for (int i = 0; i < 3; i++) {
            word += (board[i][j] == ' ') ? ' ' : board[i][j];
        }
        if (word.find(' ') == string::npos && is_valid_word(word)) {
            cout << "[WIN] Valid word in column " << j << ": " << word << endl;
            return true;
        }
    }

    // Check main diagonal
    word = "";
    for (int i = 0; i < 3; i++) {
        word += (board[i][i] == ' ') ? ' ' : board[i][i];
    }
    if (word.find(' ') == string::npos && is_valid_word(word)) {
        cout << "[WIN] Valid word in main diagonal: " << word << endl;
        return true;
    }

    // Check anti-diagonal
    word = "";
    for (int i = 0; i < 3; i++) {
        word += (board[i][2 - i] == ' ') ? ' ' : board[i][2 - i];
    }
    if (word.find(' ') == string::npos && is_valid_word(word)) {
        cout << "[WIN] Valid word in anti-diagonal: " << word << endl;
        return true;
    }

    return false;
}

/**
 * @brief Determines if the current player has won the game
 * @param player Pointer to current player (not used in this implementation)
 * @return true if a valid 3-letter word exists on the board, false otherwise
 * @details Win condition: any row, column, or diagonal forms a valid dictionary word
 * @note Parameter 'player' is required by interface but not used in logic
 * @see check_all_directions()
 */
bool WordTicTacToe_Board::is_win(Player<char>* player) {
    return check_all_directions();
}

/**
 * @brief Determines if the current player has lost the game
 * @param player Pointer to current player
 * @return Always returns false
 * @details This game has no explicit lose condition - only win or draw
 * @note Required by base class interface but always returns false in this game variant
 */
bool WordTicTacToe_Board::is_lose(Player<char>* player) {
    return false;
}

/**
 * @brief Determines if the game is a draw
 * @param player Pointer to current player
 * @return true if board is full (9 moves) and no valid word exists, false otherwise
 * @details Draw condition: all 9 cells are filled but no player has formed a valid word
 * @note Only checks for draw when n_moves reaches 9
 * @pre Board must be in valid state with accurate n_moves counter
 */
bool WordTicTacToe_Board::is_draw(Player<char>* player) {
    // Draw condition: board is full and no winner
    if (n_moves == 9) {
        return !check_all_directions();
    }
    return false;
}

/**
 * @brief Checks if the game has ended (win or draw)
 * @param player Pointer to current player
 * @return true if game is won or drawn, false if game continues
 * @details Combines win and draw checks to determine overall game state
 * @note Game cannot end in a loss (no lose condition exists)
 * @see is_win(), is_draw()
 */
bool WordTicTacToe_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

// ============================================
// WordTicTacToe_UI Implementation
// ============================================

/**
 * @brief Constructs the game user interface
 * @details Initializes UI with game title and displays welcome message with instructions
 * @post UI is ready to accept player input
 * @note Automatically prints game instructions to console
 */
WordTicTacToe_UI::WordTicTacToe_UI()
        : UI<char>("=== Word Tic-Tac-Toe ===", 3) {
    cout << "\n[GAME] Form a valid 3-letter English word to win!" << endl;
    cout << "[INFO] Enter: row (0-2), column (0-2), and letter (A-Z)\n" << endl;
}

/**
 * @brief Sets up two players for the game
 * @return Array of two Player pointers (Human, Random, or Smart AI)
 * @details Creates appropriate player types based on user selection:
 *          - Option 1: Human player (manual input)
 *          - Option 2: Random Computer (random moves)
 *          - Option 3: Smart AI (intelligent strategic moves)
 *
 * @note This overrides the base class method to support 3 player types
 * @warning Caller is responsible for deleting returned player objects
 *
 * @post Two valid player objects are created and returned in array
 *
 * @par Player Type Selection:
 * User is prompted to choose from:
 * - 1: Human Player - requires manual input for each move
 * - 2: Random Computer - makes random valid moves
 * - 3: Smart AI - uses strategic algorithm to find best moves
 */
Player<char>** WordTicTacToe_UI::setup_players() {
    Player<char>** players = new Player<char>*[2];

    // Player X Setup
    string nameX;
    cout << "Enter Player X name: ";
    getline(cin >> ws, nameX);
    cout << "Choose Player X type:\n1. Human\n2. Random Computer\n3. Smart AI\n";
    int choiceX;
    cin >> choiceX;

    if (choiceX == 2) {
        players[0] = new WordTicTacToe_RandomPlayer(nameX, 'X');
    } else if (choiceX == 3) {
        players[0] = new WordTicTacToe_SmartPlayer(nameX, 'X');
    } else {
        players[0] = new Player<char>(nameX, 'X', PlayerType::HUMAN);
    }

    // Player O Setup
    string nameO;
    cout << "Enter Player O name: ";
    getline(cin >> ws, nameO);
    cout << "Choose Player O type:\n1. Human\n2. Random Computer\n3. Smart AI\n";
    int choiceO;
    cin >> choiceO;

    if (choiceO == 2) {
        players[1] = new WordTicTacToe_RandomPlayer(nameO, 'O');
    } else if (choiceO == 3) {
        players[1] = new WordTicTacToe_SmartPlayer(nameO, 'O');
    } else {
        players[1] = new Player<char>(nameO, 'O', PlayerType::HUMAN);
    }

    return players;
}

/**
 * @brief Gets a move from the current player (human or computer)
 * @param player Pointer to current player making the move
 * @return Pointer to Move object containing the player's chosen position and letter
 * @retval nullptr If player is null or error occurs
 * @retval Move* Valid move for human players (from keyboard input)
 * @retval Move* Valid random move for random computer players
 * @retval Move* Valid smart move for smart AI players
 *
 * @details Behavior differs based on player type:
 *          - COMPUTER (Random): Calls get_random_move() for random AI player
 *          - AI (Smart): Calls get_smart_move() for intelligent AI player
 *          - HUMAN: Prompts for row, column, and letter via console
 *
 * @note Input is converted to uppercase automatically
 * @note Does not validate move (validation done in update_board)
 * @warning Caller is responsible for deleting returned Move pointer
 *
 * @see WordTicTacToe_RandomPlayer::get_random_move()
 * @see WordTicTacToe_SmartPlayer::get_smart_move()
 *
 * @code
 * Move<char>* move = ui->get_move(player);
 * if (move) {
 *     board->update_board(move);
 *     delete move;
 * }
 * @endcode
 */
Move<char>* WordTicTacToe_UI::get_move(Player<char>* player) {
    if (player == nullptr) return nullptr;

    // If Random Computer player
    if (player->get_type() == PlayerType::COMPUTER) {
        WordTicTacToe_RandomPlayer* rp = dynamic_cast<WordTicTacToe_RandomPlayer*>(player);
        if (rp) return rp->get_random_move();
    }

    // If Smart AI player
    if (player->get_type() == PlayerType::AI) {
        WordTicTacToe_SmartPlayer* sp = dynamic_cast<WordTicTacToe_SmartPlayer*>(player);
        if (sp) return sp->get_smart_move();
    }

    // If Human player
    int x, y;
    char letter;
    cout << "\n" << player->get_name() << "'s turn:" << endl;
    cout << "Enter row (0-2): ";
    cin >> x;
    cout << "Enter column (0-2): ";
    cin >> y;
    cout << "Enter letter (A-Z): ";
    cin >> letter;

    return new Move<char>(x, y, toupper(letter));
}

// ============================================
// WordTicTacToe_RandomPlayer Implementation
// ============================================

/**
 * @brief Constructs a computer-controlled random player
 * @param name Player's display name (e.g., "Computer", "AI")
 * @param symbol Player's symbol (not used in word variant but required by interface)
 * @details Initializes player as COMPUTER type and seeds random number generator
 * @note Seeds srand with current time for randomness
 * @post Player is ready to generate random moves
 */
WordTicTacToe_RandomPlayer::WordTicTacToe_RandomPlayer(const string& name, char symbol)
        : Player<char>(name, symbol, PlayerType::COMPUTER) {
    srand(time(0));
}

/**
 * @brief Generates a random valid move for the computer player
 * @return Pointer to Move object with random empty cell and random letter (A-Z)
 * @retval nullptr If no empty cells available or board pointer is null
 *
 * @details Algorithm:
 *          1. Scans entire 3x3 board for empty cells (' ')
 *          2. Stores all empty cell coordinates in vector
 *          3. Randomly selects one empty cell
 *          4. Randomly selects a letter A-Z
 *          5. Creates and returns Move object
 *
 * @throws Prints error to cerr if board is full or pointer is null
 * @note Uses rand() for randomization (seeded in constructor)
 * @note Prints move to console for transparency
 * @warning Caller must delete returned Move pointer
 * @pre Board pointer must be set via set_board()
 *
 * @code
 * Move<char>* move = randomPlayer->get_random_move();
 * if (move) {
 *     board->update_board(move);
 *     delete move;
 * }
 * @endcode
 */
Move<char>* WordTicTacToe_RandomPlayer::get_random_move() {
    Board<char>* board = this->get_board_ptr();
    if (!board) return nullptr;

    // Collect all empty cells
    vector<pair<int, int>> empty_cells;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board->get_cell(i, j) == ' ') {
                empty_cells.push_back({i, j});
            }
        }
    }

    if (empty_cells.empty()) return nullptr;

    // Select random cell
    int idx = rand() % empty_cells.size();
    int x = empty_cells[idx].first;
    int y = empty_cells[idx].second;
    char letter = 'A' + (rand() % 26);

    cout << "[RANDOM] " << get_name() << " plays: (" << x << ", " << y << ", " << letter << ")" << endl;
    return new Move<char>(x, y, letter);
}

// ============================================
// WordTicTacToe_SmartPlayer Implementation
// ============================================

/**
 * @brief Constructs an intelligent AI player
 * @param name Player's display name (e.g., "Smart AI", "Computer")
 * @param symbol Player's symbol (X or O)
 * @details Initializes player as AI type using strategic algorithms
 * @post Player is ready to generate intelligent strategic moves
 * @note This player uses heuristic evaluation to find optimal moves
 */
WordTicTacToe_SmartPlayer::WordTicTacToe_SmartPlayer(const string& name, char symbol)
        : Player<char>(name, symbol, PlayerType::AI) {}

/**
 * @brief Evaluates the current board state for strategic advantage
 * @param board Current board matrix (3x3 grid)
 * @param dictionary Reference to valid words set
 * @return Score indicating board advantage (higher = better for AI)
 *
 * @details Evaluation criteria:
 *          - Checks rows for potential word completion (+3 points)
 *          - Checks columns for potential word completion (+3 points)
 *          - Evaluates proximity to forming valid dictionary words
 *          - Positive score = advantageous position
 *          - Negative score = disadvantageous position
 *
 * @note This is a heuristic function used by the move selection algorithm
 * @pre Board must be valid 3x3 matrix
 * @pre Dictionary must be loaded with valid words
 *
 * @par Scoring System:
 * - +3: Row with 2 letters and 1 empty space (potential word)
 * - +3: Column with 2 letters and 1 empty space (potential word)
 */
int WordTicTacToe_SmartPlayer::evaluate_board(const vector<vector<char>>& board,
                                              const set<string>& dictionary) {
    int score = 0;

    // Check rows and columns for potential words
    for (int i = 0; i < 3; i++) {
        string row = "", col = "";
        int row_empty = 0, col_empty = 0;

        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') row_empty++;
            else row += board[i][j];

            if (board[j][i] == ' ') col_empty++;
            else col += board[j][i];
        }

        // Reward rows and columns close to completion
        if (row_empty == 1 && row.length() == 2) score += 3;
        if (col_empty == 1 && col.length() == 2) score += 3;
    }

    return score;
}

/**
 * @brief Finds all possible valid words that can be formed from given letters
 * @param letters Vector of available letters
 * @param dictionary Reference to valid words set
 * @return Vector of valid 3-letter words that can be formed
 *
 * @details Algorithm:
 *          1. Tries all permutations of 3 letters from available letters
 *          2. Checks each permutation against dictionary
 *          3. Returns list of all valid words found
 *
 * @note Returns empty vector if fewer than 3 letters provided
 * @pre letters must contain at least 3 characters
 * @pre dictionary must be loaded with valid words
 *
 * @par Example:
 * @code
 * vector<char> letters = {'C', 'A', 'T', 'R'};
 * vector<string> words = find_possible_words(letters, dict);
 * // Returns: ["CAT", "CAR", "TAR", "RAT", "ART", ...]
 * @endcode
 */
vector<string> WordTicTacToe_SmartPlayer::find_possible_words(const vector<char>& letters,
                                                              const set<string>& dictionary) {
    vector<string> result;
    if (letters.size() < 3) return result;

    // Try all combinations of 3 letters
    for (size_t i = 0; i < letters.size(); i++) {
        for (size_t j = 0; j < letters.size(); j++) {
            if (i == j) continue;
            for (size_t k = 0; k < letters.size(); k++) {
                if (k == i || k == j) continue;

                string word = "";
                word += letters[i];
                word += letters[j];
                word += letters[k];

                if (dictionary.find(word) != dictionary.end()) {
                    result.push_back(word);
                }
            }
        }
    }
    return result;
}

/**
 * @brief Calculates strategic value of placing a letter at specific position
 * @param board Current board state (3x3 grid)
 * @param x Row position (0-2)
 * @param y Column position (0-2)
 * @param letter Letter to be placed (A-Z)
 * @param dictionary Reference to valid words dictionary
 * @return Score indicating strategic value of this move
 *
 * @details Evaluation process:
 *          1. Creates hypothetical row/column with letter placed
 *          2. Checks if placing letter can complete valid words
 *          3. Tests all possible completions with remaining empty cells
 *          4. Awards points based on word completion potential
 *
 * @par Scoring Breakdown:
 * - +15 points: Letter can complete a valid word in row
 * - +15 points: Letter can complete a valid word in column
 * - +2 points: Bonus for center position (1,1)
 *
 * @note Higher scores indicate more strategic positions
 * @pre x and y must be valid coordinates (0-2)
 * @pre letter must be uppercase A-Z
 * @pre dictionary must be loaded
 *
 * @code
 * // Board: C A _
 * //        _ _ _
 * //        _ _ _
 * int value = calculate_move_value(board, 0, 2, 'T', dict);
 * // Returns high score because "CAT" is a valid word
 * @endcode
 */
int WordTicTacToe_SmartPlayer::calculate_move_value(const vector<vector<char>>& board,
                                                    int x, int y, char letter,
                                                    const set<string>& dictionary) {
    int value = 0;

    // Check row potential
    string row = "";
    for (int j = 0; j < 3; j++) {
        row += (j == y) ? letter : ((board[x][j] == ' ') ? '?' : board[x][j]);
    }

    // Check column potential
    string col = "";
    for (int i = 0; i < 3; i++) {
        col += (i == x) ? letter : ((board[i][y] == ' ') ? '?' : board[i][y]);
    }

    // Try completing words with all possible letters
    for (char c = 'A'; c <= 'Z'; c++) {
        string test_row = row, test_col = col;

        // Try row completion
        size_t pos = test_row.find('?');
        if (pos != string::npos) {
            test_row[pos] = c;
            if (test_row.find('?') == string::npos && dictionary.find(test_row) != dictionary.end()) {
                value += 15; // High reward for word completion
            }
        }

        // Try column completion
        pos = test_col.find('?');
        if (pos != string::npos) {
            test_col[pos] = c;
            if (test_col.find('?') == string::npos && dictionary.find(test_col) != dictionary.end()) {
                value += 15; // High reward for word completion
            }
        }
    }

    // Extra bonus for center position (strategic location)
    if (x == 1 && y == 1) value += 2;

    return value;
}

/**
 * @brief Generates an intelligent strategic move using AI algorithms
 * @return Pointer to Move object with best strategic position and letter
 * @retval Move* Optimal move based on strategic analysis
 * @retval Move* Random valid move if no strategic advantage found
 * @retval nullptr If no valid moves available
 *
 * @details AI Algorithm:
 *          1. Evaluates all empty cells on the board
 *          2. For each empty cell, tests all 26 letters (A-Z)
 *          3. Calculates strategic value for each position+letter combination
 *          4. Selects move with highest strategic value
 *          5. Falls back to random move if no strategic advantage
 *
 * @par Strategic Considerations:
 * - Prioritizes moves that can form valid words
 * - Considers both immediate wins and future opportunities
 * - Evaluates defensive positions to block opponent
 * - Prefers center positions when values are equal
 *
 * @note Much more intelligent than random player
 * @note Prints selected move to console for transparency
 * @warning Caller must delete returned Move pointer
 * @pre Board pointer must be set and valid
 * @pre Dictionary must be loaded in board
 *
 * @see calculate_move_value()
 * @see evaluate_board()
 *
 * @code
 * Move<char>* move = smartPlayer->get_smart_move();
 * if (move) {
 *     cout << "AI selected: " << move->get_x() << ", "
 *          << move->get_y() << ", " << move->get_symbol();
 *     delete move;
 * }
 * @endcode
 */
Move<char>* WordTicTacToe_SmartPlayer::get_smart_move() {
    WordTicTacToe_Board* wtb = dynamic_cast<WordTicTacToe_Board*>(this->get_board_ptr());
    if (!wtb) return nullptr;

    vector<vector<char>> board = wtb->get_board_matrix();
    const set<string>& dictionary = wtb->get_dictionary();

    int best_value = -9999;
    int best_x = -1, best_y = -1;
    char best_letter = 'A';

    // Priority 1: Look for immediate winning move
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] != ' ') continue;

            // Try every letter
            for (char c = 'A'; c <= 'Z'; c++) {
                // Place letter temporarily
                board[i][j] = c;

                // Check for immediate win
                if (can_win_with_move(board, dictionary)) {
                    board[i][j] = ' '; // Restore cell
                    cout << "[AI] " << get_name() << " (Smart AI - WINNING MOVE!) plays: ("
                         << i << ", " << j << ", " << c << ")" << endl;
                    return new Move<char>(i, j, c);
                }

                board[i][j] = ' '; // Restore cell
            }
        }
    }

    // Priority 2: Block opponent from winning
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] != ' ') continue;

            // Try every letter
            for (char c = 'A'; c <= 'Z'; c++) {
                board[i][j] = c;

                // Check if this move blocks opponent
                int block_value = evaluate_blocking_move(board, i, j, c, dictionary);

                if (block_value > 500) { // Important defensive move
                    board[i][j] = ' ';
                    cout << "[AI] " << get_name() << " (Smart AI - BLOCKING!) plays: ("
                         << i << ", " << j << ", " << c << ")" << endl;
                    return new Move<char>(i, j, c);
                }

                board[i][j] = ' ';
            }
        }
    }

    // Priority 3: Find best strategic move
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] != ' ') continue;

            // Try every letter
            for (char c = 'A'; c <= 'Z'; c++) {
                int value = calculate_advanced_move_value(board, i, j, c, dictionary);

                if (value > best_value) {
                    best_value = value;
                    best_x = i;
                    best_y = j;
                    best_letter = c;
                }
            }
        }
    }

    // Fallback: random move if no good strategic move found
    if (best_x == -1) {
        vector<pair<int, int>> empty;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') empty.push_back({i, j});
            }
        }
        if (!empty.empty()) {
            int idx = rand() % empty.size();
            best_x = empty[idx].first;
            best_y = empty[idx].second;
            best_letter = 'A' + (rand() % 26);
        }
    }

    cout << "[AI] " << get_name() << " (Smart AI) plays: ("
         << best_x << ", " << best_y << ", " << best_letter << ")" << endl;

    return new Move<char>(best_x, best_y, best_letter);
}

// ============================================
// Helper Functions
// ============================================

/**
 * @brief Checks if a move leads to immediate win
 * @param board Current board state
 * @param dictionary Reference to dictionary
 * @return true if any valid word is formed, false otherwise
 */
bool can_win_with_move(const vector<vector<char>>& board, const set<string>& dictionary) {
    string word;

    // Check rows
    for (int i = 0; i < 3; i++) {
        word = "";
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') return false;
            word += board[i][j];
        }
        if (dictionary.find(word) != dictionary.end()) return true;
    }

    // Check columns
    for (int j = 0; j < 3; j++) {
        word = "";
        for (int i = 0; i < 3; i++) {
            if (board[i][j] == ' ') return false;
            word += board[i][j];
        }
        if (dictionary.find(word) != dictionary.end()) return true;
    }

    // Check main diagonal
    word = "";
    for (int i = 0; i < 3; i++) {
        if (board[i][i] == ' ') return false;
        word += board[i][i];
    }
    if (dictionary.find(word) != dictionary.end()) return true;

    // Check anti-diagonal
    word = "";
    for (int i = 0; i < 3; i++) {
        if (board[i][2-i] == ' ') return false;
        word += board[i][2-i];
    }
    if (dictionary.find(word) != dictionary.end()) return true;

    return false;
}

/**
 * @brief Evaluates if a move blocks opponent from forming words
 * @param board Current board state
 * @param x Row position
 * @param y Column position
 * @param letter Placed letter
 * @param dictionary Reference to dictionary
 * @return Score for blocking value (higher = better block)
 */
int evaluate_blocking_move(const vector<vector<char>>& board, int x, int y,
                           char letter, const set<string>& dictionary) {
    int block_score = 0;

    // Check row completion potential
    string row = "";
    int row_empty = 0;
    for (int j = 0; j < 3; j++) {
        if (board[x][j] == ' ') row_empty++;
        else row += board[x][j];
    }

    // If row has only 2 letters, check for possible words
    if (row_empty == 0) {
        for (char c = 'A'; c <= 'Z'; c++) {
            string test = "";
            for (int j = 0; j < 3; j++) {
                test += board[x][j];
            }
            if (dictionary.find(test) != dictionary.end()) {
                block_score += 600; // Blocked a potential word!
            }
        }
    }

    // Check column with same logic
    string col = "";
    int col_empty = 0;
    for (int i = 0; i < 3; i++) {
        if (board[i][y] == ' ') col_empty++;
        else col += board[i][y];
    }

    if (col_empty == 0) {
        for (char c = 'A'; c <= 'Z'; c++) {
            string test = "";
            for (int i = 0; i < 3; i++) {
                test += board[i][y];
            }
            if (dictionary.find(test) != dictionary.end()) {
                block_score += 600;
            }
        }
    }

    return block_score;
}

/**
 * @brief Advanced calculation of move value with deep analysis
 * @param board Current board state
 * @param x Row position
 * @param y Column position
 * @param letter Letter to place
 * @param dictionary Reference to dictionary
 * @return Strategic value score
 */
int calculate_advanced_move_value(const vector<vector<char>>& board, int x, int y,
                                  char letter, const set<string>& dictionary) {
    int value = 0;

    // Analyze row
    string row = "";
    for (int j = 0; j < 3; j++) {
        if (j == y) row += letter;
        else if (board[x][j] == ' ') row += '?';
        else row += board[x][j];
    }

    // Analyze column
    string col = "";
    for (int i = 0; i < 3; i++) {
        if (i == x) col += letter;
        else if (board[i][y] == ' ') col += '?';
        else col += board[i][y];
    }

    // Analyze diagonals
    string diag1 = "", diag2 = "";
    if (x == y) { // Main diagonal
        for (int i = 0; i < 3; i++) {
            if (i == x) diag1 += letter;
            else if (board[i][i] == ' ') diag1 += '?';
            else diag1 += board[i][i];
        }
    }

    if (x + y == 2) { // Anti-diagonal
        for (int i = 0; i < 3; i++) {
            if (i == x) diag2 += letter;
            else if (board[i][2-i] == ' ') diag2 += '?';
            else diag2 += board[i][2-i];
        }
    }

    // Evaluate each direction
    value += evaluate_sequence(row, dictionary);
    value += evaluate_sequence(col, dictionary);
    if (!diag1.empty()) value += evaluate_sequence(diag1, dictionary);
    if (!diag2.empty()) value += evaluate_sequence(diag2, dictionary);

    // Additional bonuses
    if (x == 1 && y == 1) value += 5; // Center bonus
    if ((x == 0 || x == 2) && (y == 0 || y == 2)) value += 3; // Corner bonus

    return value;
}

/**
 * @brief Evaluates a sequence for word completion potential
 * @param seq Sequence with '?' for empty cells
 * @param dictionary Reference to dictionary
 * @return Score based on word potential
 */
int evaluate_sequence(const string& seq, const set<string>& dictionary) {
    if (seq.length() != 3) return 0;

    int score = 0;
    int question_marks = count(seq.begin(), seq.end(), '?');

    // No empty positions - check for complete word
    if (question_marks == 0) {
        if (dictionary.find(seq) != dictionary.end()) {
            score += 1000; // Complete word!
        }
    }
        // One empty position - try to complete word
    else if (question_marks == 1) {
        for (char c = 'A'; c <= 'Z'; c++) {
            string test = seq;
            size_t pos = test.find('?');
            test[pos] = c;

            if (dictionary.find(test) != dictionary.end()) {
                score += 50; // Can be completed with one letter!
            }
        }
    }
        // Two empty positions
    else if (question_marks == 2) {
        // Try all possible combinations
        for (char c1 = 'A'; c1 <= 'Z'; c1++) {
            for (char c2 = 'A'; c2 <= 'Z'; c2++) {
                string test = seq;
                int filled = 0;
                for (size_t i = 0; i < test.length() && filled < 2; i++) {
                    if (test[i] == '?') {
                        test[i] = (filled == 0) ? c1 : c2;
                        filled++;
                    }
                }

                if (dictionary.find(test) != dictionary.end()) {
                    score += 10; // Future potential
                    break; // One match is enough to prove potential
                }
            }
        }
    }

    return score;
}