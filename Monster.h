#ifndef MONSTER_H
#define MONSTER_H

#include "Entity.h"
#include "String.h"
#include <vector>

enum class MonsterType {
    GOBLIN,
    ORC,
    DRAGON,
    SKELETON,
    TROLL
};

class Monster : public Entity {
private:
    MonsterType type;
    int health;
    int maxHealth;
    int attack;
    int defense;
    String weakness; // Item required to defeat easily
    bool isDefeated;
    vector<int> requiredItems; // IDs of items needed to defeat

public:
    Monster() : Entity(), type(MonsterType::GOBLIN), health(100), maxHealth(100), 
                attack(10), defense(5), isDefeated(false) {
        weakness = "None";
    }
    
    Monster(const String& monsterName, int posX, int posY, int monsterId, MonsterType monsterType, 
            int hp, int att, int def, const String& weak = "None")
        : Entity(monsterName, posX, posY, monsterId), type(monsterType), health(hp), 
          maxHealth(hp), attack(att), defense(def), weakness(weak), isDefeated(false) {}
    
    virtual ~Monster() {}
    
    // Override virtual functions
    virtual void display() const override {
        cout << "Monster: " << name << " (HP: " << health << "/" << maxHealth 
             << ", ATK: " << attack << ", DEF: " << defense << ")" << endl;
        if (weakness != "None") {
            cout << "Weakness: " << weakness << endl;
        }
    }
    
    virtual bool interact() override {
        if (isDefeated) {
            cout << name << " has already been defeated." << endl;
            return false;
        }
        cout << "You encounter " << name << "! Prepare for battle!" << endl;
        return true;
    }
    
    // Combat methods
    void takeDamage(int damage) {
        int actualDamage = damage - defense;
        if (actualDamage < 0) actualDamage = 0;
        health -= actualDamage;
        if (health <= 0) {
            health = 0;
            isDefeated = true;
            cout << name << " has been defeated!" << endl;
        }
    }
    
    int dealDamage() const {
        return attack;
    }
    
    bool checkWeakness(const String& itemName) const {
        return weakness == itemName;
    }
    
    // Getters
    MonsterType getType() const { return type; }
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    int getAttack() const { return attack; }
    int getDefense() const { return defense; }
    String getWeakness() const { return weakness; }
    bool getDefeated() const { return isDefeated; }
    const vector<int>& getRequiredItems() const { return requiredItems; }
    
    // Setters
    void setHealth(int hp) { health = hp; }
    void setDefeated(bool defeated) { isDefeated = defeated; }
    void addRequiredItem(int itemId) { requiredItems.push_back(itemId); }
    
    // Heal monster
    void heal(int amount) {
        health += amount;
        if (health > maxHealth) health = maxHealth;
    }
    
    // Serialization
    virtual void serialize(ofstream& file) const override {
        Entity::serialize(file);
        file.write(reinterpret_cast<const char*>(&type), sizeof(type));
        file.write(reinterpret_cast<const char*>(&health), sizeof(health));
        file.write(reinterpret_cast<const char*>(&maxHealth), sizeof(maxHealth));
        file.write(reinterpret_cast<const char*>(&attack), sizeof(attack));
        file.write(reinterpret_cast<const char*>(&defense), sizeof(defense));
        file.write(reinterpret_cast<const char*>(&isDefeated), sizeof(isDefeated));
        
        int weaknessLen = weakness.size();
        file.write(reinterpret_cast<const char*>(&weaknessLen), sizeof(weaknessLen));
        for(int i = 0; i < weaknessLen; i++) {
            file.write(&weakness[i], sizeof(char));
        }
        
        int reqItemsSize = requiredItems.size();
        file.write(reinterpret_cast<const char*>(&reqItemsSize), sizeof(reqItemsSize));
        for(int item : requiredItems) {
            file.write(reinterpret_cast<const char*>(&item), sizeof(item));
        }
    }
    
    virtual void deserialize(ifstream& file) override {
        Entity::deserialize(file);
        file.read(reinterpret_cast<char*>(&type), sizeof(type));
        file.read(reinterpret_cast<char*>(&health), sizeof(health));
        file.read(reinterpret_cast<char*>(&maxHealth), sizeof(maxHealth));
        file.read(reinterpret_cast<char*>(&attack), sizeof(attack));
        file.read(reinterpret_cast<char*>(&defense), sizeof(defense));
        file.read(reinterpret_cast<char*>(&isDefeated), sizeof(isDefeated));
        
        int weaknessLen;
        file.read(reinterpret_cast<char*>(&weaknessLen), sizeof(weaknessLen));
        weakness.clear();
        for(int i = 0; i < weaknessLen; i++) {
            char ch;
            file.read(&ch, sizeof(char));
            weakness.push_back(ch);
        }
        
        int reqItemsSize;
        file.read(reinterpret_cast<char*>(&reqItemsSize), sizeof(reqItemsSize));
        requiredItems.clear();
        for(int i = 0; i < reqItemsSize; i++) {
            int item;
            file.read(reinterpret_cast<char*>(&item), sizeof(item));
            requiredItems.push_back(item);
        }
    }
};

#endif