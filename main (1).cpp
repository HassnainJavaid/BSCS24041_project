#include "GameEngine.h"
#include "GameExceptions.h"
#include <iostream>
using namespace std;

int main()
{
    try
    {
        cout << "Starting Dungeon Crawler Game..." << endl;
        
        GameEngine game;
        game.run();
        
        if(game.hasWon()) 
        {
            cout << "\nFinal Score: VICTORY!" << endl;
        } 
        else 
        {
            cout << "\nThanks for playing the Dungeon Crawler!" << endl;
        }
        
    } 
    catch(const GameException& e)
    {
        cout << "Game Error: " << e.what() << endl;
        cout << "The game will now exit." << endl;
        return 1;
    } 
    catch(const exception& e)
    {
        cout << "Critical Error: " << e.what() << endl;
        cout << "The game will now exit." << endl;
        return 1;
    } 
    catch(...) 
    {
        cout << "Unknown error occurred. The game will now exit." << endl;
        return 1;
    }
    
    return 0;
}