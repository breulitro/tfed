#ifdef __cplusplus
#include <cstdlib>	// para usar atexit, exit, etc.
#else
#include <stdlib.h>   // idem
#endif
#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif
#include <iostream>

using namespace std;

SDL_Joystick *joy;

void myExit() {
	// Close if opened
	if(SDL_JoystickOpened(0))
		SDL_JoystickClose(joy);
	SDL_Quit();
}

int main(int argc, char *argv[]) {
	// Tela de desenho
	SDL_Surface* screen;
	//  "Superfície" onde sera carregada a imagem
	SDL_Surface* img; 

	// Inicializa a SDL
	SDL_Init(SDL_INIT_EVERYTHING); 
	// Indica que a função SDL_Quit será chamada na finalização do programa
	atexit(myExit);

	// Setup da tela: largura, altura, bits por pixel, e modo de rendering
	// SDL_HWSURFACE => usar memória da placa de vídeo
	// SDL_DOUBLEBUF => usar double buffering (para tela não "piscar")
	screen = SDL_SetVideoMode(640,480,32, SDL_HWSURFACE | SDL_DOUBLEBUF);

	// Carrega uma imagem
	img = SDL_LoadBMP("sdl_logo.bmp");

	// Região onde será desenhada a imagem
	SDL_Rect area;
	area.x = 10; // canto esquerdo (x)
	area.y = 20; // canto superior (y)
	area.w = img->w; // largura
	area.h = img->h; // altura

	// Limpa a tela
	SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));

	// Desenha a imagem na área definida
	SDL_BlitSurface(img, NULL, screen, &area);

	// Exibe resultado na tela
	SDL_Flip(screen);
	bool fim = false;

	int i;
	printf("%i joysticks were found.\n\n", SDL_NumJoysticks() );
	printf("The names of the joysticks are:\n");

	for( i=0; i < SDL_NumJoysticks(); i++ ) 
	{
		printf("    %s\n", SDL_JoystickName(i));
	}

	// Check for joystick
	if(SDL_NumJoysticks()>0){
		// Open joystick
		joy=SDL_JoystickOpen(0);

		if(joy)
		{
			printf("Opened Joystick 0\n");
			printf("Name: %s\n", SDL_JoystickName(0));
			printf("Number of Axes: %d\n", SDL_JoystickNumAxes(joy));
			printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joy));
			printf("Number of Balls: %d\n", SDL_JoystickNumBalls(joy));

			cout << "Number of hats: " << SDL_JoystickNumHats(joy) << endl;
		}
		else
			printf("Couldn't open Joystick 0\n");

	}

	int velx = 0, vely = 0;

	while (!fim) {
		// http://www.libsdl.org/docs/html/sdlevent.html
		SDL_Event evento;
		// Enquanto houver eventos para processar...
		 while (SDL_PollEvent(&evento)) {
		// verifica o tipo de evento
			switch (evento.type) {
				// Finaliza se a janela for fechada
				case SDL_QUIT:
					fim = true;
					break;

				// Verifica eventos de teclado
				case SDL_KEYDOWN:
					// Finaliza se ESC for pressionada
					if (evento.key.keysym.sym == SDLK_ESCAPE)
						fim = true;
					break;
				
				case SDL_JOYHATMOTION:
					if (evento.jhat.value == SDL_HAT_CENTERED)
						velx = vely = 0;
					if (evento.jhat.value & SDL_HAT_RIGHT)
						velx = 2;
					if (evento.jhat.value & SDL_HAT_LEFT)
						velx = -2;
					if (evento.jhat.value & SDL_HAT_UP)
						vely = -2;
					if (evento.jhat.value & SDL_HAT_DOWN)
						vely = 2;
			}
		}
		
		area.x += velx;
		area.y += vely;
		// Limpa a tela
		SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));
		// Desenha a imagem na área definida
		SDL_BlitSurface(img, NULL, screen, &area);

		// Exibe resultado na tela
		SDL_Flip(screen);
	}
}

