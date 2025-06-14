#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "Dungeon.h"
#include "Character.h"
#include "Logger.h"
#include "GameExceptions.h"
#include "String.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

// Initialize global logger
Logger<String>* gameLogger = nullptr;

class GameEngine {
private:
    Dungeon* dungeon;
    Character* player;
    bool gameRunning;
    bool gameWon;
    String saveFileName;
    
    // Template function for combat calculations
    template<typename T1, typename T2>
    int calculateDamage(const T1& attacker, const T2& defender) {
        int damage = attacker->getAttack() - defender->getDefense();
        return (damage > 0) ? damage : 1; // Minimum 1 damage
    }
    
    // Helper function to convert std::string to String
    String stdStringToString(const string& str) {
        return String(str.c_str());
    }
    
    // Helper function to convert String to std::string
    string stringToStdString(const String& str) {
        string result;
        for(int i = 0; i < str.size(); i++) {
            result += str[i];
        }
        return result;
    }
    
public:
    GameEngine() : dungeon(nullptr), player(nullptr), gameRunning(true), gameWon(false) {
        saveFileName = String("savegame.dat");
        
        // Initialize logger
        if(!gameLogger) {
            gameLogger = new Logger<String>("game.log", LogLevel::INFO);
        }
        
        initializeGame();
    }
    
    ~GameEngine() {
        cleanup();
        
        if(gameLogger) {
            delete gameLogger;
            gameLogger = nullptr;
        }
    }
    
    void initializeGame() {
        try {
            String logMsg("Initializing new game");
            LOG_INFO(logMsg);
            
            // Create dungeon
            dungeon = new Dungeon(10, 10, "The Cursed Dungeon");
            
            // Create player
            player = new Character("Hero", 0, 0, 1);
            dungeon->setPlayer(player);
            
            String successMsg("Game initialized successfully");
            LOG_INFO(successMsg);
            
        } catch(const exception& e) {
            String errorMsg("Failed to initialize game");
            LOG_ERROR(errorMsg);
            throw GameStateException("Game initialization failed");
        }
    }
    
    void cleanup() {
        if(dungeon) {
            delete dungeon;
            dungeon = nullptr;
        }
        if(player) {
            delete player;
            player = nullptr;
        }
    }
    
    void displayWelcome() {
        cout << "========================================" << endl;
        cout << "    WELCOME TO THE DUNGEON CRAWLER!" << endl;
        cout << "========================================" << endl;
        cout << "You are a brave adventurer who has entered" << endl;
        cout << "a mysterious dungeon filled with monsters," << endl;
        cout << "treasures, and dangers. Your goal is to" << endl;
        cout << "reach the exit at the far corner while" << endl;
        cout << "collecting items and defeating monsters." << endl;
        cout << "========================================" << endl;
        cout << "Commands:" << endl;
        cout << "  move <direction> - Move N/S/E/W" << endl;
        cout << "  look - Examine current location" << endl;
        cout << "  inventory - Check your items" << endl;
        cout << "  use <item> - Use an item" << endl;
        cout << "  pickup <item> - Pick up an item" << endl;
        cout << "  attack - Attack a monster" << endl;
        cout << "  map - Display the dungeon map" << endl;
        cout << "  status - Show character status" << endl;
        cout << "  save - Save the game" << endl;
        cout << "  load - Load a saved game" << endl;
        cout << "  help - Show this help" << endl;
        cout << "  quit - Exit the game" << endl;
        cout << "========================================" << endl;
    }
    
    void run() {
        displayWelcome();
        dungeon->displayCurrentLocation();
        
        while(gameRunning) {
            try {
                cout << "\n> ";
                string input;
                getline(cin, input);
                
                if(input.empty()) continue;
                
                processCommand(stdStringToString(input));
                
                // Check win condition
                if(dungeon->isWinCondition()) {
                    gameWon = true;
                    gameRunning = false;
                    cout << "\n*** CONGRATULATIONS! ***" << endl;
                    cout << "You have reached the exit and won the game!" << endl;
                    String winMsg("Player won the game!");
                    LOG_INFO(winMsg);
                }
                
                // Check if player is dead
                if(!player->isAlive()) {
                    gameRunning = false;
                    cout << "\n*** GAME OVER ***" << endl;
                    cout << "You have died. Better luck next time!" << endl;
                    String deathMsg("Player died - game over");
                    LOG_INFO(deathMsg);
                }
                
            } catch(const GameException& e) {
                handleGameException(e, "main game loop");
            } catch(const exception& e) {
                cout << "Unexpected error: " << e.what() << endl;
                String errorMsg("Unexpected error in main loop");
                LOG_ERROR(errorMsg);
            }
        }
    }
    
