#ifndef CHARACTER_H
#define CHARACTER_H

#include "Entity.h"
#include "Item.h"
#include "String.h"
#include <vector>

class Character : public Entity {
private:
    int health;
    int maxHealth;
    int mana;
    int maxMana;
    int attack;
    int defense;
    int level;
    int experience;
    vector<Item*> inventory; // Composition - Character has Items

public:
    Character() : Entity(), health(100), maxHealth(100), mana(50), maxMana(50),
                  attack(15), defense(10), level(1), experience(0) {
        name = "Hero";
    }
    
    Character(const String& characterName, int posX, int posY, int characterId,
              int hp = 100, int mp = 50, int att = 15, int def = 10)
        : Entity(characterName, posX, posY, characterId), health(hp), maxHealth(hp),
          mana(mp), maxMana(mp), attack(att), defense(def), level(1), experience(0) {}
    
    virtual ~Character() {
        // Clean up inventory
        for(Item* item : inventory) {
            delete item;
        }
        inventory.clear();
    }
    
    // Override virtual functions
    virtual void display() const override {
        cout << "Character: " << name << " (Level " << level << ")" << endl;
        cout << "Health: " << health << "/" << maxHealth << endl;
        cout << "Mana: " << mana << "/" << maxMana << endl;
        cout << "Attack: " << attack << ", Defense: " << defense << endl;
        cout << "Experience: " << experience << endl;
        cout << "Inventory (" << inventory.size() << " items):" << endl;
        for(const Item* item : inventory) {
            cout << "  - " << item->getName() << endl;
        }
    }
    
    virtual bool interact() override {
        cout << "You examine yourself." << endl;
        display();
        return true;
    }
    
    // Combat methods
    void takeDamage(int damage) {
        int actualDamage = damage - defense;
        if (actualDamage < 0) actualDamage = 0;
        health -= actualDamage;
        if (health < 0) health = 0;
        cout << name << " takes " << actualDamage << " damage! (Health: " << health << "/" << maxHealth << ")" << endl;
    }
    
    int dealDamage() const {
        return attack;
    }
    
    bool isAlive() const {
        return health > 0;
    }
    
    // Item management
    void addItem(Item* item) {
        inventory.push_back(item);
        cout << "Added " << item->getName() << " to inventory." << endl;
    }
    
    bool removeItem(int itemId) {
        for(auto it = inventory.begin(); it != inventory.end(); ++it) {
            if((*it)->getId() == itemId) {
                delete *it;
                inventory.erase(it);
                return true;
            }
        }
        return false;
    }
    
    Item* findItem(const String& itemName) {
        for(Item* item : inventory) {
            if(item->getName() == itemName) {
                return item;
            }
        }
        return nullptr;
    }
    
    bool hasItem(int itemId) const {
        for(const Item* item : inventory) {
            if(item->getId() == itemId) {
                return true;
            }
        }
        return false;
    }
    
    // Use item
    bool useItem(const String& itemName) {
        Item* item = findItem(itemName);
        if(!item) {
            cout << "Item not found in inventory." << endl;
            return false;
        }
        
        switch(item->getType()) {
            case ItemType::HEALTH_POTION:
                heal(item->getValue());
                cout << "Used " << itemName << " and restored " << item->getValue() << " health." << endl;
                break;
            case ItemType::MANA_POTION:
                restoreMana(item->getValue());
                cout << "Used " << itemName << " and restored " << item->getValue() << " mana." << endl;
                break;
            default:
                cout << "Cannot use " << itemName << " directly." << endl;
                return false;
        }
        
        if(item->getConsumable()) {
            removeItem(item->getId());
        }
        return true;
    }
    
    // Healing and mana restoration
    void heal(int amount) {
        health += amount;
        if(health > maxHealth) health = maxHealth;
    }
    
    void restoreMana(int amount) {
        mana += amount;
        if(mana > maxMana) mana = maxMana;
    }
    
    // Level up system
    void gainExperience(int exp) {
        experience += exp;
        cout << "Gained " << exp << " experience!" << endl;
        
        // Level up logic
        int expNeeded = level * 100;
        if(experience >= expNeeded) {
            levelUp();
        }
    }
    
    void levelUp() {
        level++;
        experience = 0;
        maxHealth += 20;
        maxMana += 10;
        attack += 5;
        defense += 3;
        health = maxHealth; // Full heal on level up
        mana = maxMana;
        cout << "Level up! You are now level " << level << "!" << endl;
    }
    
    // Movement
    void move(int newX, int newY) {
        setPosition(newX, newY);
        cout << name << " moved to (" << newX << ", " << newY << ")" << endl;
    }
    
    // Getters
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    int getMana() const { return mana; }
    int getMaxMana() const { return maxMana; }
    int getAttack() const { return attack; }
    int getDefense() const { return defense; }
    int getLevel() const { return level; }
    int getExperience() const { return experience; }
    const vector<Item*>& getInventory() const { return inventory; }
    
    // Setters
    void setHealth(int hp) { health = hp; }
    void setMana(int mp) { mana = mp; }
    void setAttack(int att) { attack = att; }
    void setDefense(int def) { defense = def; }
    
    // Serialization
    virtual void serialize(ofstream& file) const override {
        Entity::serialize(file);
        file.write(reinterpret_cast<const char*>(&health), sizeof(health));
        file.write(reinterpret_cast<const char*>(&maxHealth), sizeof(maxHealth));
        file.write(reinterpret_cast<const char*>(&mana), sizeof(mana));
        file.write(reinterpret_cast<const char*>(&maxMana), sizeof(maxMana));
        file.write(reinterpret_cast<const char*>(&attack), sizeof(attack));
        file.write(reinterpret_cast<const char*>(&defense), sizeof(defense));
        file.write(reinterpret_cast<const char*>(&level), sizeof(level));
        file.write(reinterpret_cast<const char*>(&experience), sizeof(experience));
        
        int inventorySize = inventory.size();
        file.write(reinterpret_cast<const char*>(&inventorySize), sizeof(inventorySize));
        for(const Item* item : inventory) {
            item->serialize(file);
        }
    }
    
    virtual void deserialize(ifstream& file) override {
        Entity::deserialize(file);
        file.read(reinterpret_cast<char*>(&health), sizeof(health));
        file.read(reinterpret_cast<char*>(&maxHealth), sizeof(maxHealth));
        file.read(reinterpret_cast<char*>(&mana), sizeof(mana));
        file.read(reinterpret_cast<char*>(&maxMana), sizeof(maxMana));
        file.read(reinterpret_cast<char*>(&attack), sizeof(attack));
        file.read(reinterpret_cast<char*>(&defense), sizeof(defense));
        file.read(reinterpret_cast<char*>(&level), sizeof(level));
        file.read(reinterpret_cast<char*>(&experience), sizeof(experience));
        
        int inventorySize;
        file.read(reinterpret_cast<char*>(&inventorySize), sizeof(inventorySize));
        
        // Clear existing inventory
        for(Item* item : inventory) {
            delete item;
        }
        inventory.clear();
        
        // Load inventory items
        for(int i = 0; i < inventorySize; i++) {
            Item* item = new Item();
            item->deserialize(file);
            inventory.push_back(item);
        }
    }
};

#endif