/*This source code copyrighted by Lazy Foo' Productions (2004-2022)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <string>

using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

bool init();
void close(SDL_Surface* surface);

void eventHandler();

bool loadMedia();
SDL_Surface* loadSurface(string path);

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gCurrentSurface = NULL;
SDL_Surface* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];

bool init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL2 couldn't be initialized: %s\n", SDL_GetError());
		return false;
	}
	else
	{
		gWindow = SDL_CreateWindow("Cosmic Kong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window couldn't be created: %s\n", SDL_GetError());
			return false;
		}
		else
		{
			gScreenSurface = SDL_GetWindowSurface(gWindow);
			return true;
		}
	}
}

void close(SDL_Surface* surface)
{
	SDL_FreeSurface(surface);
	surface = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
}

void eventHandler()
{
	SDL_Event e;
	bool quit = false;
	while (!quit)
	{
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
				case SDL_QUIT:
					quit = true;
					break;
				case SDL_KEYDOWN:
					switch (e.key.keysym.sym)
					{
						case SDLK_UP:
							gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
							break;
						case SDLK_RIGHT:
							gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
							break;
						case SDLK_DOWN:
							gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
							break;
						case SDLK_LEFT:
							gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
							break;
						default:
							gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
							break;
					}

			}
		}

		SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);

		SDL_UpdateWindowSurface(gWindow);

	}
}

bool loadKeyPressSurface(int index, string path)
{
	gKeyPressSurfaces[index] = loadSurface(path);
	if (gKeyPressSurfaces[index] == NULL)
	{
		printf("Key press image couldn't load.\n");
		return false;
	}
	return true;
}

bool loadMedia()
{
	return loadKeyPressSurface(KEY_PRESS_SURFACE_DEFAULT, "./src/images/default.bmp") &&
		loadKeyPressSurface(KEY_PRESS_SURFACE_UP, "./src/images/up.bmp") &&
		loadKeyPressSurface(KEY_PRESS_SURFACE_RIGHT, "./src/images/right.bmp") &&
		loadKeyPressSurface(KEY_PRESS_SURFACE_DOWN, "./src/images/down.bmp") &&
		loadKeyPressSurface(KEY_PRESS_SURFACE_LEFT, "./src/images/left.bmp");
}

SDL_Surface* loadSurface(string path)
{
	SDL_Surface* surface = SDL_LoadBMP(path.c_str());

	if (surface == NULL)
	{
		printf("%s couldn't load: %s\n", path.c_str(), SDL_GetError());
	}

	return surface;
}

int main(int argc, char* args[])
{


	//Initialize SDL
	if (!init())
	{
		printf("SDL could not initialize.\n");
	}
	else
	{
		if (!loadMedia())
		{
			printf("Media couldn't load.\n");
		}
		else
		{
			eventHandler();
		}
	}

	close(gCurrentSurface);

	return 0;
}
