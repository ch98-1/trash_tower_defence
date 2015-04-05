//Copyright (C) 2014-2015  Corwin Hansen
#include "main.h"//main header


int main(int argc, char *argv[]){

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0){//start SDL and get any error if it dosen't.
		printf("Could not load SDL: %s\n", SDL_GetError());//print out error
		exit(EXIT_FAILURE);//exit
	}




	// load support for PNG image format
	int flags = IMG_INIT_PNG;//flag for PNG image loading
	int initted = IMG_Init(flags);//get initialised flags
	if ((initted&flags) != flags) {//if it caused error
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


	//initialise and load stuff
	delay = DELAY;//10 ms delay
	Resize();//resize stuff
	menue = 0;//default no menue at start
	message = 0;//no message at start
	LoadMenue();//load menue textures
	if (LoadFile(SAVE)){//if save file could not be loaded
		menue = 1;//need to display menue
		Message("Could not load save file");//send message that save file could not be loaded
	}
	objectssize = 0;//initialise array sizes
	texturessize = 0;
	pause = 0;//start without pausing







	//Load textures and objects
	LoadObjects();





	//main loop
	while (1) {
		while (SDL_PollEvent(&event)) {//for each event
			switch (event.type) {//for each event type
			case SDL_QUIT://quit everything
				Quit();//quit everything
				break;//get out
			case SDL_MOUSEBUTTONDOWN://when clicking down
					Clicked(event.button.x - baseX, event.button.y - baseY);//run clicked function 
				break;//get out
			case SDL_MOUSEMOTION://when mouse moved
					MouseX = event.button.x - baseX;//set x and y position of mouse from square
					MouseY = event.button.y - baseY;
				break;//get out
			default://for everything else
				//ignore event
				break;//get out
			}
		}




		if (message){//if displaying message
			if (message == 1){//if message was not displayd yet
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);//draw white
				SDL_RenderClear(renderer);//clear screen
				DrawText(messagetexture, CENTERED, CENTERED, NULL);//draw message at center
				DrawEdge();//draw edge
				SDL_RenderPresent(renderer);//update screen
				message = 2;
			}

		}
		else{//if there is no message

			if (menue){//if in menue
				if (menue == 1){//if you need to display menue
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);//draw white
					SDL_RenderClear(renderer);//clear screen
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);//draw black
					SDL_Rect rect;//rectangle to draw
					long int y;//y position
					int i = 0;//counter
					for (y = maxside / 16; y < maxside - maxside / 16; y += maxside / 32){
						rect.w = maxside;//rectangle to draw
						rect.h = maxside / 32;
						rect.x = baseX;
						rect.y = y;
						SDL_RenderDrawRect(renderer, &rect);//draw rectangle
						DrawText(menuetexture[i], CENTERED, y, NULL);
						i++;//count i up
					}

					DrawEdge();//draw edge
					SDL_RenderPresent(renderer);//update screen
					menue = 2;
				}

			}





			else if(!pause){//if in normal play mode and not in pause

				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);//draw white
				SDL_RenderClear(renderer);//clear screen

				DrawBase();//draw basic stuff for normal play


				DrawIMG(textures[0], MouseX, MouseY, NULL, 0.25, 0.25, 1);//draw image centered at mouse
				int x = ((int)((clock() * 100) / CLOCKS_PER_SEC)) % maxside;
				int y = baseY;
				DrawText(textures[1], x, baseY, NULL);//draw that text at top
				for (y = 0; y < maxside; y += maxside / 32){
					DrawText(textures[1], x, y, NULL);//draw text
				}





				DrawEdge();//draw edge
				SDL_RenderPresent(renderer);//update screen
			}

		}


		SDL_Delay(delay);
	}

	exit(EXIT_SUCCESS);//if it somehow reaches here
}









void Message(const char *text){//display message 
	messagetexture = GetTextTexture(font_32, text, 0, 0, 0);//render black text of the message
	message = 1;//set to display message
}









