#include <iostream>
#include <SDL.h> 
#include <SDL_image.h>
#include "Sprite.h"
#include "Game.h"
// We need to figure out how to...
//1.	get SDL header files (.h files) to be included in this project so we can call its functions in the source code
//2.	get SDL precompiled libraries (.lib files) to be linked in this project so when we compile our code it can connect with SDL's compiled code!
//3.	get SDL dynamically linked libraries to connect so that our when our application runs, it can call SDL's functions
//4.	load a texture
//5		create a game loop
//6.	create a window object and a renderer object
//7.	render images to the window

//Things to look up:
//C++ Classes and objects
/*
A class in C++... defines a new Type. It is a blueprint that allows you to create many Objects of this Type. 
When you define a class, you can make variables of it as a new type

A class can be instantiated to create a new Object

A class contains data members and functions
the data members can belong to instances of the class
the functions can operate on that class

*/ 

//C++ functions
//C++ pointers
//SDL_Rect
//SDL_Texture
//SDL_Renderer
//SDL_Window
//nullptr
/*
main
{
	game = Game();
	game.run();
	
	load()
	while(gameRunning)
	{
		game.input();
		game.update();
		game.draw();
	}
	cleanup();
}

load()
{
	ship1 =  Ship()
	ship1.setLocation(50,500)

	ship2 =  Sprite("ship2.png")
	ship2.setPosition(50,500)
}

update()
{
	ship1.update()
}

draw()
{
	ship1.draw();
}

*/


Game myGame;

// Main function.
int main(int argc, char* args[]) // Main MUST have these parameters for SDL.
{
	myGame.run();
	
	getchar();
	return 0;
}