    void processCommand(const String& input) {
        // Use String's tokenize method to parse command
        int tokenCount;
        String* tokens = input.tokenize(" \t", tokenCount);
        
        if(tokenCount == 0) {
            delete[] tokens;
            return;
        }
        
        String command = tokens[0];
        command.tolower(); // Convert to lowercase using String method
        
        if(command == String("move")) {
            if(tokenCount > 1) {
                String direction = tokens[1];
                handleMove(direction[0]);
            } else {
                cout << "Move where? (N/S/E/W)" << endl;
            }
        }
        else if(command == String("look")) {
            handleLook();
        }
        else if(command == String("inventory")) {
            handleInventory();
        }
        else if(command == String("use")) {
            if(tokenCount > 1) {
                // Join remaining tokens as item name
                String* itemTokens = new String[tokenCount - 1];
                for(int i = 1; i < tokenCount; i++) {
                    itemTokens[i - 1] = tokens[i];
                }
                String itemName = String::join(" ", itemTokens, tokenCount - 1);
                handleUseItem(itemName);
                delete[] itemTokens;
            } else {
                cout << "Use what item?" << endl;
            }
        }
        else if(command == String("pickup")) {
            if(tokenCount > 1) {
                // Join remaining tokens as item name
                String* itemTokens = new String[tokenCount - 1];
                for(int i = 1; i < tokenCount; i++) {
                    itemTokens[i - 1] = tokens[i];
                }
                String itemName = String::join(" ", itemTokens, tokenCount - 1);
                handlePickup(itemName);
                delete[] itemTokens;
            } else {
                cout << "Pick up what?" << endl;
            }
        }
        else if(command == String("attack")) {
            handleAttack();
        }
        else if(command == String("map")) {
            handleMap();
        }
        else if(command == String("status")) {
            handleStatus();
        }
        else if(command == String("save")) {
            handleSave();
        }
        else if(command == String("load")) {
            handleLoad();
        }
        else if(command == String("help")) {
            displayWelcome();
        }
        else if(command == String("quit") || command == String("exit")) {
            gameRunning = false;
            cout << "Thanks for playing!" << endl;
        }
        else {
            cout << "Unknown command. Type 'help' for available commands." << endl;
        }
        
        delete[] tokens;
    }
    
    void handleMove(char direction) {
        try {
            if(dungeon->movePlayer(direction)) {
                String moveMsg("Player moved successfully");
                LOG_INFO(moveMsg);
                // Check for monsters at new location
                Location* currentLoc = dungeon->getLocation(player->getX(), player->getY());
                if(currentLoc && currentLoc->hasMonsters()) {
                    Monster* monster = currentLoc->getAliveMonster();
                    if(monster) {
                        cout << "A wild " << monster->getName() << " blocks your path!" << endl;
                    }
                }
            }
        } catch(const InvalidPositionException& e) {
            cout << e.what() << endl;
        }
    }
    
    void handleLook() {
        dungeon->displayCurrentLocation();
    }
    
    void handleInventory() {
        player->display();
    }
    
    void handleUseItem(const String& itemName) {
        try {
            if(player->useItem(itemName)) {
                String logMsg = String("Player used item: ") + itemName;
                LOG_INFO(logMsg);
            }
        } catch(const ItemNotFoundException& e) {
            cout << e.what() << endl;
        }
    }
    
    void handlePickup(const String& itemName) {
        try {
            Location* currentLoc = dungeon->getLocation(player->getX(), player->getY());
            if(!currentLoc) {
                throw InvalidPositionException(player->getX(), player->getY());
            }
            
            // Find item by name
            const vector<Item*>& items = currentLoc->getItems();
            Item* targetItem = nullptr;
            
            for(Item* item : items) {
                if(item->getName() == itemName && item->getActive()) {
                    targetItem = item;
                    break;
                }
            }
            
            if(!targetItem) {
                throw ItemNotFoundException(itemName);
            }
            
            // Create a copy for player inventory
            Item* itemCopy = new Item(targetItem->getName(), player->getX(), player->getY(), 
                                    targetItem->getId(), targetItem->getType(), 
                                    targetItem->getValue(), targetItem->getConsumable());
            
            player->addItem(itemCopy);
            currentLoc->removeItem(targetItem->getId());
            
            String logMsg = String("Player picked up: ") + targetItem->getName();
            LOG_INFO(logMsg);
            
        } catch(const GameException& e) {
            cout << e.what() << endl;
        }
    }
    
