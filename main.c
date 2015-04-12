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
	speed = 1;//default of normal speed
	map = -1;//no map is -1
	Resize();//resize stuff
	start = NULL;//set start and end to null
	end = NULL;
	somethingwentwrong = GetTextTexture(font_64, "somethingwentwrong", 0, 0, 0);//image to display if something went wrong
	menubutton = GetTexture("menubutton.png");//texture for menu button

	//loading message
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);//draw white
	SDL_RenderClear(renderer);//clear screen
	SDL_Texture *loading = GetTextTexture(font_16, "Loading", 0, 0, 0);//loading message
	DrawText(loading, 0.5, 0.5, NULL, 1);//draw loading message
	SDL_RenderPresent(renderer);//present loading message
	SDL_DestroyTexture(loading);//don't need this texture

	menu = 0;//default no menu at start
	message = 0;//no message at start
	Loadmenu();//load map files and menu texts
	objectssize = 0;//initialise array sizes
	texturessize = 0;
	pause = 0;//start without pausing
	money = 0;//nomoney at start
	landfill = 0;//empty landfill
	speed = 1;//normal speed
	ftime = 0;//reset time
	difficulty = 0;//easy
	RemoveSatus();//remove status buttons
	//load save file last
	if (LoadFile(SAVE)){//if save file could not be loaded
		menu = 1;//need to display menu
		Message("Could not load save file");//send message that save file could not be loaded
	}
	if (map == -1){//if there is no map
		menu = 1;//go to menu
	}







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
				MouseX = (double)(event.button.x - baseX) / maxside;//set x and y position of mouse from square normalised
				MouseY = (double)(event.button.y - baseY) / maxside;
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
				DrawText(messagetexture, 0.5, 0.5, NULL, 1);//draw message at center
				DrawEdge();//draw edge
				SDL_RenderPresent(renderer);//update screen
				message = 2;
			}

		}
		else{//if there is no message

			if (menu){//if in menu
				if (menu == 1){//if you need to display menu
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);//draw white
					SDL_RenderClear(renderer);//clear screen
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);//draw black
					long int y;//y position
					int i = 0;//counter
					for (y = 2; y < 30; y++){//for each button
						if (y > 2) DrawIMG(menubutton, 0.5, (double)(y + 0.5) / 32, NULL, 0.5, 0.03125, 1);//draw button texture
						DrawText(menutexture[i], 0.5, (double)(y + 0.5) / 32, NULL, 1);//draw button text
						i++;//count i up
					}

					DrawEdge();//draw edge
					SDL_RenderPresent(renderer);//update screen
					menu = 2;
				}

			}





			else if(!pause){//if in normal play mode and not in pause

				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);//draw white
				SDL_RenderClear(renderer);//clear screen

				DrawBase();//draw basic stuff for normal play




				Data *current;//current data pointer
				current = start;//set current to start
				while (current != NULL){//until current is null
					Object *object = objects[current->oid];//object for this data
					object->update(current);//run update function
					SDL_Rect rect;//rectangle to draw
					rect.w = object->iw;//set width and height of image to display
					rect.h = object->ih;
					rect.x = (int)(object->iw * current->frame);//set x and y of image to draw
					rect.y = 0;
					if (object->frames = 0){
						DrawIMG(object->texture, current->x, current->y, NULL, object->w, object->h, object->center);//draw that whole texture 
					}
					else{
						DrawIMG(object->texture, current->x, current->y, &rect, object->w, object->h, object->center);//draw part of that texture
					}
					current = current->next;//set current to next
				}

				DrawEdge();//draw edge
				SDL_RenderPresent(renderer);//update screen
				ftime++;//increment time
			}

		}

		if (difftime(time(NULL), lasttime) >= 1){//if 1 second passed
			fps = ftime - lastframe;//set fps
			lasttime = time(NULL);//set new lasttime
			lastframe = ftime;
		}

		SDL_Delay((uint32_t)((double)delay / speed));
	}

	exit(EXIT_SUCCESS);//if it somehow reaches here
}









