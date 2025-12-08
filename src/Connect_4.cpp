#include <iostream>
#include <iomanip>
#include <cctype>
#include "Connect_4.h"

using namespace std;

Connect_4::Connect_4() : Board(6, 7) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
    a = -1;
    b = -1;
}

bool Connect_4::update_board(Move<char>* move) {
    int y = move->get_y();
    char mark = move->get_symbol();
    
    if (y < 0 || y >= columns) return false;
    
    for (int x = rows - 1; x >= 0; x--) {
        if (board[x][y] == blank_symbol) {
            board[x][y] = toupper(mark);
            a = x;
            b = y;
            n_moves++;
            return true;
        }
    }
    return false;
}

bool Connect_4::is_win(Player<char>* player) {
    const char sym = player->get_symbol();
    
    if (a < 0 || b < 0) return false;
    
    auto check_four = [&](int dx, int dy) {
        int count = 1;
        
        for (int i = 1; i < 4; i++) {
            int nx = a + i * dx;
            int ny = b + i * dy;
            if (nx >= 0 && nx < rows && ny >= 0 && ny < columns && board[nx][ny] == sym)
                count++;
            else
                break;
        }
        
        for (int i = 1; i < 4; i++) {
            int nx = a - i * dx;
            int ny = b - i * dy;
            if (nx >= 0 && nx < rows && ny >= 0 && ny < columns && board[nx][ny] == sym)
                count++;
            else
                break;
        }
        
        return count >= 4;
    };
    
    return check_four(0, 1) || check_four(1, 0) || check_four(1, 1) || check_four(1, -1);
}

bool Connect_4::is_draw(Player<char>* player) {
    return (n_moves == 42 && !is_win(player));
}

bool Connect_4::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

Connect_4_UI::Connect_4_UI() : UI<char>("Welcome to FCAI Connect 4 Game", 2) {
    cout << "\n========== GAME RULES ==========\n";
    cout << "- Board: 6 rows x 7 columns grid\n";
    cout << "- Players take turns choosing a column (0-6)\n";
    cout << "- Your token drops to the lowest available position in that column\n";
    cout << "- Win: Get 4 of your marks in a row (horizontal, vertical, or diagonal)\n";
    cout << "- Draw: Board is full with no winner\n";
    cout << "================================\n\n";
}

Player<char>* Connect_4_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
         << " player: " << name << " (" << symbol << ")\n";
    
    return new Player<char>(name, symbol, type);
}

Move<char>* Connect_4_UI::get_move(Player<char>* player) {
    int y;
    Board<char>* boardPtr = player->get_board_ptr();
    
    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\n" << player->get_name() << " (" << player->get_symbol() << ")'s turn\n";
        cout << "Enter column (0 to 6): ";
        cin >> y;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        y = rand() % boardPtr->get_columns();
        cout << player->get_name() << " chooses column " << y << "\n";
    }
    
    int landing_row = -1;
    auto matrix = boardPtr->get_board_matrix();
    for (int x = boardPtr->get_rows() - 1; x >= 0; x--) {
        if (matrix[x][y] == '.') {
            landing_row = x;
            break;
        }
    }
    
    if (landing_row != -1) {
        cout << player->get_name() << " placed " << player->get_symbol() 
             << " at position (" << landing_row << ", " << y << ")\n";
    }
    
    return new Move<char>(0, y, player->get_symbol());
}