    void handleAttack() {
        try {
            Location* currentLoc = dungeon->getLocation(player->getX(), player->getY());
            if(!currentLoc) {
                throw InvalidPositionException(player->getX(), player->getY());
            }
            
            Monster* monster = currentLoc->getAliveMonster();
            if(!monster) {
                throw CombatException("No monsters to fight here");
            }
            
            if(monster->getDefeated()) {
                throw MonsterDefeatedException(monster->getName());
            }
            
            // Combat loop
            cout << "Combat begins with " << monster->getName() << "!" << endl;
            
            while(player->isAlive() && !monster->getDefeated()) {
                // Player attacks
                int playerDamage = calculateDamage(player, monster);
                monster->takeDamage(playerDamage);
                String attackMsg = String("Player attacks ") + monster->getName() + String(" for damage");
                LOG_INFO(attackMsg);
                
                if(monster->getDefeated()) {
                    cout << "You defeated " << monster->getName() << "!" << endl;
                    player->gainExperience(50 + monster->getAttack());
                    break;
                }
                
                // Monster attacks back
                int monsterDamage = calculateDamage(monster, player);
                player->takeDamage(monsterDamage);
                String counterMsg = monster->getName() + String(" attacks player for damage");
                LOG_INFO(counterMsg);
                
                if(!player->isAlive()) {
                    throw PlayerDeathException();
                }
                
                // Show current status
                cout << "Your health: " << player->getHealth() << "/" << player->getMaxHealth() << endl;
                cout << monster->getName() << " health: " << monster->getHealth() << "/" << monster->getMaxHealth() << endl;
                
                // Ask if player wants to continue or flee
                cout << "Continue fighting? (y/n): ";
                string choice;
                getline(cin, choice);
                if(choice == "n" || choice == "N") {
                    cout << "You flee from combat!" << endl;
                    break;
                }
            }
            
        } catch(const GameException& e) {
            cout << e.what() << endl;
        }
    }
    
    void handleMap() {
        dungeon->displayMap();
    }
    
    void handleStatus() {
        player->display();
        cout << "\nCurrent Location: (" << player->getX() << ", " << player->getY() << ")" << endl;
    }
    
    void handleSave() {
        try {
            string filename = stringToStdString(saveFileName);
            ofstream saveFile(filename.c_str(), ios::binary);
            if(!saveFile.is_open()) {
                throw FileOperationException("save", saveFileName);
            }
            
            // Save game state
            saveFile.write(reinterpret_cast<const char*>(&gameWon), sizeof(gameWon));
            
            // Save player
            player->serialize(saveFile);
            
            // Save dungeon
            dungeon->serialize(saveFile);
            
            saveFile.close();
            cout << "Game saved successfully!" << endl;
            String logMsg = String("Game saved to ") + saveFileName;
            LOG_INFO(logMsg);
            
        } catch(const GameException& e) {
            cout << e.what() << endl;
        }
    }
    
    void handleLoad() {
        try {
            string filename = stringToStdString(saveFileName);
            ifstream loadFile(filename.c_str(), ios::binary);
            if(!loadFile.is_open()) {
                throw FileOperationException("load", saveFileName);
            }
            
            // Clean up current game
            cleanup();
            
            // Load game state
            loadFile.read(reinterpret_cast<char*>(&gameWon), sizeof(gameWon));
            
            // Load player
            player = new Character();
            player->deserialize(loadFile);
            
            // Load dungeon
            dungeon = new Dungeon();
            dungeon->deserialize(loadFile);
            dungeon->setPlayer(player);
            
            loadFile.close();
            cout << "Game loaded successfully!" << endl;
            String logMsg = String("Game loaded from ") + saveFileName;
            LOG_INFO(logMsg);
            
            // Display current location after loading
            dungeon->displayCurrentLocation();
            
        } catch(const GameException& e) {
            cout << e.what() << endl;
            // Reinitialize game if load fails
            initializeGame();
        }
    }
    
    bool isRunning() const {
        return gameRunning;
    }
    
    bool hasWon() const {
        return gameWon;
    }
};

#endif