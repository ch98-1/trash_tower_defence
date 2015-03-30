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
#define CENTERED -32000 //center x and y in draw function

//paths
#define RESOURCES "resources/" //media resources path. 
#define FONTS "fonts/" //fonts path. Inside media resources path.
#define SAVE "resources/save.txt" //saved data file path. Same format as maps.
#define FONT "OpenSans-Regular.ttf" //font path
#define MAP "map.txt" //map file path has map name with path. Will load first map on bootup.


//message stuff
SDL_Texture *messagetexture;//message to display
int message;//0 for no message, 1 for undisplayd message, 2 for displayd message

char maps[26][2][256];//map name and path for each of 26 maps [map number][0 for name, 1 for path] maximum 256 characters
SDL_Texture *menuetexture[28];//texture for 28 texts on the menue

SDL_DisplayMode display;//display mode 
SDL_Window *window;//sdl window
SDL_Renderer *renderer;//sdl renderer
SDL_Event event;//event

int width, height, maxside;//width and hight and smallest of 2 side of window
int baseX, baseY;//x and y starting point


double money, landfill, speed;//status of game
long int month;
int menue;//if there is menue on screen 0 for normal play, 1 for displaying menue, 2 for menue already displayd
int map;//which one of 26 map to use



uint32_t delay;//amount of delay for each frame
long int MouseX, MouseY;//x and y position of mouse / touch


//fonts for different sizes (1/n of maxside)
TTF_Font* font_4;
TTF_Font* font_6;
TTF_Font* font_8;
TTF_Font* font_16;
TTF_Font* font_24;
TTF_Font* font_32;
TTF_Font* font_46;
TTF_Font* font_64;


int EventFilter(void* userdata, SDL_Event* e);//event filter
void Quit(void);//quit everything
void GetDisplay(void);//get display
void Clicked(int x, int y);//x and y positions clicked
void Resize(void);//recalculate numbers related to size
SDL_Texture* GetTexture(const char *file);//make texture from this file
TTF_Font* GetFont(const char *file, int size);//get font from file
SDL_Texture* GetTextTexture(TTF_Font* font, const char* text, int r, int g, int b);//make texture from font text and rgb
void DrawBase(void);//draw basic stuff
void Message(const char *text);//display message in 
int LoadFile(const char *file);//load file in to memory. return 0 for success
void Save(void);//save in to save file
void LoadMenue(void);//load menue texts
void DrawText(SDL_Texture *texture, int x, int y, SDL_Rect *rect);//draw rect of texture at x and y position. Null rect for whole texture. -32000 or CENTERED to get that axis centered
void DrawIMG(SDL_Texture *texture, int x, int y, SDL_Rect *rect, double w, double h, int center);//draw rect of texture at x and y position at scale from maxside. Null rect for whole texture. -32000 or CENTERED to get that axis centered. set center to 1 to center to x and y

#endif