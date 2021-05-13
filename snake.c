#include <clocale>
#include <ncurses.h>
#include <iostream>
#include <ctime>
#include "snake.h"
using namespace std;

const HALF_SECOND = 0.5;

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
  resize_term(24, 50);
  border('|', '|', '-', '-', '+', '+', '+', '+');   // 테두리
  mvprintw(1, 1, "SnakeGame!");
  refresh();   // 화면 출력
  noecho();   // 입력한 문자 가리기
  getch();   // 문자 입력시 다음 이동
}

// 윈도우 초기화
void WinInit(){
  WINDOW *win1;
  win1 = newwin(21, 21, 2, 1);   // 맵 생성
  wbkgd(win1, '0');   // 맵 배경

  keypad(win1, TRUE);   // 특수 키 입력 가능
  curs_set(0);   // 커서 가림

  wattron(win1, COLOR_PAIR(2));
  wborder(win1, '1', '1', '1', '1', '2', '2', '2', '2');
  wattroff(win1, COLOR_PAIR(2));
}

// sanke 초기화
void SnakeInit(){
  snake.y = 10;   // 처음 위치 초기화
  snake.x = 10;
  sanke.length = 3;   // 길이 초기화
  wattron(win1, COLOR_PAIR(1));
  mvwprintw(win1, y, x, "3");
  wattroff(win1, COLOR_PAIR(1));

  wrefresh(win1);
}

// 상태 업데이트
void StateUpdate(){

}

// 화면 출력
void Render(WINDOW *win){
    wrefresh(win);
}



int main(){
  OptionInit();
  TermInit();
  WinInit();

  while(1){
    Update();
    Render(win1);
  }

}
