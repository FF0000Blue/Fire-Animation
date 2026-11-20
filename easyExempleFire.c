#include <ncurses.h>
#include <stdint.h>

#include "./colors.c"
#include "./fireModule.c"

const char KEY_QUIT = 'q';

const int UPDATE_DELAY = 20;

void print_fire(struct Fire * fire, struct Palette palette){	
	for (int i = 0; i < LINES; i++){
		for (int j = 0; j < COLS; j++){
			mvprintw(i, j, "%c", get_char_fire(fire->structure[i][j].heat, palette));
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

void update_screen(struct Fire * fire, struct Palette palette){
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
  
  struct Fire * fire = fire_create();
  struct Palette palette = palette_create();
  
  int input;
  
  while (input != KEY_QUIT) {  
    input = getch();  

    timeout(UPDATE_DELAY);
    
    update_fire_logic(fire);
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
