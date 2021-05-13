#include <ncurses.h>

int main()
{
  WINDOW *win1;

  initscr();
  resize_term(100, 100);
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_YELLOW);
  init_pair(2, COLOR_WHITE, COLOR_GREEN);


  border('|', '|', '-', '-', '+', '+', '+', '+');
  mvprintw(1, 1, "Presh any button >> SnakeGame!");
  refresh();
  getch();

  win1 = newwin(51, 51, 2, 1);
  wbkgd(win1, '0');
  wattron(win1, COLOR_PAIR(1));
  mvwprintw(win1, 1, 1, "3");  // 지렁이
  mvwprintw(win1, 1, 2, "4");  // 지렁이
  mvwprintw(win1, 1, 3, "5");  // 지렁이
  wattroff(win1, COLOR_PAIR(1));

  wattron(win1, COLOR_PAIR(2));
  wborder(win1, '1', '1', '1', '1', '2', '2', '2', '2');
  wattroff(win1, COLOR_PAIR(2));

  wrefresh(win1);

  getch();
  delwin(win1);
  endwin();

  return 0;
}
