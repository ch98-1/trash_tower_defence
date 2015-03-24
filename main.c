//Copyright (C) 2014-2015  Corwin Hansen
#include "main.h"//main header


int main(int argc, char *argv[]){

	//initialise variables 
	delay = DELAY;//10 ms delay

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0){//start SDL and get any error if it dosen't.
		printf("Could not load SDL: %s\n", SDL_GetError());//print out error
		exit(EXIT_FAILURE);//exit
	}




	// load support for PNG image format
	int flags = IMG_INIT_PNG;//flag for PNG image loading
	int initted = IMG_Init(flags);//get initialised flags
	if (initted&flags != flags) {//if it caused error
		printf("Could not initialised png support: %s\n", IMG_GetError());//print error message
		exit(EXIT_FAILURE);//exit
	}


	//initialise ttf
	if (TTF_Init() == -1) {//if it causes error
		printf("could not initialised ttf support: %s\n", TTF_GetError());//print error message
		exit(EXIT_FAILURE);//exit
	}


	//filter events
	SDL_SetEventFilter(EventFilter, NULL);

	
	//quit SDL at exit
	atexit(Quit);


	GetDisplay();//get display

	
	//Create fullscreen window
	window = SDL_CreateWindow(
		WINDOW_NAME,                  // window title
		SDL_WINDOWPOS_CENTERED,           // initial x position
		SDL_WINDOWPOS_CENTERED,           // initial y position
		width,                               // width, in pixels
		height,                               // height, in pixels
		SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_BORDERLESS        //fullscreen borderless window
		);


	// Check that the window was made
	if (window == NULL) {
		// In the event that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);//exit
	}


	//Create renderer
	renderer = SDL_CreateRenderer(window, -1, 0);


	// Check that the renderer was made
	if (renderer == NULL) {
		// In the event that the window could not be made...
		printf("Could not create renderer: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);//exit
	}



	
	//test image 
	SDL_Texture* texture = GetTesture("test.png");
	//test font
	TTF_Font* font = GetFont("OpenSans-Regular.ttf", 72);
	SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Blended(font, "hello world", (SDL_Color){ 0, 0, 0 }));


	while (1) {
		while (SDL_PollEvent(&event)) {//for each event
			switch (event.type) {//for each event type
			case SDL_QUIT://quit everything
				Quit();//quit everything
				break;//get out
			case SDL_MOUSEBUTTONDOWN://when clicking down
				if (event.button.which != SDL_TOUCH_MOUSEID){//ignore touch event for later
					Clicked(event.button.x, event.button.y);//run clicked function
				}
				break;//get out
			case SDL_MOUSEMOTION://when mouse moved
				if (event.button.which != SDL_TOUCH_MOUSEID){//ignore touch event for later
					MouseX = event.button.x;//set x and y position of mouse
					MouseY = event.button.y;
				}
				break;//get out
			case SDL_FINGERDOWN://when touching down
				Clicked(event.button.x, event.button.y);//run clicked function
				break;//get out
			case SDL_FINGERMOTION://when finger moved
				MouseX = event.button.x;//set x and y position of mouse
				MouseY = event.button.y;
				break;//get out
			default://for everything else
				//ignore event
				break;//get out
			}
		}




		//test image
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);//draw white
		SDL_RenderClear(renderer);//clear screen


		SDL_Rect rect;//rectangle to draw in
		rect.w = maxside/4;//centered to mouse with 1/4 of small side
		rect.h = maxside/4;
		rect.x = MouseX - rect.w / 2;
		rect.y = MouseY - rect.h / 2;
		SDL_RenderCopy(renderer, texture, NULL, &rect);//draw image
		int w, h, id;
		long stuff;
		SDL_QueryTexture(text, &stuff, &id, &w, &h);
		rect.w = w;
		rect.h = h;
		rect.x = 0;
		rect.y = 0;
		SDL_RenderCopy(renderer, text, NULL, &rect);//draw text
		SDL_RenderPresent(renderer);//update screen








		SDL_Delay(delay);
	}

	exit(EXIT_SUCCESS);//if it somehow reaches here
}









int EventFilter(void* userdata, SDL_Event* e){//event filter
	switch (e->type) {//for each event type
	case SDL_QUIT://quit everything
		exit(EXIT_SUCCESS);//exit
		return 0;//delete that event
		break;//get out
	default://for everything else
		return 1;//don't delete event
		break;//get out
	}
	return 1;//just in case something wiard happens
}






void Quit(void){//quit everything

	IMG_Quit();//quit SDL_Image
	TTF_Quit();//quit SDL_TTF
	SDL_Quit();//quit SDL
	
	return;//exit function if it didn't exit for some reason
}








void GetDisplay(void){//get display
	//get desplay mode of the desktop
	if (SDL_GetDesktopDisplayMode(0, &display) != 0) {
		SDL_Log("Could not get display mode: %s\n", SDL_GetError());//take care of errors
		exit(EXIT_FAILURE);//exit
	}

	width = display.w;//set display width and height
	height = display.h;

	if (width > height){//get smallest of 2 sides
		maxside = height;
	}
	else {
		maxside = width;
	}
	return;//exit function
}







void Clicked(long int x, long int y){//x and y positions clicked



	MouseX = event.button.x;//set x and y position of mouse
	MouseY = event.button.y;
	return;//exit function
}







SDL_Texture* GetTesture(const char *file){//make texture from this file
	char image[256] = RESOURCES;//folder path
	strcat(image, file);//append path
	SDL_Surface* surface = IMG_Load(image);//load surface
	if (surface == NULL){//if it could not be loaded
		printf("could not load image: %s\n", IMG_GetError());//error message
		exit(EXIT_FAILURE);//exit
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);//get texture from loaded image
	if (surface == NULL){//if it could not be converted to texture
		printf("could not create texture: %s\n", SDL_GetError());//error message
		exit(EXIT_FAILURE);//exit
	}
	return texture;//return texture
}







TTF_Font* GetFont(const char *file, int size){//get font from file
	char fontfile[256] = RESOURCES;//folder path
	strcat(fontfile, FONTS);//append fonts path
	strcat(fontfile, file);//append path
	TTF_Font *font = TTF_OpenFont(fontfile, size);//get font
	if (font == NULL){//if it could not be loaded
		printf("could not load font: %s\n", TTF_GetError());//error message
		exit(EXIT_FAILURE);//exit
	}
	return font;//return font
}