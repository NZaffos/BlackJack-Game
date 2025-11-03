## Blackjack Game

A C++-based interactive Blackjack game developed in Qt Creator, featuring a guided tutorial, practice mode with hints, standard gameplay with virtual currency, and a built-in rules page. The project emphasizes clean UI design, object-oriented programming, and real-time feedback using the Qt framework and Box2D for physics and animation.

---

## Features
- **Tutorial Mode:** Step-by-step onboarding for new players.
- **Practice Mode:** Dynamic hints/strategy suggestions.
- **Standard Mode:** Virtual money, betting logic, win/loss tracking.
- **Rules Page:** Hand values, dealer rules, actions, and outcomes.
- **Polished UX:** Qt-based UI, responsive layouts, Box2D animations.

---

## Tech Stack
- **Language:** C++
- **Framework/IDE:** Qt (Qt Creator)
- **Libraries:** Box2D, Qt Widgets/GUI
- **Design:** OOP with modular classes for deck/shoe, player/dealer, game state, and UI controllers

---

## Build & Run

> **Prereqs:** Qt (with qmake or CMake), a C++17 compiler, and Box2D installed/available to the build.

1. **Clone**: `git clone https://github.com/Nzaffos/BlackJack-Game>.git`
2. **Open** the `.pro` project in **Qt Creator**.
3. **Configure** your kit (Desktop Qt).
4. **Build & Run** Play directly within the Qt application window

## Future Improvements
- Add persistent save/load functionality for player balance
- Integrate leaderboard or online multiplayer support
