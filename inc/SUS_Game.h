#ifndef SUS_GAME_H
#define SUS_GAME_H

#include "BoardGame_Classes.h"
#include <vector>
#include <map>

class SUS_Move : public Move<char> {
public:
    SUS_Move(int x, int y, char letter)
        : Move<char>(x, y, letter) {}
};

class SUS_Board : public Board<char> {
private:
    char blank = '.';
    

    char last_player;

public:
	int Total_SUS = 0;
    int score_X = 0;
    int score_O = 0;
    SUS_Board();

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
	string display_message();

    int get_score_X() { return score_X; }
    int get_score_O() { return score_O; }
    int count_total_sus(); 

private:
    void check_and_count_sus(char player_symbol);
};

class SUS_Player : public Player<char> {
private:
    char chosen_letter; 

public:
    SUS_Player(string n, char s, PlayerType t)
        : Player<char>(n, s, t), chosen_letter('S') {}

    Move<char>* get_random_move();
    void set_chosen_letter(char letter) { chosen_letter = letter; }
    char get_chosen_letter() const { return chosen_letter; }

private:
    vector<Move<char>*> get_valid_moves();
};

class SUS_UI : public UI<char> {
public:
    SUS_UI() : UI<char>("SUS Game - Form S-U-S sequences to win!", 2) {
        cout << "\nRules:\n";
        cout << "- Players place either 'S' or 'U' on the board\n";
        cout << "- Each turn, choose which letter to place\n";
        cout << "- Score points by forming 'S-U-S' sequences\n";
        cout << "- Sequences can be horizontal, vertical, or diagonal\n";
        cout << "- Most sequences wins when board is full!\n\n";
    }

    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;
};

#endif