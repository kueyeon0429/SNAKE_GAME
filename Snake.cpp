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
#include "snake.h"

SNAKE snake;

void SnakeInit();
void Map(WINDOW *win);
void Turn();
void move();
void SnakeFirst(WINDOW *win);
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

//방향키 받는 함수
void press() {
  if (kbhit()) {
    snake.ch = getch();
  }
}

// 윈도우 초기화
void WinInit(WINDOW *win){
   // 맵 생성
  wbkgd(win, COLOR_PAIR(1));   // 맵 배경
  keypad(stdscr, TRUE);   // 특수 키 입력 가능
  curs_set(0);   // 커서 가림
  wattron(win, COLOR_PAIR(2));
  wborder(win, '|','|','-','-','+','+','+','+');
  wattroff(win, COLOR_PAIR(2));
  wrefresh(win);
}

// snake 초기화
void SnakeInit(){
  snake.y = 10;   // 처음 위치 초기화
  snake.x = 10;
  snake.length = 3;   // 길이 초기화
  //snake.head = KEY_LEFT;   // 방향 초기화

  //뱀의 시작 지점
  vector <int> vec1;     //임시 1차원 벡터
  vec1.push_back(snake.y);
  vec1.push_back(snake.x);
  snake.vec.push_back(vec1);
  vec1[1]++;
  snake.vec.push_back(vec1);
  vec1[1]++;
  snake.vec.push_back(vec1);
}

// 초기 snake 출력
void SnakeFirst(WINDOW *win) {
  wattron(win, COLOR_PAIR(1));
  for (int i=0; i<snake.length; i++) {
      mvwprintw(win, snake.vec[i][0], snake.vec[i][1], "3");
  }
  wattroff(win, COLOR_PAIR(1));
  wrefresh(win);
}

// 뱀 임시 삭제
void deletesnake(WINDOW *win) {
  wattron(win, COLOR_PAIR(1));
  for (int i=0; i<snake.length; i++) {
    mvwprintw(win, snake.vec[i][0], snake.vec[i][1], "0");
  }
  wattroff(win, COLOR_PAIR(1));
}

// 뱀 출력
void printsnake(WINDOW *win) {
  wattron(win, COLOR_PAIR(1));
  for (int i=0; i<snake.length; i++) {
    mvwprintw(win, snake.vec[i][0], snake.vec[i][1], "3");
  }
  wattroff(win, COLOR_PAIR(1));
  wrefresh(win);
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

// 화면 출력
//void Render(WINDOW *win){
//    wrefresh(win);
//}

//맵 생성
void Map(WINDOW *win) {
  string mapstring =
  "211111111111111111112100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001100000000000000000001211111111111111111112";
  int mapline = 21;  //맵 줄 수
  int num = 0;   //임시변수

  //2차원 배열을 생성하여 맵을 2차원 배열에 저장
  int **map = new int *[mapline];
  for (int i=0; i<mapline; i++)
    map[i] = new int[mapline];
  for (int i = 0; i < mapline; i++)
    for (int j = 0; j < mapline; j++)
      map[i][j] = mapstring[num++] - '0';

  //윈도우에 맵 출력
  wattron(win, COLOR_PAIR(1));
  for (int i=0; i<mapline; i++) {
    for (int j=0; j<mapline; j++) {
      mvwprintw(win, i+1, j+1, to_string(map[i][j]).c_str());
    }
  }
  wattroff(win, COLOR_PAIR(1));
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

void time() {


}


int main(){
  WINDOW *win;

  OptionInit();
  TermInit();
  WinInit(win);

  win = newwin(23, 23, 1, 1);
  SnakeInit(); // snake 초기화
  Map(win); //맵 생성
  SnakeFirst(win); // 초기 snake 출력
  while (1) {          //무한 반복
    usleep(500000);    //0.5초 대기
    press();           //방향키 입력
    deletesnake(win); //이전 뱀 출력된거 삭제
    Turn();         //머리 방향 초기화
    move();         //머리 위치 이동
    StateUpdate();
    printsnake(win); //새로운 뱀 출력
  }
  delwin(win);
  endwin();
  return 0;
}
