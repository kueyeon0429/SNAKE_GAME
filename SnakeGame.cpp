#include <ncurses.h>

int main()
{
  WINDOW *win1;

  initscr();
  resize_term(50, 50);
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_YELLOW);
  init_pair(2, COLOR_WHITE, COLOR_GREEN);


  border('|', '|', '-', '-', '+', '+', '+', '+');
  mvprintw(1, 1, "SnakeGame!");
  refresh();
  getch();

  win1 = newwin(21, 21, 2, 1);
  wbkgd(win1, '0');
  wattron(win1, COLOR_PAIR(1));
  mvwprintw(win1, 1, 1, "3");  // 지렁이 head
  mvwprintw(win1, 1, 2, "4");  // 지렁이 body
  mvwprintw(win1, 1, 3, "4");  // 지렁이 body
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
