#ifndef ITEM_H
#define ITEM_H

#include "Entity.h"
#include "String.h"

enum class ItemType 
{
    HEALTH_POTION,
    MANA_POTION,
    SWORD,
    SHIELD,
    KEY,
    TREASURE
};

class Item : public Entity {
private:
    ItemType type;
    int value;
    String description;
    bool isConsumable;

public:
    Item() : Entity(), type(ItemType::HEALTH_POTION), value(0), isConsumable(true) {
        description = "A mysterious item";
    }
    
    Item(const String& itemName, int posX, int posY, int itemId, ItemType itemType, int itemValue, bool consumable = true)
        : Entity(itemName, posX, posY, itemId), type(itemType), value(itemValue), isConsumable(consumable) {
        
        switch(itemType) {
            case ItemType::HEALTH_POTION:
                description = "Restores health points";
                break;
            case ItemType::MANA_POTION:
                description = "Restores mana points";
                break;
            case ItemType::SWORD:
                description = "A sharp weapon for combat";
                isConsumable = false;
                break;
            case ItemType::SHIELD:
                description = "Provides defense against attacks";
                isConsumable = false;
                break;
            case ItemType::KEY:
                description = "Opens locked doors";
                isConsumable = false;
                break;
            case ItemType::TREASURE:
                description = "Valuable treasure";
                isConsumable = false;
                break;
        }
    }
    
    virtual ~Item() {}
    
    // Override virtual functions
    virtual void display() const override {
        cout << "Item: " << name << " - " << description << " (Value: " << value << ")" << endl;
    }
    
    virtual bool interact() override {
        cout << "You picked up: " << name << endl;
        return true;
    }
    
    // Getters
    ItemType getType() const { return type; }
    int getValue() const { return value; }
    String getDescription() const { return description; }
    bool getConsumable() const { return isConsumable; }
    
    // Setters
    void setValue(int newValue) { value = newValue; }
    void setDescription(const String& desc) { description = desc; }
    
    // Serialization
    virtual void serialize(ofstream& file) const override {
        Entity::serialize(file);
        file.write(reinterpret_cast<const char*>(&type), sizeof(type));
        file.write(reinterpret_cast<const char*>(&value), sizeof(value));
        file.write(reinterpret_cast<const char*>(&isConsumable), sizeof(isConsumable));
        
        int descLen = description.size();
        file.write(reinterpret_cast<const char*>(&descLen), sizeof(descLen));
        for(int i = 0; i < descLen; i++) {
            file.write(&description[i], sizeof(char));
        }
    }
    
    virtual void deserialize(ifstream& file) override {
        Entity::deserialize(file);
        file.read(reinterpret_cast<char*>(&type), sizeof(type));
        file.read(reinterpret_cast<char*>(&value), sizeof(value));
        file.read(reinterpret_cast<char*>(&isConsumable), sizeof(isConsumable));
        
        int descLen;
        file.read(reinterpret_cast<char*>(&descLen), sizeof(descLen));
        description.clear();
        for(int i = 0; i < descLen; i++) {
            char ch;
            file.read(&ch, sizeof(char));
            description.push_back(ch);
        }
    }
};

#endif