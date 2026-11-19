#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <ncurses.h>
#include <assert.h>

const int UPDATE_DELAY = 20;

const char KEY_QUIT = 'q';

const double INTENSITY = 0.5;
const double COOLING = 0.25;

const int MAX_FIRE = 100;

const int COLOR_BACKGROUND = COLOR_BLACK;

struct Palette {
	int weak_fire;
	int mid_fire;
	int strong_fire;
	int background;
	int ashes;
};

struct Palette palette_create(void) {
	init_pair(1, COLOR_WHITE, COLOR_BACKGROUND); // Feu fort
	init_pair(2, COLOR_RED, COLOR_BACKGROUND); // Feu faible
	init_pair(3, COLOR_YELLOW, COLOR_BACKGROUND); // Feu moyen
	init_pair(4, COLOR_BACKGROUND, COLOR_BACKGROUND);
	return (struct Palette) {
	  .strong_fire = 1,
	  .mid_fire = 3,
	  .weak_fire = 2,
	  .background = 4,
	};
}

struct Cell {
	int heat;
};

struct Fire {
	struct Cell * * structure;
};

struct Fire fire_create(void){
	struct Fire fire;
	fire.structure = malloc(sizeof(struct Cell *)*LINES);
	for (int i = 0; i < LINES; i++){
		fire.structure[i] = malloc(sizeof(struct Cell)*COLS);	
		for (int j = 0; j < COLS; j++){
			fire.structure[i][j].heat = 0;
		}
	}
	return fire;
}

void fire_destroy(struct Fire fire){
	for (int i = 0; i < LINES; i++){
		free(fire.structure[i]);
	}
	free(fire.structure);
	return;
}

char get_char_fire(int heat, struct Palette palette){
	// COLOR
	if (heat > 0.4*MAX_FIRE){
		attron(COLOR_PAIR(palette.strong_fire));
	}
	else if (heat > 0.2*MAX_FIRE){
		attron(COLOR_PAIR(palette.mid_fire));
	}
	else if (heat > 0.15*MAX_FIRE){
		attron(COLOR_PAIR(palette.weak_fire));
	}
	else {
		attron(COLOR_PAIR(palette.background));
	}
	
	// CHAR
	if (heat > 0.4*MAX_FIRE){
		return '@';
	}
	if (heat > 0.25*MAX_FIRE){
		return '#';
	}
	if (heat > 0.18*MAX_FIRE){
		return 'X';
	}
	if (heat > 0.09*MAX_FIRE){
		return 'x';
	}
	return '+';
	
}

void print_fire(struct Fire fire, struct Palette palette){
	
	for (int i = 0; i < LINES; i++){
		for (int j = 0; j < COLS; j++){
			mvprintw(i, j, "%c", get_char_fire(fire.structure[i][j].heat, palette));
		}
	}
	return;
}

void clean_screen(struct Palette palette){
	attron(COLOR_PAIR(palette.background));
	for (int i = 0; i < LINES; i++){
		for (int j = 0; j < COLS; j++){
			mvprintw(i, j, " ");
		}
	}	
	return;
}

int average_heat_neighbours(struct Fire fire, int x, int y){
	int sum = fire.structure[y+1][x].heat;
	sum += fire.structure[y+1][x-1].heat;
	sum += fire.structure[y+1][x+1].heat;
	sum += fire.structure[y][x-1].heat;
	sum += fire.structure[y][x].heat;
	return sum/5;
}

void update_logic(struct Fire fire){
	// Seeding the fire
	for (int i = 0; i < INTENSITY*COLS; i++){
		fire.structure[LINES-1][rand()%COLS].heat = rand()%MAX_FIRE;
	}
	
	// Cooling
	for (int i = 0; i < COOLING*COLS; i++){
		fire.structure[LINES-1][rand()%COLS].heat = 0;
	}
	
	// Fire propagation	
	int * * new_fire_structure = malloc(sizeof(int *)*(LINES-1));
	
	for (int i = 0; i < LINES-1; i++){
		new_fire_structure[i] = malloc(sizeof(int)*(COLS-1));
		for (int j = 1; j < COLS-1; j++){
			new_fire_structure[i][j] = average_heat_neighbours(fire, j, i);	
		}
	}
	for (int i = 0; i < LINES-1; i++){
		for (int j = 1; j < COLS-1; j++){
			fire.structure[i][j].heat = new_fire_structure[i][j];
		}
		free(new_fire_structure[i]);
	}
	free(new_fire_structure);
	return;
}

void update_screen(struct Fire fire, struct Palette palette){
	clean_screen(palette);
	print_fire(fire, palette);
	return;
}

void run(void) {
  initscr();
  noecho();
  start_color();
  curs_set(0);
  refresh();
  nodelay(stdscr, TRUE);
  
  struct Fire fire = fire_create();
  struct Palette palette = palette_create();
  
  int input;
  
  while (input != KEY_QUIT) {  
    input = getch();
    timeout(UPDATE_DELAY);
    
    update_logic(fire);
    update_screen(fire, palette);
  }
  
  fire_destroy(fire);
  
  endwin();
}

int main(void) {
  printf("# Fire animation (I hope)\n");
	printf("## Keys. \n");
	printf("Quit : %c\n", KEY_QUIT);

  run();

  return EXIT_SUCCESS;
}
