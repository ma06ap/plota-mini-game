# Plota Mini-Game

A networked multiplayer board game platform built with C++ and Qt6, allowing two players to play classic board games in real-time over TCP/IP.

## Overview

Plota Mini-Game is a university project that implements a client-server architecture for playing multiplayer board games. Players can create accounts, host game rooms, and compete against each other in three classic games: Checkers, Othello, and Connect-4.

## Features

- **User Authentication**: Login, signup, and password reset functionality
- **Multiple Games**: Play Checkers, Othello/Reversi, or Connect-4
- **Room-Based Multiplayer**: Create or join game rooms with unique IDs
- **Time Controls**: Configurable per-player time limits (default: 180 seconds)
- **Profile Management**: Edit user profiles and track statistics
- **Match History**: View past games and scores
- **Real-Time Gameplay**: Synchronized game state across networked clients

## Technologies

- **Language**: C++17
- **Framework**: Qt 6 (GUI, Networking, Widgets)
- **Networking**: TCP/IP sockets (QTcpServer/QTcpSocket)
- **Build System**: qmake
- **Authentication**: CSV-based user database with password hashing

## Project Structure

```
plota-mini-game/
├── Backend/          # Shared game logic library
│   ├── Games/        # Game implementations (Checkers, Othello, Connect-4)
│   ├── Game.h        # Abstract base game class
│   └── Board.h       # Abstract board class
├── Server/           # Game server application
│   ├── GameServer    # TCP server handling game sessions
│   ├── AuthHandler   # User authentication and profile management
│   └── ServerWindow  # Server monitoring GUI
└── Client/           # Player client application
    └── Widget        # Client GUI (login, menu, game board)
```

## Prerequisites

- Qt 6.x or later
- C++17 compatible compiler
- qmake build tool

## Setup and Installation

### Building the Server

```bash
cd Server
qmake Server.pro
make
```

### Building the Client

```bash
cd Client
qmake Client.pro
make
```

## How to Run

### 1. Start the Server

Run the server application first:

```bash
cd Server
./Server
```

The server will start listening for client connections on the configured port.

### 2. Start the Client(s)

Run one or more client instances:

```bash
cd Client
./Client
```

### 3. Connect and Play

1. **Create an Account**: Sign up with a username and password
2. **Login**: Enter your credentials to access the main menu
3. **Choose a Game**: Select Checkers, Othello, or Connect-4
4. **Create or Join Room**: 
   - Create a new game room with a time limit
   - Or join an existing room by entering its ID
5. **Play**: Make moves by clicking on the board

## Games Available

### Checkers
- Classic checkers rules with king promotions
- Mandatory captures
- Strategic board game for two players

### Othello (Reversi)
- Flip opponent pieces by sandwiching them
- Control the most pieces to win
- 8x8 board gameplay

### Connect-4
- Drop pieces into a vertical grid
- Connect four in a row to win
- Diagonal, horizontal, or vertical

## University Project

This project was developed as part of a university coursework to demonstrate:
- Network programming with TCP/IP
- Object-oriented design patterns
- GUI development with Qt
- Client-server architecture
- Real-time multiplayer game synchronization

## License

This is an academic project. Please refer to your university's policies regarding code sharing and usage.
