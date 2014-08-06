#include <stdio.h>
#include "SDL.h"

//OSC specific includes
#include "ofxOscReceiver.h"
#include "ofxOscSender.h" //just for sending example


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
    //OSC SPECIFIC: Set up OSC reader on port 7000
    ofxOscReceiver myReader;
    myReader.setup(7000);
    
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
                if (e.type ==SDL_KEYDOWN)
                {
                    if (e.key.keysym.sym == SDLK_s)
                    {
                        //OSC SPECIFIC: if we press s, send a test OSC message (to ourselves)
                        // below shows initialization of a sender, creation and sending of a message
                        ofxOscSender mySender;
                        mySender.setup("127.0.0.1", 7000); //send to self
                        
                        ofxOscMessage msg;
                        msg.setAddress("/testMsg");
                        msg.addFloatArg(1.23);
                        msg.addStringArg("hello world");
                        msg.addIntArg(3);
                        mySender.sendMessage(msg);
                        
                        
                        
                    }
                }
			}
            
            //OSC SPECIFIC: check for messages, and parse them
            if (myReader.hasWaitingMessages()) {
                ofxOscMessage msg;
                myReader.getNextMessage(&msg);
                printf("received osc message with tag =%s\n", msg.getAddress().c_str());
                for (int i=0; i<msg.getNumArgs(); i++) {
                    printf("arg%i type = %s\n", i, msg.getArgTypeName(i).c_str());
                }
                
            }
		}
	}
    

	close();

	return 0;
}