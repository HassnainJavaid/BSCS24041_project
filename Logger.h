#ifndef LOGGER_H
#define LOGGER_H

#include "String.h"
#include <fstream>
#include <iostream>
using namespace std;

enum class LogLevel {
    INFO,
    WARNING,
    ERROR,
    DEBUG
};

template<typename T>
class Logger {
private:
    String logFileName;
    LogLevel currentLevel;
    ofstream logFile;
    
    String getCurrentTime() const {
        // Simple timestamp - in real implementation you'd use proper time functions
        static int timeCounter = 0;
        String time;
        time.itos(++timeCounter);
        return String("[") + time + String("] ");
    }
    
    String levelToString(LogLevel level) const {
        switch(level) {
            case LogLevel::INFO: return "INFO";
            case LogLevel::WARNING: return "WARN";
            case LogLevel::ERROR: return "ERROR";
            case LogLevel::DEBUG: return "DEBUG";
            default: return "UNKNOWN";
        }
    }
    
public:
    Logger(const String& fileName = "game.log", LogLevel level = LogLevel::INFO) 
        : logFileName(fileName), currentLevel(level) {
        logFile.open(fileName.c_str(), ios::app);
        if(!logFile.is_open()) {
            cerr << "Warning: Could not open log file " << fileName << endl;
        }
        log(LogLevel::INFO, "Logger initialized");
    }
    
    ~Logger() {
        log(LogLevel::INFO, "Logger shutting down");
        if(logFile.is_open()) {
            logFile.close();
        }
    }
    
    void log(LogLevel level, const String& message) {
        if(level < currentLevel) return;
        
        String logEntry = getCurrentTime() + levelToString(level) + ": " + message;
        
        // Write to console
        cout << logEntry << endl;
        
        // Write to file
        if(logFile.is_open()) {
            logFile << logEntry << endl;
            logFile.flush();
        }
    }
    
    void log(LogLevel level, const String& message, const T& data) {
        String fullMessage = message + " - Data: ";
        // This is a simplified approach - in real implementation you'd use proper formatting
        log(level, fullMessage);
    }
    
    void info(const String& message) {
        log(LogLevel::INFO, message);
    }
    
    void warning(const String& message) {
        log(LogLevel::WARNING, message);
    }
    
    void error(const String& message) {
        log(LogLevel::ERROR, message);
    }
    
    void debug(const String& message) {
        log(LogLevel::DEBUG, message);
    }
    
    void setLevel(LogLevel level) {
        currentLevel = level;
        log(LogLevel::INFO, "Log level changed");
    }
    
    void logPlayerAction(const String& action, const Character* player) {
        if(player) {
            String message = "Player '" + player->getName() + "' performed action: " + action;
            log(LogLevel::INFO, message);
        }
    }
    
    void logCombat(const String& attacker, const String& defender, int damage) {
        String message = attacker + " attacks " + defender + " for ";
        String dmgStr;
        dmgStr.itos(damage);
        message = message + dmgStr + " damage";
        log(LogLevel::INFO, message);
    }
    
    void logItemUsage(const String& playerName, const String& itemName) {
        String message = playerName + " used item: " + itemName;
        log(LogLevel::INFO, message);
    }
    
    void logGameEvent(const String& event) {
        log(LogLevel::INFO, "GAME EVENT: " + event);
    }
};

// Global logger instance
extern Logger<String>* gameLogger;

// Helper macros for easier logging
#define LOG_INFO(msg) if(gameLogger) gameLogger->info(msg)
#define LOG_WARNING(msg) if(gameLogger) gameLogger->warning(msg)
#define LOG_ERROR(msg) if(gameLogger) gameLogger->error(msg)
#define LOG_DEBUG(msg) if(gameLogger) gameLogger->debug(msg)

#endif