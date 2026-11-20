/**
  * # The infernal Canyon 
  * fire.c
 */
#ifndef INFERNAL_CANYON_FIRE
#define INFERNAL_CANYON_FIRE

#include <ncurses.h>
#include <stdlib.h>

#include "./constants.c"
#include "./colors.c"

struct Cell {
	int heat;
	int newHeat;
};

struct Fire {
	struct Cell * * structure;
	int max_fire;
	double intensity;
	double cooling;
		
};

struct Fire * fire_create(void){
	struct Fire * fire = malloc(sizeof(struct Fire));
	fire->structure = malloc(sizeof(struct Cell *)*LINES);
	for (int i = 0; i < LINES; i++){
		fire->structure[i] = malloc(sizeof(struct Cell)*COLS);	
		for (int j = 0; j < COLS; j++){
			fire->structure[i][j].heat = 0;
			fire->structure[i][j].newHeat = 0;
		}
	}
	fire->intensity = INTENSITY;
	fire->cooling = COOLING;
	return fire;
}

void fire_destroy(struct Fire * fire){
	for (int i = 0; i < LINES; i++){
		free(fire->structure[i]);
	}
	free(fire->structure);
	free(fire);
	return;
}

/*Fire visual*/
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

int average_heat_neighbours(struct Fire * fire, int x, int y){
	return (
		fire->structure[y+1][x].heat
		+ fire->structure[y+1][x-1].heat
		+ fire->structure[y+1][x+1].heat
		+ fire->structure[y][x-1].heat
		+ fire->structure[y][x+1].heat
	)/5;
}

/* Fire generation */
void seed_fire(struct Fire * fire){
	for (int i = 0; i < fire->intensity*COLS; i++){
		fire->structure[LINES-1][rand()%COLS].newHeat = rand()%MAX_FIRE;
	}
	return;
}

void cooling_fire(struct Fire * fire){
	for (int i = 0; i < fire->cooling*COLS; i++){
		fire->structure[LINES-1][rand()%COLS].newHeat = 0;
	}
}

void propagating_fire(struct Fire * fire){
	for (int i = 0; i < LINES-1; i++){
		for (int j = 1; j < COLS-1; j++){
			fire->structure[i][j].newHeat = average_heat_neighbours(fire, j, i);	
		}
	}
}

void update_heat_fire(struct Fire * fire){
	for (int i = 0; i < LINES; i++){
		for (int j = 1; j < COLS; j++){
			fire->structure[i][j].heat = fire->structure[i][j].newHeat;
		}
	}
}

void update_fire_logic(struct Fire * fire){
	// Seeding the fire
	seed_fire(fire);
	// Cooling
	cooling_fire(fire);
	// Fire propagation	
	propagating_fire(fire);	
	// Update the state to their real heat
	update_heat_fire(fire);
	return;
}

#endif
