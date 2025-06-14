🏰 Dungeon Game Engine - Header-Only Module
Welcome to the Dungeon Game Engine, a modular, object-oriented C++ dungeon crawler framework! This header file (Dungeon.h) defines the core engine logic for building, navigating, and interacting with a dungeon map — complete with monsters, items, rooms, and a player character.

📦 Features
Object-Oriented Design: Uses classes for Dungeon, Location, Character, Item, and Monster.

Templates: Generic GameContainer<T> for managing collections of game entities.

Composition & Association: Dungeon holds a grid of locations and interacts with player, monsters, and items.

Serialization: Save and load the entire dungeon state using file streams.

Game Logic:

Dungeon generation (rooms, treasure rooms, corridors, entrance & exit)

Player movement & location-based interaction

Item and monster spawning

Win condition check

Map Display: Visually shows dungeon layout with symbols and player position.

📂 File Structure
bash
Copy
Edit
📁 YourProject
├── Dungeon.h            # This file (main engine)
├── Location.h           # Defines locations, types, and map symbols
├── Character.h          # Base character class (e.g., player)
├── Item.h               # Items like potions, swords, keys, etc.
├── Monster.h            # Monsters (Goblin, Orc, Dragon, etc.)
├── String.h             # Custom string class used in all files
└── main.cpp             # Your game logic (example usage)
🧩 Class Overview
Dungeon
Builds and manages the dungeon grid.

Spawns predefined items and monsters.

Moves player and updates map display.

Handles serialization and deserialization.

GameContainer<T>
Template class to store and manage any game object with getId() function.

Supports: add, find, remove, getAll, clear, and destructor cleanup.

Dependencies
Location, Character, Monster, Item, and String classes.

🗺️ Map Symbols (Used in displayMap())
Symbol	Meaning
S	Start (Entrance)
E	Exit (Goal)
R	Room
T	Treasure Room
M	Monster Present
I	Item Present
.	Corridor
P	Player Position
?	Unexplored/Unknown
