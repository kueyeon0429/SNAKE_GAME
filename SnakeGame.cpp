#include <ncurses.h>

int main()
{
  initscr();
  resize_term(100, 100);
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_RED);

  border('*', '*', '*', '*', '*', '*', '*', '*');

  getch();
  delwin(win1);
  endwin();

  return 0;
}