int EventFilter(void* userdata, SDL_Event* e){//event filter
	switch (e->type) {//for each event type
	case SDL_QUIT://quit everything
		exit(EXIT_SUCCESS);//exit
		return 0;//delete that event
		break;//get out

	case SDL_APP_TERMINATING://quit everything
		exit(EXIT_SUCCESS);//exit
		return 0;//delete that event
		break;//get out

	case SDL_APP_LOWMEMORY://if low in memory
		//can't do anything
		return 0;//delete that event
		break;//get out

	case SDL_APP_WILLENTERBACKGROUND://if entering background
		Save();//save game
		pause = 1;//pause game
		delay = DELAY_SLOW;//lower fps
		return 0;//delete that event
		break;//get out

	case SDL_APP_DIDENTERBACKGROUND://if in background
		//don't do anything
		return 0;//delete that event
		break;//get out

	case SDL_APP_WILLENTERFOREGROUND://if entering forgeound
		LoadFile(SAVE);//load game
		pause = 0;//unpause pause game
		delay = DELAY;//higher fps
		return 0;//delete that event
		break;//get out

	case SDL_APP_DIDENTERFOREGROUND://if in forground
		//don't do anything
		return 0;//delete that event
		break;//get out

	default://for everything else
		return 1;//don't delete event
		break;//get out
	}
	return 1;//just in case something wiard happens
}






void Quit(void){//quit everything

	Save();//save file before quitting
	TTF_CloseFont(font_4);//close all fonts
	TTF_CloseFont(font_6);
	TTF_CloseFont(font_8);
	TTF_CloseFont(font_16);
	TTF_CloseFont(font_24);
	TTF_CloseFont(font_32);
	TTF_CloseFont(font_46);
	TTF_CloseFont(font_64);
	SDL_DestroyRenderer(renderer);//destroy renderer
	int i;//counter
	for (i = 0; i < texturessize / sizeof(SDL_Texture*); i++){//destroy each texture
		SDL_DestroyTexture(textures[i]);//destroy texture
	}
	for (i = 0; i < 28; i++){//destroy each menue texture
		SDL_DestroyTexture(menuetexture[i]);//destroy texture
	}
	free(textures);//free textures array
	free(objects);//free objects array
	SDL_DestroyWindow(window);//destroy window
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
	if (menue && !message && x > 0 && x < maxside){//if in menue and not in message and in right area
		int clicked = y / (maxside / 32);//which button was clicked
		if (clicked > 2 && clicked < 29){ //if within the range of levels
			map = clicked - 3;//get clicked button
			menue = LoadFile(maps[map][1]);//load that level
		}
		if (clicked == 29){//Canceld
			menue = 0;//stop showing menue
		}
	}
	else if(!pause){//if in normal play mode and not paused

	}


	message = 0;//reset message
	MouseX = event.button.x - baseX;//set x and y position of mouse from square
	MouseY = event.button.y - baseY;
	return;//exit function
}







SDL_Texture* GetTexture(const char *file){//make texture from this file
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
	SDL_FreeSurface(surface);//free surface
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








SDL_Texture* GetTextTexture(TTF_Font* font, const char* text, int r, int g, int b){//make texture from font text and rgb
	SDL_Color color;//set color
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = 255;
	SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);//rendere text as surface
	if (surface == NULL){//if it could not be loaded
		printf("could not load text: %s\n", TTF_GetError());//error message
		exit(EXIT_FAILURE);//exit
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);//get texture from loaded image
	if (surface == NULL){//if it could not be converted to texture
		printf("could not create texture: %s\n", SDL_GetError());//error message
		exit(EXIT_FAILURE);//exit
	}
	return texture;//return created texture
}









void Resize(void){//recalculate numbers related to size
	GetDisplay();//get display

	baseX = (width - maxside) / 2;//set starting x and y position
	baseY = (height - maxside) / 2;


	//load fonts on different sizes (1/n of maxside)
	font_4 = GetFont(FONT, maxside / 4);
	font_6 = GetFont(FONT, maxside / 6);
	font_8 = GetFont(FONT, maxside / 8);
	font_16 = GetFont(FONT, maxside / 16);
	font_24 = GetFont(FONT, maxside / 24);
	font_32 = GetFont(FONT, maxside / 32);
	font_46 = GetFont(FONT, maxside / 46);
	font_64 = GetFont(FONT, maxside / 64);
}









void DrawBase(void){//draw basic stuff
}











void DrawEdge(void){//draw edge border of screen
	DrawIMG(textures[2], CENTERED, CENTERED, NULL, 2, 2, 1);//centered at twice the size of box
}












int LoadFile(const char *file){//load file in to memory. return 0 for success






	return 0;//success
}










