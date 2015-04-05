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
#define DELAY_SLOW 10000 //dealy while in background
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
int pause;//if game is paused or not





typedef struct data{
	int oid;//object id
	int frame;//which frame it is in
	int x, y;//x and y position
	struct data *next;//linked list
}Data;

typedef void(*Button)(Data *);//function for buttons

typedef struct object{//array of clickable object definitions
	int iw, ih;//width and height of image to display
	int frames;//number of frames
	SDL_Texture *texture;//id of pointer to texture
	Button button;//button function to run if clicked on
	Button status;//id of function to display status bar while selected
	Button update;//id of function to update every frame
	double w, h;//width, and hight in scale from maxside
	int center;//if centering on x and y or not
	int selectable;//if selectable or not
}Object;

Object **objects;//object defenitions
unsigned long int objectssize;//size of objects array

SDL_Texture **textures;//array of texture pointers
unsigned long int texturessize;//size of textures array

Data *start;//first object to draw in the linked list

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
void Clicked(long int x, long int y);//x and y positions clicked
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
void LoadObjects(void);//load all objects
void DrawEdge(void);//draw edge border of screen
unsigned int AddTexture(SDL_Texture *texture);//add texture to memory and return that id
unsigned int AddObject(Object *object);//add object to memory and return that id
Object *MakeObject(int iw, int ih, int frames, SDL_Texture *texture, Button button, Button status, Button update, double w, double h, int center, int selectable);//make object with that paramiters


#endif