#include <stdio.h>
#include "SDL.h"

//OSC specific includes
#include "OscReceivedElements.h"
#include "OscPacketListener.h"
#include "UdpSocket.h"


SDL_Window* gWindow = NULL;

SDL_Surface* gScreenSurface = NULL;

SDL_Surface* gHelloWorld = NULL;

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;

bool init();

bool loadMedia();

void close();

bool init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) 
	{
		printf("error: sdl did not initialize! SDL_ERROR: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL) 
		{
			printf("Window could not be created! SDL_ERROR: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}


	//osc init

	return success;
}

bool loadMedia()
{
	bool success = true;
    //note: for iOS, will have to check image path!
	gHelloWorld = SDL_LoadBMP("../../../image.bmp");
	if (gHelloWorld == NULL)
	{
		printf("unable to load img! %s\n", SDL_GetError());
		success = false;
	}
	return success;
}

void close()
{

	SDL_FreeSurface(gHelloWorld);
	gHelloWorld = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
}

int main (int argc, char* args[])
{
	if (!init()) 
	{
		printf("failed to init!\n");
	}
	else
	{
		if (!loadMedia()) 
		{
			printf("failed to load media!\n");
		}
		else
		{
			SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);
		}

		bool quit = false;
		SDL_Event e;


		

		while (!quit) 
		{
			SDL_UpdateWindowSurface(gWindow);
			while (SDL_PollEvent(&e) != 0) 
			{
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}

			}
			// Read OSC here
		}
	}

	close();

	return 0;
}