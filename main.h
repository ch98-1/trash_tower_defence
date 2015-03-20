//Copyright (C) 2014-2015  Corwin Hansen
#ifndef MAIN_H//include protection
	#define MAIN_H


#include <stdio.h>//input and output
#include <stdlib.h>//standard stuff
#include <math.h>//all the math functions
#include <string.h>//string related things
#include <time.h>//time on system
#include "SDL.h"//include sdl
#include "SDL_ttf.h"//include sdl_ttf
#include "SDL_image.h"//include sdl_image


#define WINDOW_NAME "Trash Tower Defence"
#define DELAY 10 //default delay
#define SLEEP_DELAY 1000 //delay at sleep
#define RESOURCES "resources/" //media resources path. 
#define SAVE "resources/save.txt" //saved data file path




SDL_DisplayMode display;//display mode 
SDL_Window *window;//sdl window
SDL_Renderer *renderer;//sdl renderer
SDL_Event event;//event

int width, height, maxside;//widthand hight and smallest of 2 side of window

uint32_t delay;//amount of delay for each frame
long int MouseX, MouseY;//x and y position of mouse / touch


int EventFilter(void* userdata, SDL_Event* e);//event filter
void Quit(void);//quit everything
void GetDisplay(void);//get display
void Clicked(int x, int y);//x and y positions clicked
SDL_Texture* GetTesture(const char *file);//make texture from this file

#endif