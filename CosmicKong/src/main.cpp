/*This source code copyrighted by Lazy Foo' Productions (2004-2022)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

bool init();
bool loadMedia();
void close();

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gImage = NULL;

void hack()
{
	SDL_Event e; 
	bool quit = false; 
	while (quit == false)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}
	}
}

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

bool loadMedia()
{
	gImage = SDL_LoadBMP("./src/images/image.bmp");
	if (gImage == NULL)
	{
		printf("Image couldn't load.\n");
		return false;
	}
	return true;
}

void close()
{
	SDL_FreeSurface(gImage);
	gImage = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
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
			SDL_BlitSurface(gImage, NULL, gScreenSurface, NULL);

			SDL_UpdateWindowSurface(gWindow);

			hack();
		}
	}

	close();

	return 0;
}
