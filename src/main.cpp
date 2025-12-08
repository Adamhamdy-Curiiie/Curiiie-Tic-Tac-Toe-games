/**
 * @file main.cpp
 * @brief Main menu for Board Games Collection
 * @author AdamHamdy-Muhammed Amr - Seif Eid - Ahmed Emam
 * @date December 2025
 */

#include "BoardGame_Classes.h"
#include "DiamondTicTacToe.h"
#include "NumericalTicTacToe.h"
#include "Game7.h"
#include "InfinityTicTacToe.h"
#include "Game10.h"
#include "Memory_Classes.h"
#include "Game_3.h"            
#include "WordTicTacToe.h"       
#include "UltimateTicTacToe.h"
#include "Misere_Classes.h"
#include "Pyramid_Classes.h"
#include "SUS_Game.h"
#include "Connect_4.h"

#include <iostream>

using namespace std;

/**
 * @brief Display the game menu
 */
void display_menu() {
    cout << "\n===== BOARD GAMES MENU =====\n";
    cout << "1.  SUS \n";
    cout << "2.  Four-in-a-row\n";
    cout << "3.  5 x 5 Tic Tac Toe\n";
    cout << "4.  Word Tic-tac-toe\n";
    cout << "5.  Misere Tic Tac Toe\n";
    cout << "6.  Diamond Tic-Tac-Toe\n";
    cout << "7.  4 x 4 Tic-Tac-Toe \n";
    cout << "8.  Pyramid Tic-Tac-Toe\n";
    cout << "9.  Numerical Tic-Tac-Toe\n";
    cout << "10. Obstacles Tic-Tac-Toe\n";
    cout << "11. Infinity Tic-Tac-Toe\n";
    cout << "12. Ultimate Tic Tac Toe\n";
    cout << "13. Memory Tic-Tac-Toe\n";
    cout << "0.  Exit\n";
    cout << "============================\n";
    cout << "Enter your choice: ";
}

/**
 * @brief Main game loop - handles menu selection and game execution
 */
void switch_case() {
    int choice;
    
    while (true) {
        display_menu();
        cin >> choice;

        if (choice == 0) {
            cout << "Thank you for playing!\n";
            break;
        }

        // Special case: Numerical game uses Board<int>
        if (choice == 9) {
            auto board = new NumericalBoard();
            auto ui = new NumericalUI();
            Player<int>** players = ui->setup_players();

            GameManager<int> game(board, players, ui);
            game.run();

            delete board;
            delete players[0];
            delete players[1];
            delete[] players;
            delete ui;
            continue;
        }
        
        UI<char>* ui = nullptr;
        Board<char>* board = nullptr;

        switch (choice) {
        case 1:
            ui = new SUS_UI();
            board = new SUS_Board();
            break;
        case 2:
            ui = new Connect_4_UI();
            board = new Connect_4();
            break;
        case 3:
            ui = new FiveByFiveUI();
            board = new FiveByFiveBoard();
            break;
        case 4:
            ui = new WordTicTacToe_UI();
            board = new WordTicTacToe_Board();
            break;
        case 5:
            ui = new Misere_UI();
            board = new Misere_TTT_Board();
            break;
        case 6:
            ui = new DiamondUI();
            board = new DiamondBoard();
            break;
        case 7:
            ui = new Game7_UI();
            board = new Game7_Board();
            break;
        case 8:
            ui = new Pyramid_UI();
            board = new Pyramid_Board();
            break;
        case 10:
            ui = new Game10_UI();
            board = new Game10_Board();
            break;
        case 11:
            ui = new InfinityUI();
            board = new InfinityBoard();
            break;
        case 12:
            ui = new Ultimate_TTT_UI<char>();
            board = new Ultimate_TTT_Board<char>();
            break;
        case 13:
            ui = new Memory_UI();
            board = new Memory_Board();
            break;
        default:
            cout << "\nInvalid choice! Try again.\n";
            continue;
        }

        if (ui != nullptr && board != nullptr) {
            Player<char>** players = ui->setup_players();
            GameManager<char> game(board, players, ui);
            game.run();

            delete board;
            delete players[0];
            delete players[1];
            delete[] players;
            delete ui;
        }
    }
}

/**
 * @brief Program entry point
 * @return 0 on successful exit
 */
int main() {
    switch_case();
    return 0;
}