#include "../include/Game.h"

Game::Game() = default;

void Game::initGame() {
    gameBoard.reset();
    score = 0;
    moveCount = 0;
    historyGameBoards = std::stack<GameBoard>();
    histroyScores = std::stack<int>();
    startTime = std::chrono::system_clock::now();
    historyGameBoards.push(gameBoard);  // Save initial state
    histroyScores.push(score);           // Save initial score
}

void Game::updateGame(Direction direction) {
    // Save current state to history
    historyGameBoards.push(gameBoard);
    histroyScores.push(score);

    // Get move score and update score
    int moveScore = gameBoard.move(direction);
    score += moveScore;

    // update move count
    ++moveCount;

}

void Game::undoLastMove() {
    if (historyGameBoards.size() > 1) {
        // Remove current state
        historyGameBoards.pop();
        histroyScores.pop();

        // Restore previous state
        gameBoard = historyGameBoards.top();
        score = histroyScores.top();
        --moveCount; // Decrement move count

    } else {
        gameBoard.reset();
    }
}

bool Game::hasWon() const {
    return gameBoard.hasWinningTile();
}

bool Game::hasLost() const {
    return gameBoard.isGameOver();
}

double Game::getElapsedTime() const {
    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsedSeconds = endTime - startTime;
    return elapsedSeconds.count();
}

int Game::getScore() const {
    return score;
}

int Game::getMoveCount() const {
    return moveCount;
}

const GameBoard& Game::getGameBoard() const {
    return gameBoard;
}