void Message(const char *text){//display message 
	SDL_DestroyTexture(messagetexture);//delete message texture before getting text
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
		speed = 1;//normal speed
		return 0;//delete that event
		break;//get out

	case SDL_APP_DIDENTERBACKGROUND://if in background
		//don't do anything
		return 0;//delete that event
		break;//get out

	case SDL_APP_WILLENTERFOREGROUND://if entering forgeound
		delay = DELAY;//higher fps
		LoadFile(SAVE);//load game
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
	SDL_DestroyTexture(somethingwentwrong);//destroy something went wrong texture
	SDL_DestroyTexture(messagetexture);//delete message texture
	int i;//counter
	for (i = 0; i < texturessize / sizeof(SDL_Texture*); i++){//destroy each texture
		SDL_DestroyTexture(textures[i]);//destroy texture
	}
	for (i = 0; i < 28; i++){//destroy each menu texture
		SDL_DestroyTexture(menutexture[i]);//destroy texture
	}
	free(textures);//free textures array
	free(objects);//free objects array
	ClearData();//clear all data from linked list
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
	if (menu && !message && x > 0 && x < maxside){//if in menu and not in message and in right area
		int clicked = y / (maxside / 32);//which button was clicked
		if (clicked > 2 && clicked < 29){ //if within the range of levels
			map = clicked - 3;//get clicked button
			menu = LoadFile(maps[map][1]);//load that level
			if (menu == 1){//if still in menu
				map = -1;//still has no map yet
			}
			if (map == -1){//if there is no map
				menu = 1;//go to menu
			}
		}
		if (clicked == 29){//Canceld
			menu = 0;//stop showing menu
			if (map == -1){//if there is no map
				menu = 1;//go to menu
			}
		}
	}
	else if(!pause){//if in normal play mode and not paused
		Data *data = GetLayer((double)(x) / maxside, (double)(y) / maxside, 1);//get first data object
		if (data != NULL){//if pointing at something
			objects[data->oid]->button(data);//run button function
			if (objects[data->oid]->selectable){//if selectable
				RemoveSatus();//remove status buttons
				selected = data;//set selected object
				objects[data->oid]->status(data);//run status function
			}
		}
	}


	if (message == 2){//if message is allady display'd
		message = 0;//reset message
	}

	MouseX = (double)(x) / maxside;//set x and y position of mouse from square normalised
	MouseY = (double)(y) / maxside;
	return;//exit function
}







