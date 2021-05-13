#include <clocale>
#include <ncurses.h>
#include <iostream>
#include <ctime>

using namespace std;

int main() {
  string mapstring = "211111111111111111112100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001211111111111111111112";
  int mapline = 21;
  int num = 0;
  int **map = new int *[mapline];
  for (int i=0; i<mapline; i++)
    map[i] = new int[mapline];
  for (int i = 0; i < mapline; i++)
    for (int j = 0; j < mapline; j++)
      map[i][j] = mapstring[num++] - '0';
  int *head = new int[2];
  head[0] = 10;  //head row
  head[1] = 10;  //head col

  setlocale(LC_ALL, "");
  WINDOW *win1;

  initscr();
  resize_term(50, 50);

  keypad(stdscr, TRUE);
  curs_set(0);
  noecho();

  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_WHITE, COLOR_BLACK);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);

  attron(COLOR_PAIR(1));
  //mvprintw(1, 2, "\u2B1B");

  attroff(COLOR_PAIR(1));
  border('|','|','-','-','+','+','+','+');
  refresh();

  win1 = newwin(23, 23, 1, 1);
  wbkgd(win1, COLOR_PAIR(2));
  wattron(win1, COLOR_PAIR(3));
  for (int i=0; i<mapline; i++) {
    for (int j=0; j<mapline; j++) {
      mvwprintw(win1, i+1, j+1, to_string(map[i][j]).c_str());
    }
  }
  wattroff(win1, COLOR_PAIR(3));
  wborder(win1, '|', '|', '-', '-', '+', '+', '+', '+');
  wattron(win1, COLOR_PAIR(2));
  wattroff(win1, COLOR_PAIR(2));
  wrefresh(win1);

  wattron(win1, COLOR_PAIR(3));
  mvwprintw(win1, 10, 10, "X");
  wattroff(win1, COLOR_PAIR(3));
  wrefresh(win1);

  wattron(win1, COLOR_PAIR(3));
  int ch=KEY_LEFT;
  int a=0;
  char key[1];
  int direct = 1;
  clock_t t = clock();
/*
  while (true) {
    t = clock();
    while(((clock() - t) <= 1000000)==true) {
      if (a==0) {
        mvwprintw(win1, head[0], head[1], "X");
        head[1]--;
        a=1;
      }
    }
    a=0;
    wrefresh(win1);
  }
*/

  while (true) {
    t = clock();
    while (((clock() - t) <= 1000000)==true) {
        if (a==0) {
          ch = getch();
          if (ch == KEY_LEFT) {
            direct = 1;
            mvwprintw(win1, head[0], head[1], "X");
            head[1]--;
            a=1;
          }
          else if (ch == KEY_RIGHT) {
            direct = 2;
            mvwprintw(win1, head[0], head[1], "X");
            head[1]++;
            a=1;
          }
          else if (ch == KEY_UP) {
            direct = 3;
            mvwprintw(win1, head[0], head[1], "X");
            head[0]--;
            a=1;
          }
          else if (ch == KEY_DOWN) {
            direct = 4;
            mvwprintw(win1, head[0], head[1], "X");
            head[0]++;
            a=1;
          }
          mvwprintw(win1, head[0], head[1], "O");
        }
      }
       if (a==0) {
        if (direct == 1) {
          mvwprintw(win1, head[0], head[1], "X");
          head[1]--;
          a=1;
        }
        else if (direct == 2) {
          mvwprintw(win1, head[0], head[1], "X");
          head[1]++;
          a=1;
        }
        else if (direct == 3) {
          mvwprintw(win1, head[0], head[1], "X");
          head[0]--;
          a=1;
        }
        else if (direct == 4) {
          mvwprintw(win1, head[0], head[1], "X");
          head[0]++;
          a=1;
        }
      }
      a=0;
      wrefresh(win1);
    }


  wattroff(win1, COLOR_PAIR(3));

  getch();
  delwin(win1);
  endwin();

  return 0;
}
