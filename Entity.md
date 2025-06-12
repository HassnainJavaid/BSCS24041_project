🧩 Entity Base Class - C++ Inheritance & Polymorphism Demo
This project demonstrates the core principles of Object-Oriented Programming (OOP) in C++ using a base class named Entity. It showcases key OOP concepts including:

Inheritance

Polymorphism (virtual functions)

Encapsulation

Operator overloading

Custom serialization/deserialization

Custom string handling with a user-defined String class

📁 File Overview
Entity.h
Defines an abstract base class Entity that can be inherited by more specific entity types (e.g., Player, NPC, Object, etc.). It includes:

🔧 Core Components
✅ Attributes:
String name: Custom string class used for the entity's name.

int x, y: Coordinates of the entity's position.

int id: Unique identifier for each entity.

bool isActive: Indicates if the entity is active in the system or not.

🧱 Constructors:
Default Constructor: Initializes with default values.

Parameterized Constructor: Allows initialization with custom name, position, and ID.

🔁 Pure Virtual Functions (for Polymorphism):
These must be implemented by derived classes.

void display() const: For displaying entity details.

bool interact(): To handle interaction logic.

🎮 Virtual Serialization:
serialize(ofstream&): Writes entity data to a binary file.

deserialize(ifstream&): Reads entity data from a binary file.

➕ Operator Overloading:
operator==: Compares two entities based on their ID.

operator<<: Streams entity data in a readable format.

🧰 Utility Functions:
Getters and setters for all attributes.

setPosition() for updating coordinates.

📦 Usage
This Entity class is not directly instantiable because it contains pure virtual functions. You must derive a subclass and implement the display() and interact() method
