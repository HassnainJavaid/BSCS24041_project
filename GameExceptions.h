#ifndef GAME_EXCEPTIONS_H
#define GAME_EXCEPTIONS_H

#include <iostream>
#include <exception>
#include "String.h"

using namespace std;

// Base game exception class
class GameException : public exception
{
protected:
    String message;

public:
    GameException(const String& msg) : message(msg) {}

    virtual const char* what() const noexcept override
    {
        return message.c_str();
    }

    String getMessage() const
    {
        return message;
    }
};

// Invalid position exception
class InvalidPositionException : public GameException
{
public:
    InvalidPositionException(int x, int y)
        : GameException(buildMessage(x, y)) {
    }

private:
    static String buildMessage(int x, int y)
    {
        String sx, sy;
        sx.itos(x);
        sy.itos(y);
        return "Invalid position: (" + sx + ", " + sy + ")";
    }
};

// Item not found exception
class ItemNotFoundException : public GameException
{
public:
    ItemNotFoundException(const String& itemName)
        : GameException("Item not found: " + itemName) {
    }

    ItemNotFoundException(int itemId)
        : GameException(buildMessage(itemId)) {
    }

private:
    static String buildMessage(int itemId)
    {
        String idStr;
        idStr.itos(itemId);
        return "Item not found with ID: " + idStr;
    }
};

// Inventory full exception
class InventoryFullException : public GameException
{
public:
    InventoryFullException()
        : GameException("Inventory is full - cannot add more items") {
    }
};

// Combat exception
class CombatException : public GameException
{
public:
    CombatException(const String& msg)
        : GameException("Combat error: " + msg) {
    }
};

// File operation exception
class FileOperationException : public GameException
{
public:
    FileOperationException(const String& operation, const String& filename)
        : GameException("File " + operation + " failed for: " + filename) {
    }
};

// Monster already defeated exception
class MonsterDefeatedException : public GameException
{
public:
    MonsterDefeatedException(const String& monsterName)
        : GameException("Monster already defeated: " + monsterName) {
    }
};

// Player death exception
class PlayerDeathException : public GameException
{
public:
    PlayerDeathException()
        : GameException("Player has died - game over") {
    }
};

// Save/Load exception
class SaveLoadException : public GameException
{
public:
    SaveLoadException(const String& operation)
        : GameException("Save/Load operation failed: " + operation) {
    }
};

// Invalid input exception
class InvalidInputException : public GameException
{
public:
    InvalidInputException(const String& input)
        : GameException("Invalid input received: " + input) {
    }
};

// Game state exception
class GameStateException : public GameException
{
public:
    GameStateException(const String& state)
        : GameException("Invalid game state: " + state) {
    }
};

// Template exception class for type-specific errors
template<typename T>
class TypedGameException : public GameException
{
private:
    T errorData;

public:
    TypedGameException(const String& msg, const T& data)
        : GameException(msg), errorData(data) {
    }

    T getErrorData() const
    {
        return errorData;
    }
};

// Utility function to handle exceptions gracefully
inline void handleGameException(const GameException& e, const String& context = "")
{
    String fullMessage = "Game Exception";
    if (!context.empty())
    {
        fullMessage = fullMessage + " in " + context;
    }
    fullMessage = fullMessage + ": " + e.getMessage();

    cout << "ERROR: " << fullMessage << endl;

#ifdef LOG_ERROR
    LOG_ERROR(fullMessage);
#endif
}

#endif