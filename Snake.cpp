#include <clocale>
#include <ncurses.h>
#include <iostream>
#include <ctime>
#include <unistd.h> //sleep
#include <termios.h> // kbhit()
#include <sys/ioctl.h> // kbhit()
#include <vector>
//#include "kbhit.c"
using namespace std;
#include "Snake.h"

SNAKE snake;

void SnakeInit(WINDOW *win);
void Map(WINDOW *win);
void Turn();
void move();
void StateUpdate();
void deletesnake(WINDOW *win);
void printsnake(WINDOW *win);

int kbhit() {
    termios term;
    tcgetattr(0, &term);
    termios term2 = term;
    term2.c_lflag &= ~ICANON;
    tcsetattr(0, TCSANOW, &term2);
    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);
    tcsetattr(0, TCSANOW, &term);
    return byteswaiting > 0;
}

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
void WinInit(){
  snake.win = newwin(23, 23, 1, 1);
   // 맵 생성
  wbkgd(snake.win, '0');   // 맵 배경
  keypad(stdscr, TRUE);   // 특수 키 입력 가능
  curs_set(0);   // 커서 가림
  wattron(snake.win, COLOR_PAIR(2));
  wborder(snake.win, '1','1','1','1','2','2','2','2');
  wattroff(snake.win, COLOR_PAIR(2));
  //snake.head = KEY_LEFT;   // 방향 초기화

}

// snake 초기화
void SnakeInit(){
  snake.fail = true;
  snake.y = 10;   // 처음 위치 초기화
  snake.x = 10;
  snake.length = 8;   // 길이 초기화
  //snake.head = KEY_LEFT;   // 방향 초기화

  //뱀의 시작 지점
  vector <int> vec1;     //임시 1차원 벡터
  vec1.push_back(snake.y);
  vec1.push_back(snake.x);
  snake.vec.push_back(vec1);
  for(int i = 0; i < snake.length-1; i++){
    vec1[1]++;
    snake.vec.push_back(vec1);
  }

  //초기 뱀 출력
  wattron(snake.win, COLOR_PAIR(1));
  mvwprintw(snake.win, snake.vec[0][0], snake.vec[0][1], "3");
  for (int i=1; i<snake.length; i++) {
      mvwprintw(snake.win, snake.vec[i][0], snake.vec[i][1], "4");
  }
  wattroff(snake.win, COLOR_PAIR(1));
  wrefresh(snake.win);
}

// 뱀 임시 삭제
void deletesnake() {
  for (int i=0; i<snake.length; i++) {
    mvwprintw(snake.win, snake.vec[i][0], snake.vec[i][1], "0");
  }
}

// 뱀 출력
void printsnake() {
  wattron(snake.win, COLOR_PAIR(1));
  mvwprintw(snake.win, snake.vec[0][0], snake.vec[0][1], "3");
  for (int i=1; i<snake.length; i++) {
    mvwprintw(snake.win, snake.vec[i][0], snake.vec[i][1], "4");
  }
  wattroff(snake.win, COLOR_PAIR(1));
  wrefresh(snake.win);
}

//방향키 받는 함수
void press() {
  if (kbhit()) {
    snake.ch = getch();
  }
}

//머리 방향 초기화
void Turn(){
  if (snake.ch == KEY_LEFT && snake.head != KEY_RIGHT) {
    snake.head = KEY_LEFT;
  }
  else if (snake.ch == KEY_RIGHT && snake.head != KEY_LEFT) {
    snake.head = KEY_RIGHT;
  }
  else if (snake.ch == KEY_UP && snake.head != KEY_DOWN) {
    snake.head = KEY_UP;
  }
  else if (snake.ch == KEY_DOWN && snake.head != KEY_UP) {
    snake.head = KEY_DOWN;
  }
}

// 상태 업데이트
void StateUpdate(){
  vector <int> vec1;     //임시 1차원 벡터
  vec1.push_back(snake.y);
  vec1.push_back(snake.x);
  snake.vec.insert(snake.vec.begin(),vec1);
  snake.vec.pop_back();
}

//머리 위치 이동
void move() {
    if (snake.head == KEY_LEFT) {
      snake.x--;
    }
    else if (snake.head == KEY_RIGHT) {
      snake.x++;
    }
    else if (snake.head == KEY_UP) {
      snake.y--;
    }
    else if (snake.head == KEY_DOWN) {
      snake.y++;
    }
}

// 실패 조건 판단
void fail(){
  // wall에 닿는 경우
  if(snake.x == 0 || snake.x == 22 || snake.y == 0 || snake.y == 22) snake.fail = false;
  // body에 닿는 경우
  for (int i=4; i < snake.length; i++){
    if(snake.y == snake.vec[i][0] && snake.x == snake.vec[i][1]){
      snake.fail = false;
      break;
    }
  }
}


/****************main*******************/
int main(){
  OptionInit();
  TermInit();
  WinInit();

  bool t = true;

  SnakeInit(); // snake 초기화
  while(snake.fail) {          //무한 반복
    usleep(500000);    //0.5초 대기
    press();           //방향키 입력
    deletesnake(); //이전 뱀 출력된거 삭제
    Turn();         //머리 방향 초기화
    move();         //머리 위치 이동
    StateUpdate();
    fail();   // 실패 조건 판단
    printsnake(); //새로운 뱀 출력
  }
  delwin(snake.win);
  endwin();
  return 0;
}
