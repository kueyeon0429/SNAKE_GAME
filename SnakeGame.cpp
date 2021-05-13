#include <clocale>
#include <ncurses.h>
#include <iostream>
#include <cstring>
using namespace std;

int main()
{
  setlocale(LC_ALL, "");
  initscr();
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_YELLOW);   // 팔레트1
  init_pair(2, COLOR_WHITE, COLOR_GREEN);    // 팔레트2

  //*******터미널*********
  resize_term(24, 50);
  border('|', '|', '-', '-', '+', '+', '+', '+');   // 테두리
  mvprintw(1, 1, "SnakeGame!");
  refresh();   // 화면 출력
  noecho();   // 입력한 문자 가리기
  getch();   // 문자 입력시 다음 이동

  //*******맵*********
  WINDOW *win1;
  win1 = newwin(21, 21, 2, 1);   // 맵 생성
  wbkgd(win1, '0');   // 맵 배경

  keypad(win1, TRUE);   // 특수 키 입력 가능
  curs_set(0);   // 커서 가림

  //wattron(win1, COLOR_PAIR(1));
  //mvwprintw(win1, 1, 1, "3");  // 지렁이 head
  //mvwprintw(win1, 1, 2, "4");  // 지렁이 body
  //mvwprintw(win1, 1, 3, "4");  // 지렁이 body
  //mvwprintw(win1, 1, 1, "\u2B1B");  // 지렁이 head 유니코드
  //mvwprintw(win1, 1, 3, "\u2B1C");  // 지렁이 body 유니코드
  //wattroff(win1, COLOR_PAIR(1));

  wattron(win1, COLOR_PAIR(2));
  wborder(win1, '1', '1', '1', '1', '2', '2', '2', '2');
  wattroff(win1, COLOR_PAIR(2));

 // **** 2동****
  int y = 10, x = 10;
  wattron(win1, COLOR_PAIR(1));
  mvwprintw(win1, y, x, "3");    // 왜 이 처음 위치가 안 뜨는지 몰겠음;;
  wattroff(win1, COLOR_PAIR(1));

  wrefresh(win1);

  int pre = KEY_RIGHT;  // default 방향
  while(1){
      int input = wgetch(win1);  // 방향키 입력 받음
      // 초기화 및 배경 유지
      wclear(win1);
      wbkgd(win1, '0');
      wattron(win1, COLOR_PAIR(2));
      wborder(win1, '1', '1', '1', '1', '2', '2', '2', '2');
      wattroff(win1, COLOR_PAIR(2));
      // 이동
      wattron(win1, COLOR_PAIR(1));
      if(pre == KEY_LEFT){   // head방향: 좌
        switch(input){
            case KEY_UP:
            mvwprintw(win1, --y, ++x, "3");
            pre = input;  // head방향(=pre) 갱신
            continue;
            case KEY_DOWN:
            mvwprintw(win1, ++y, ++x, "3");
            pre = input;  // head방향(=pre) 갱신
            continue;
            case KEY_LEFT:
            continue;
            case KEY_RIGHT:
            break;
        }
      }
      else if(pre == KEY_RIGHT){
        switch(input){
            case KEY_UP:
            mvwprintw(win1, --y, --x, "3");
            pre = input;  // head방향(=pre) 갱신
            continue;
            case KEY_DOWN:
            mvwprintw(win1, ++y, --x, "3");
            pre = input;  // head방향(=pre) 갱신
            continue;
            case KEY_LEFT:
            break;
            case KEY_RIGHT:
            continue;
        }
      }
      else if(pre == KEY_UP){
        switch(input){
            case KEY_UP:
            continue;
            case KEY_DOWN:
            break;
            case KEY_LEFT:
            mvwprintw(win1, ++y, --x, "3");
            pre = input;  // head방향(=pre) 갱신
            continue;
            case KEY_RIGHT:
            mvwprintw(win1, ++y, ++x, "3");
            pre = input;  // head방향(=pre) 갱신
            continue;
        }
      }
      else if(pre == KEY_DOWN){
        switch(input){
            case KEY_UP:
            break;
            case KEY_DOWN:
            continue;
            case KEY_LEFT:
            mvwprintw(win1, --y, --x, "3");
            pre = input;  // head방향(=pre) 갱신
            continue;
            case KEY_RIGHT:
            mvwprintw(win1, --y, ++x, "3");
            pre = input;  // head방향(=pre) 갱신
            continue;
        }
      }
      /*
      switch(input){
          case KEY_UP:
          mvwprintw(win1, --row, col, "3");
          continue;
          case KEY_DOWN:
          mvwprintw(win1, ++row, col, "3");
          continue;
          case KEY_LEFT:
          mvwprintw(win1, row, --col, "3");
          continue;
          case KEY_RIGHT:
          mvwprintw(win1, row, ++col, "3");
          continue;
      }
      if(input == 'q') break;
      */
      wattroff(win1, COLOR_PAIR(1));
  }

  getch();
  delwin(win1);
  endwin();

  return 0;
}