void Save(void){//save in to save file







}










void LoadMenue(void){
	menuetexture[0] = GetTextTexture(font_46, "Choose Level", 0, 0, 0);//Title of menue
	int i;//counter
	char text[512];
	for (i = 0; i < 26; i++){//for each map
		sprintf(text, "%d: %s", i, maps[i][0]);//start name with number then 
		menuetexture[i + 1] = GetTextTexture(font_46, text, 0, 0, 0);//load text as texture
	}
	menuetexture[27] = GetTextTexture(font_46, "Cancel", 0, 0, 0);//Cancel button
}










void DrawText(SDL_Texture *texture, int x, int y, SDL_Rect *rect){//draw rect of texture at x and y position. Null rect for whole texture. -32000 or CENTERED to get that axis centered
	SDL_Rect dest;
	int w, h, access;//value to fill up
	long format;
	SDL_QueryTexture(texture, &format, &access, &w, &h);//get text box size
	dest.w = (int) w;//set width and height
	dest.h = (int) h;
	dest.x = x;//set x and y
	dest.y = y;
	if (x == CENTERED) dest.x = (maxside / 2 - w / 2);//center it if set to centered
	if (y == CENTERED) dest.y = (maxside / 2 - h / 2);

	dest.x += baseX;//set origin to basex and y
	dest.y += baseY;

	SDL_RenderCopy(renderer, texture, rect, &dest);//draw texture

}










void DrawIMG(SDL_Texture *texture, int x, int y, SDL_Rect *rect, double w, double h, int center){//draw rect of texture at x and y position at scale from maxside. Null rect for whole texture. -32000 or CENTERED to get that axis centered. set center to 1 to center to x and y
	SDL_Rect dest;
	dest.w = (int) (maxside * w);//set width and height
	dest.h = (int) (maxside * h);
	dest.x = x;//set x and y
	dest.y = y;
	if (x == CENTERED) dest.x = (int)(maxside / 2 - w / 2);//center it if set to centered
	if (y == CENTERED) dest.y = (int)(maxside / 2 - h / 2);
	if (center){
		dest.x = dest.x - dest.w / 2;//set x and y centered to x and y
		dest.y = dest.y - dest.h / 2;
	}

	dest.x += baseX;//set origin to basex and y
	dest.y += baseY;

	SDL_RenderCopy(renderer, texture, rect, &dest);//draw texture

}











void LoadObjects(void){//load all objects
	//add test textures
	AddTexture(GetTexture("test.png"));//0
	AddTexture(GetTextTexture(font_32, "Heloooooo WORLD!!!!!!!!", 0, 255, 255));//1
	AddTexture(GetTexture("border.png"));//2

}










unsigned int AddTexture(SDL_Texture *texture){//add texture to memory and return that id
	texturessize += sizeof(SDL_Texture*);//update texturessize
	textures = realloc(textures, texturessize);//reallocate memory for longer array
	if (textures == NULL){//if it becomes null
		printf("could not allocate new memory for textures\n");//print out error if it happens
		exit(EXIT_FAILURE);//exit
	}
	textures[(texturessize / sizeof(SDL_Texture*))-1] = texture;//set value
	return (texturessize / sizeof(SDL_Texture*))-1;//texture adress

}












unsigned int AddObject(Object *object){//add object to memory and return that id
	objectssize += sizeof(Object);//update objectssize
	objects = realloc(objects, objectssize);//reallocate memory for longer array
	if (objects == NULL){//if it becomes null
		printf("could not allocate new memory for object\n");//print out error if it happens
		exit(EXIT_FAILURE);//exit
	}
	objects[(objectssize / sizeof(Object*))-1] = object;//set value
	return (objectssize / sizeof(Object*))-1;//object adress

}










Object *MakeObject(int iw, int ih, int frames, SDL_Texture *texture, Button button, Button status, Button update, double w, double h, int center, int selectable){//make object with that paramiters
	Object *object = malloc(sizeof(Object));//make that object
	object->iw = iw;//set each variable
	object->ih = ih;
	object->frames = frames;
	object->texture = texture;
	object->button = button;
	object->status = status;
	object->update = update;
	object->w = w;
	object->h = h;
	object->center = center;
	object->selectable = selectable;

	return object;//return the created object
}




















