# 🎮 Curiiie Tic-Tac-Toe Games

A collection of **13 unique Tic-Tac-Toe variants** implemented in C++ using Object-Oriented Programming principles. Built on a shared framework (`BoardGame_Classes.h`) for reusable board logic, player handling, and game control.

---

## 🎯 Game Collection

### 1️ SUS Game
**3×3 grid** • Players place `S` or `U` to form `S-U-S` sequences • Most sequences wins

### 2️ Four-in-a-Row
**6×7 grid** • Connect Four style • Marks drop to lowest available cell • First to align 4 wins

### 3️ 5×5 Tic-Tac-Toe
**5×5 grid** • Play 24 moves (one cell left empty) • Most three-in-a-row sequences wins

### 4️ Word Tic-Tac-Toe
**3×3 grid** • Place letters instead of X/O • Form valid 3-letter words • Uses `dic.txt` dictionary

### ️5️⃣ Misère Tic-Tac-Toe
**3×3 grid** • Inverse rules • **Avoid** three-in-a-row to win

### 6️ Diamond Tic-Tac-Toe ♦️
**7×7 diamond shape** • Win by completing **both** a 3-line AND 4-line in different directions • Lines may share ≤1 cell • **AI-powered with Minimax**

### 7️ 4×4 Tic-Tac-Toe (Moving Tokens) 🔄
**4×4 grid** • Move tokens to adjacent cells • Starts: `O X O X` / `X O X O` • First to align 3 wins • **AI-powered with Minimax**

### 8️ Pyramid Tic-Tac-Toe 🔺
**Pyramid structure** • Non-rectangular board • Align 3 marks to win

### 9️ Numerical Tic-Tac-Toe 🔢
**3×3 grid** • Player 1: odd numbers (1,3,5,7,9) • Player 2: even numbers (2,4,6,8) • First line summing to **15** wins

### 🔟 Obstacles Tic-Tac-Toe 🚧
**6×6 grid** • Two random obstacles added each round • Align 4 marks to win

### 1️⃣1️⃣1 Infinity Tic-Tac-Toe ♾️
**3×3 grid** • Oldest mark vanishes every 3 moves • Win before your marks disappear

### 1️2️⃣ Ultimate Tic-Tac-Toe 🏆
**9 sub-boards (3×3 each)** • Meta-game: win sub-boards to claim main board cells • Align 3 sub-board wins

### 1️3️ Memory Tic-Tac-Toe 🧠
**3×3 grid** • Marks hidden after placement • Remember positions to align 3

---

## 🛠️ Technical Details

| Feature | Technology                                            |
|---------|-------------------------------------------------------|
| **Language** | C++                                                   |
| **Paradigm** | Object-Oriented Programming                           |
| **Framework** | `BoardGame_Classes.h`                                 |
| **AI** | Minimax with Alpha-Beta Pruning (Games 3 & 4 & 6 & 7) |
| **Documentation** | Doxygen                                               |

---

## 📚 Features

- ✅ **13 unique game variants**
- ✅ **Modular OOP design** with inheritance
- ✅ **Smart AI opponents** (Minimax algorithm)
- ✅ **Single & multiplayer modes**
- ✅ **Professional documentation** with Doxygen