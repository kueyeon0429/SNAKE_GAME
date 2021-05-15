#include <clocale>
#include <ncurses.h>
#include <iostream>
#include <ctime>
#include <unistd.h> //sleep
#include <termios.h> // kbhit()
#include <sys/ioctl.h> // kbhit()
#include <vector>
#include <random>
#include <algorithm>
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


  //wrefresh(win);
}

// snake 초기화
void SnakeInit(){
  snake.y = 10;   // 처음 위치 초기화
  snake.x = 10;
  snake.length = 3;   // 길이 초기화
  //snake.head = KEY_LEFT;   // 방향 초기화
  snake.fail = true;

  //뱀의 시작 지점
  vector <int> vec1;     //임시 1차원 벡터
  vec1.push_back(snake.y);
  vec1.push_back(snake.x);
  snake.vec.push_back(vec1);
  vec1[1]++;
  snake.vec.push_back(vec1);
  vec1[1]++;
  snake.vec.push_back(vec1);

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
  //뱀 벡터 초기화
  vector <int> vec1;     //임시 1차원 벡터
  vec1.push_back(snake.y);
  vec1.push_back(snake.x);
  snake.vec.insert(snake.vec.begin(),vec1);
  if (snake.vecitem.end()==find(snake.vecitem.begin(), snake.vecitem.end(), vec1)) {
    snake.vec.pop_back();  //아이템을 먹지 못한 경우
  } else snake.length++;   //아이템을 먹은 경우
}


//맵 생성
void Map() {
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
  wattron(snake.win, COLOR_PAIR(1));
  for (int i=0; i<mapline; i++) {
    for (int j=0; j<mapline; j++) {
      mvwprintw(snake.win, i+1, j+1, to_string(map[i][j]).c_str());
    }
  }
  wattroff(snake.win, COLOR_PAIR(1));
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

void InsertItemTime() {
  snake.InsertItemTime = clock();
}
void DeleteItemTime() {
  snake.DeleteItemTime = clock();
}

//아이템 출현
void insertitem() {
  InsertItemTime();
  srand(time(NULL));
  vector <int> vec2 = {0, 0};  //임시 1차원 벡터
  do {
    vec2.pop_back();
    vec2.pop_back();
    int a = rand() % 21 + 1;
    int b = rand() % 21 + 1;
    vec2.push_back(a);
    vec2.push_back(b);
  } while (snake.vec.end()!=find(snake.vec.begin(), snake.vec.end(), vec2));
  snake.vecitem.push_back(vec2);
  wattron(snake.win, COLOR_PAIR(1));
  mvwprintw(snake.win, vec2[0], vec2[1], "5");
  wattroff(snake.win, COLOR_PAIR(1));
  wrefresh(snake.win);
}

//아이템 삭제
void deleteitem() {
  DeleteItemTime();
  mvwprintw(snake.win, snake.vecitem[0][0], snake.vecitem[0][1], "0");
  snake.vecitem.erase(snake.vecitem.begin());
  wrefresh(snake.win);
}
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

void result_win(){
  snake.res = newwin(23, 23, 1, 1);
   // 맵 생성
  wbkgd(snake.res, COLOR_PAIR(3));   // 맵 배경
  keypad(stdscr, TRUE);   // 특수 키 입력 가능
  curs_set(0);   // 커서 가림
  wattron(snake.res, COLOR_PAIR(3));
  if(snake.fail == false) mvwprintw(snake.res, 1, 1, "fail");
  //else~ 성공으로 겜 끝난 경우는 또 따로
  wattroff(snake.res, COLOR_PAIR(3));
  wattron(snake.res, COLOR_PAIR(1));
  wborder(snake.res, '*', '*', '*', '*', '*', '*', '*', '*');
  wattroff(snake.res, COLOR_PAIR(1));
  wrefresh(snake.res);
}


int main(){
  //WINDOW *win;
  OptionInit();
  TermInit();
  WinInit();
  bool t = true;
  //win = newwin(23, 23, 1, 1);
  //Map(win); //맵 생성
  SnakeInit(); // snake 초기화
  InsertItemTime();
  DeleteItemTime();
  while (snake.fail) {          //무한 반복
    usleep(300000);    //0.5초 대기
    press();           //방향키 입력
    deletesnake(); //이전 뱀 출력된거 삭제
    Turn();         //머리 방향 초기화
    move();         //머리 위치 이동
    StateUpdate();
    fail();
    printsnake(); //새로운 뱀 출력
    if ((1.0)*(clock()-snake.InsertItemTime)/1000.0>2) insertitem();
    if ((1.0)*(clock()-snake.DeleteItemTime)/1000.0>3) deleteitem();
  }
  delwin(snake.win);
  result_win();
  getch();
  endwin();
  return 0;
}