SDL_Texture* GetTexture(const char *file){//make texture from this file
	char image[256] = RESOURCES;//folder path
	strcat(image, file);//append path
	SDL_Surface* surface = IMG_Load(image);//load surface
	if (surface == NULL){//if it could not be loaded
		printf("could not load image: %s\n", IMG_GetError());//error message
		return somethingwentwrong;//something went wrong
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);//get texture from loaded image
	if (texture == NULL){//if it could not be converted to texture
		printf("could not create texture: %s\n", SDL_GetError());//error message
		return somethingwentwrong;//something went wrong
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
	char texturetext[256];//text to display for texture
	strcpy(texturetext, text);//copy text
	strtok(texturetext, "\n");//remove newline
	SDL_Surface* surface = TTF_RenderText_Blended(font, texturetext, color);//rendere text as surface
	if (surface == NULL){//if it could not be loaded
		printf("could not load text: %s\n", TTF_GetError());//error message
		exit(EXIT_FAILURE);//exit
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);//get texture from loaded image
	if (texture == NULL){//if it could not be converted to texture
		printf("could not create texture: %s\n", SDL_GetError());//error message
		exit(EXIT_FAILURE);//exit
	}
	SDL_FreeSurface(surface);//free surface
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
	DrawIMG(textures[0], 0.5, 0.5, NULL, 2, 2, 1);//centered at twice the size of box
}












int LoadFile(const char *file){//load file in to memory. return 0 for success
	ClearData();//clear data before loading file
	char filename[256];//folder path
	if (strcmp(file, SAVE)) {//if file is not save file
		strcpy(filename, RESOURCES);//add folder path
	}
	else{//if file is save file
		strcpy(filename, "");//empty path
	}
	strcat(filename, file);//append path
	strtok(filename, "\n");//remove newline
	FILE *mapfile = fopen(filename, "r");//open map file for reading
	if (mapfile == NULL){//if map file could not be opened
		Message("Map could not be opened");//message that map couldn't be opened
		return 1;//file loading unsuccessful
	}
	//map file:
	//in order, money, landfill, speed, month, menu, pause, map, difficulty.
	//then list of x and y values with -1 as x at end (-1 is required and you only can have up to 31 values)
	//list of data objects untill end of file
	if (fscanf(mapfile, "%lf", &money) == EOF){//read money
		Message("Map could not be read");//message that map couldn't be read
		return 1;//file loading unsuccessful
	}
	if (fscanf(mapfile, "%lf", &landfill) == EOF){//read landfill
		Message("Map could not be read");//message that map couldn't be read
		return 1;//file loading unsuccessful
	}
	if (fscanf(mapfile, "%lf", &speed) == EOF){//read speed
		Message("Map could not be read");//message that map couldn't be read
		return 1;//file loading unsuccessful
	}
	if (fscanf(mapfile, "%lu", &ftime) == EOF){//read month
		Message("Map could not be read");//message that map couldn't be read
		return 1;//file loading unsuccessful
	}
	if (fscanf(mapfile, "%d", &menu) == EOF){//read menu
		Message("Map could not be read");//message that map couldn't be read
		return 1;//file loading unsuccessful
	}
	if (menu == 2) menu = 1;//need to draw menu
	if (fscanf(mapfile, "%d", &map) == EOF){//read map
		Message("Map could not be read");//message that map couldn't be read
		return 1;//file loading unsuccessful
	}
	if (map == -1){//if there is no map
		menu = 1;//go to menu
	}
	if (fscanf(mapfile, "%d", &pause) == EOF){//read pause
		Message("Map could not be read");//message that map couldn't be read
		return 1;//file loading unsuccessful
	}
	if (fscanf(mapfile, "%d", &difficulty) == EOF){//read difficulty
		Message("Map could not be read");//message that map couldn't be read
		return 1;//file loading unsuccessful
	}
	int i;//counter
	for (i = 0; i < 32; i++){
		if (fscanf(mapfile, "%lf", &path[i][0]) == EOF){//read x
			Message("Map could not be read");//message that map couldn't be read
			return 1;//file loading unsuccessful
		}
		if (path[i][0] == -1) break;//break if x is -1
		if (fscanf(mapfile, "%lf", &path[i][1]) == EOF){//read y
			Message("Map could not be read");//message that map couldn't be read
			return 1;//file loading unsuccessful
		}
	}
	while (1){
		int oid;//object id
		int frame;//which frame it is in
		double x, y;//x and y position normalised
		int path;//what element of path it is heading for.-1 for none
		long int health;//health of object
		if (fscanf(mapfile, "%d\n", &oid) == EOF){//read object id
			break;//end of data objects
		}
		if (fscanf(mapfile, "%d\n", &frame) == EOF){//read which frame it is in
			Message("Map could not be read");//message that map couldn't be read
			return 1;//file loading unsuccessful
		}
		if (fscanf(mapfile, "%lf\n", &x) == EOF){//read x position normalised
			Message("Map could not be read");//message that map couldn't be read
			return 1;//file loading unsuccessful
		}
		if (fscanf(mapfile, "%lf\n", &y) == EOF){//read y position normalised
			Message("Map could not be read");//message that map couldn't be read
			return 1;//file loading unsuccessful
		}
		if (fscanf(mapfile, "%d\n", &path) == EOF){//read path it is heading for
			Message("Map could not be read");//message that map couldn't be read
			return 1;//file loading unsuccessful
		}
		if (fscanf(mapfile, "%ld\n\n", &health) == EOF){//read health
			Message("Map could not be read");//message that map couldn't be read
			return 1;//file loading unsuccessful
		}
		AddData(oid, frame, x, y, path, health);//add that data
	}





	fclose(mapfile);//close file
	return 0;//success
}










void Save(void){//save in to save file
	FILE *mapfile = fopen(SAVE, "w");//open map file for reading
	if (mapfile == NULL){//if map file could not be opened
		printf("Savefile could not be made");//message that map couldn't be opened
		return;//file saving unsuccessful
	}
	//map file:
	//in order, money, landfill, speed, month, menu, pause, difficulty.
	//then list of x and y values with -1 as x at end (-1 is required and you only can have up to 31 values)
	//list of data objects untill end of file
	if (fprintf(mapfile, "%lf\n", money) == EOF){//write money
		printf("Savefile could not be written");//message that map couldn't be saved
		return;//file saving unsuccessful
	}
	if (fprintf(mapfile, "%lf\n", landfill) == EOF){//write landfill
		printf("Savefile could not be written");//message that map couldn't be saved
		return;//file saving unsuccessful
	}
	if (fprintf(mapfile, "%lf\n", speed) == EOF){//write speed
		printf("Savefile could not be written");//message that map couldn't be saved
		return;//file saving unsuccessful
	}
	if (fprintf(mapfile, "%lu\n", ftime) == EOF){//write month
		printf("Savefile could not be written");//message that map couldn't be saved
		return;//file saving unsuccessful
	}
	if (fprintf(mapfile, "%d\n", menu) == EOF){//write menu
		printf("Savefile could not be written");//message that map couldn't be saved
		return;//file saving unsuccessful
	}
	if (fprintf(mapfile, "%d\n", map) == EOF){//write map
		printf("Savefile could not be written");//message that map couldn't be saved
		return;//file saving unsuccessful
	}
	if (fprintf(mapfile, "%d\n", pause) == EOF){//write pause
		printf("Savefile could not be written");//message that map couldn't be saved
		return;//file saving unsuccessful
	}
	if (fprintf(mapfile, "%d\n\n", difficulty) == EOF){//write difficulty
		printf("Savefile could not be written");//message that map couldn't be saved
		return;//file saving unsuccessful
	}
	int i;//counter
	for (i = 0; i < 32; i++){
		if (fprintf(mapfile, "%lf\n", path[i][0]) == EOF){//write x
			printf("Savefile could not be written");//message that map couldn't be saved
			return;//file saving unsuccessful
		}
		if (path[i][0] == -1) break;//break if x is -1
		if (fprintf(mapfile, "%lf\n", path[i][1]) == EOF){//write y
			printf("Savefile could not be written");//message that map couldn't be saved
			return;//file saving unsuccessful
		}
	}

	Data *current;//current data pointer
	current = start;//set current to start
	while (current != NULL){//until current is null
		if (fprintf(mapfile, "%d\n", current->oid) == EOF){//write object id
			printf("Savefile could not be written");//message that map couldn't be saved
			return;//file saving unsuccessful
		}
		if (fprintf(mapfile, "%d\n", current->frame) == EOF){//write which frame it is in
			printf("Savefile could not be written");//message that map couldn't be saved
			return;//file saving unsuccessful
		}
		if (fprintf(mapfile, "%lf\n", current->x) == EOF){//write x position normalised
			printf("Savefile could not be written");//message that map couldn't be saved
			return;//file saving unsuccessful
		}
		if (fprintf(mapfile, "%lf\n", current->y) == EOF){//write y position normalised
			printf("Savefile could not be written");//message that map couldn't be saved
			return;//file saving unsuccessful
		}
		if (fprintf(mapfile, "%d\n", current->path) == EOF){//write path it is heading for
			printf("Savefile could not be written");//message that map couldn't be saved
			return;//file saving unsuccessful
		}
		if (fprintf(mapfile, "%ld\n\n", current->health) == EOF){//write health
			printf("Savefile could not be written");//message that map couldn't be saved
			return;//file saving unsuccessful
		}
		current = current->next;//set current to next
	}








	fclose(mapfile);//close file
}










void Loadmenu(void){//load map files and menu texts
	char mapfilename[256] = RESOURCES;//folder path
	strcat(mapfilename, MAP);//append path
	FILE *mapfile = fopen(mapfilename, "r");//open map file for reading
	if (mapfile == NULL){//if map file could not be opened
		printf("mapfile %s could not be opend \n", mapfilename);//send error message
		exit(EXIT_FAILURE);//exit
	}
	int i;//counter
	for (i = 0; i < 26; i++){//for each map
		if (fgets(maps[i][0], 255, mapfile) == NULL){//if map file could not be read
			printf("mapfile %s could not be read \n", mapfilename);//send error message
			exit(EXIT_FAILURE);//exit
		}
		if (fgets(maps[i][1], 255, mapfile) == NULL){//if map file could not be read
			printf("mapfile %s could not be read \n", mapfilename);//send error message
			exit(EXIT_FAILURE);//exit
		}
	}
	menutexture[0] = GetTextTexture(font_46, "Choose Level", 0, 0, 0);//Title of menu
	char text[512];
	for (i = 0; i < 26; i++){//for each map
		sprintf(text, "%d: %s", i, maps[i][0]);//start name with number then 
		menutexture[i + 1] = GetTextTexture(font_46, text, 0, 0, 0);//load text as texture
	}
	menutexture[27] = GetTextTexture(font_46, "Cancel", 0, 0, 0);//Cancel button
}










void DrawText(SDL_Texture *texture, double x, double y, SDL_Rect *rect, int center){//draw rect of texture at x and y position normalised. Null rect for whole texture. set center to 1 to center to x and y
	if (texture == NULL) {//if texture passed dosen't exist
		texture = somethingwentwrong;//set texture to something went wrong
	}
	SDL_Rect dest;
	int w, h, access;//value to fill up
	long format;
	SDL_QueryTexture(texture, &format, &access, &w, &h);//get text box size
	dest.w = (int) w;//set width and height
	dest.h = (int) h;
	dest.x = (int)(x * maxside);//set x and y
	dest.y = (int)(y * maxside);

	if (center){
		dest.x = dest.x - dest.w / 2;//set x and y centered to x and y
		dest.y = dest.y - dest.h / 2;
	}

	dest.x += baseX;//set origin to basex and y
	dest.y += baseY;

	SDL_RenderCopy(renderer, texture, rect, &dest);//draw texture

}










void DrawIMG(SDL_Texture *texture, double x, double y, SDL_Rect *rect, double w, double h, int center){//draw rect of texture at x and y position at scale from maxside normalised. Null rect for whole texture. set center to 1 to center to x and y
	if (texture == NULL) {//if texture passed dosen't exist
		texture = somethingwentwrong;//set texture to something went wrong
	}	SDL_Rect dest;
	dest.w = (int) (maxside * w);//set width and height
	dest.h = (int) (maxside * h);
	dest.x = (int)(x * maxside);//set x and y
	dest.y = (int)(y * maxside);

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
	AddTexture(GetTexture("border.png"));//0
	AddTexture(GetTextTexture(font_32, "Menu", 0, 0, 0));//1









	AddObject(MakeObject(0, 0, 0, textures[1], &Menu, &Nothing, &Nothing, 3.463 / 32, 1 / 32, 0, 0, 0, 1));//menue button 0

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










Object *MakeObject(int iw, int ih, int frames, SDL_Texture *texture, Button button, Button status, Button update, double w, double h, int center, int selectable, int damageable, long int maxhealth){//make object with that paramiters
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
	object->damageable = damageable;
	object->maxhealth = maxhealth;

	return object;//return the created object
}



















void AddData(int oid, int frame, double x, double y, int path, long int health){//adds this data at end
	Data *data = malloc(sizeof(Data));//make new data object
	data->oid = oid;//set variables
	data->frame = frame;
	data->x = x;
	data->y = y;
	data->path = path;
	data->health = health;
	if (start == NULL && end == NULL){//if both were null
		start = data;//set data to both
		end = data;
		data->last = NULL;//set fronmt and back to null
		data->next = NULL;
	}
	else if (start == end){//if there is only one element
		start->next = data;//set nest to new data
		end = data;//set end to new data
		data->last = start;//set back
		data->next = NULL;//set next to null
	}
	else{//if both first and second was different element
		end->next = data;//set next to new data
		Data *last = end;//make copy of end
		end = data;//set end to new data
		data->last = last;//set back
		data->next = NULL;//set next to null
	}

}




















void RemoveData(Data *data){//remove this data
	Data *last = data->last;//copy last and next
	Data *next = data->next;
	free(data);//free that data
	last->next = next;//set new last and next
	next->last = last;
}














void ClearData(void){//clear all data from linked list
	RemoveSatus();//remove status buttons
	while (start != NULL){//until start is null
		Data *next = start->next;//copy next
		free(start);//free memory of next
		start = next;//set start to next
	}
}












Data *GetLayer(double x, double y, int layer){//get object at that point and layer. return null if no object exists
	Data *current;//current data pointer
	current = end;//set current to end
	int l = 0;//layer
	while (current != NULL){//until current is null
		Object *object = objects[current->oid];//object for this data
		if (x > current->x && x < current->x + object->w && y > current->y && y < current->y + object->h){//if object is in that boundry
			l++;//count layer up
			if (l == layer){//if in correct layer
				return current;//return current object
			}
		}
		current = current->last;//set current to last
	}
	return NULL;
}














void RemoveSatus(void){//remove status buttonds and unselect all object
	selected = NULL;//deselect object

}















void Menu(Data *data){//go to meue
	menu = 1;//go to menue
}















void Nothing(Data *data){}//do nothing














































