#include <clocale>
#include <ncurses.h>
#include <iostream>
#include <ctime>
#include "snake.h"
//#include "kbhit.c"
using namespace std;

const double HALF_SECOND = 0.5;

SNAKE snake;

// 옵션 초기화
void OptionInit(){
  setlocale(LC_ALL, "");
  initscr();
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_YELLOW);   // 팔레트1
  init_pair(2, COLOR_WHITE, COLOR_GREEN);    // 팔레트2
}

// 터미널 초기화
void TermInit(){
  resize_term(50, 50);
  border('|', '|', '-', '-', '+', '+', '+', '+');   // 테두리
  refresh();   // 화면 출력
  noecho();   // 입력한 문자 가리기
  //getch();   // 문자 입력시 다음 이동
}

// 윈도우 초기화
void WinInit(WINDOW *win){
  win = newwin(30, 30, 1, 1);   // 맵 생성
  wbkgd(win, COLOR_PAIR(1));   // 맵 배경

  keypad(stdscr, TRUE);   // 특수 키 입력 가능
  curs_set(0);   // 커서 가림

  wattron(win, COLOR_PAIR(2));
  wborder(win, '|','|','-','-','+','+','+','+');
  wattroff(win, COLOR_PAIR(2));
  wrefresh(win);
}

// sanke 초기화
void SnakeInit(WINDOW *win){
  snake.y = 10;   // 처음 위치 초기화
  snake.x = 10;
  snake.length = 3;   // 길이 초기화
  snake.head = KEY_RIGHT;   // 방향 초기화


  wattron(win, COLOR_PAIR(1));
  mvwprintw(win, 10, 10, "3");
  wattroff(win, COLOR_PAIR(1));
  wrefresh(win);

}

// 방향키 받는 함수
/*
int Head(){
  if(kbhit() == true)
    return getch();
}
*/

// 회전 ---------------완전수정해야댐,,
void Turn(WINDOW *win){
  wattron(win, COLOR_PAIR(2));
  int ch=KEY_LEFT;
  int a=0;
  char key[1];
  int direct = 1;
  clock_t t = clock();
  int b = 0;
  while (1) {
      t = clock();
      while ((clock() - t) <= 2000000) {
        //cout << clock() - t << endl;
        if (b==0) {
          ch = getch();
          b=1;
        }
      }
      if (ch == KEY_LEFT) {
        mvwprintw(win, snake.y, snake.x, "0");
        snake.x--;
      }
      else if (ch == KEY_RIGHT) {
        mvwprintw(win, snake.y, snake.x, "O");
        snake.x++;
      }
      else if (ch == KEY_UP) {
        mvwprintw(win, snake.y, snake.x, "0");
        snake.y--;
      }
      else if (ch == KEY_DOWN) {
        mvwprintw(win, snake.y, snake.x, "0");
        snake.y++;
      }
      b=0;
      mvwprintw(win, snake.y, snake.x, "3");
      wrefresh(win);
  }
  wattroff(win, COLOR_PAIR(2));
  wrefresh(win);
}

// 상태 업데이트
void StateUpdate(int head, clock_t curr){

}

// 화면 출력
//void Render(WINDOW *win){
//    wrefresh(win);
//}

void Map(WINDOW *win) {
  string mapstring =
  "211111111111111111112100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001211111111111111111112";
  int mapline = 21;
  int num = 0;
  int **map = new int *[mapline];
  for (int i=0; i<mapline; i++)
    map[i] = new int[mapline];
  for (int i = 0; i < mapline; i++)
    for (int j = 0; j < mapline; j++)
      map[i][j] = mapstring[num++] - '0';
  wattron(win, COLOR_PAIR(1));
  for (int i=0; i<mapline; i++) {
    for (int j=0; j<mapline; j++) {
      mvwprintw(win, i+1, j+1, to_string(map[i][j]).c_str());
    }
  }
  wattroff(win, COLOR_PAIR(1));
  wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
  wrefresh(win);

  wattron(win, COLOR_PAIR(2));
  mvwprintw(win, 10, 10, "X");
  wattroff(win, COLOR_PAIR(2));
  wrefresh(win);
}



int main(){
  WINDOW *win;
  OptionInit();
  TermInit();
  WinInit(win);
  SnakeInit(win);
  Map(win);

  //while(1){
  //  Turn(win);
  //}
  getch();
  delwin(win);
  endwin();

  /*
  OptionInit();
  TermInit();
  WinInit();
  SnakeInit();

  while(1){
    snake.head = Head();
    clock_t cuttTime = clock();
    StateUpdate(snake_head, currTime);
    wrefresh(snake_win)
  }

  getch();
  delwin(snake_win);
  endwin();

  return 0;
  */
  return 0;
}
