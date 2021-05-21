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
#include "ScoreBoard.h"
#include "Mission.h"

SNAKE snake;
SCORE_BOARD score;
MISSION mission;

void SnakeInit(WINDOW *win);
void Map(WINDOW *win);
void Turn();
void move();
void StateUpdate();
void deletesnake(WINDOW *win);
void printsnake(WINDOW *win);
void GateOn();

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



/*********************init************************/

// 옵션 초기화
void OptionInit(){
  setlocale(LC_ALL, "");
  initscr();
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_YELLOW);   // 팔레트1
  init_pair(2, COLOR_BLACK, COLOR_WHITE);    // 팔레트2
  init_pair(3, COLOR_WHITE, COLOR_RED);  // fail!, poison
  init_pair(4, COLOR_WHITE, COLOR_BLUE);    // growth
  init_pair(5, COLOR_WHITE, COLOR_GREEN);    // gate on
  init_pair(6, COLOR_BLACK, COLOR_GREEN); // gate off
  init_pair(7, COLOR_WHITE, COLOR_BLACK);    // 팔레트2
}

// 터미널 초기화
void TermInit(){
  resize_term(39, 54);
  border('|', '|', '-', '-', '+', '+', '+', '+');   // 테두리
  refresh();   // 화면 출력
  noecho();   // 입력한 문자 가리기
  //getch();   // 문자 입력시 다음 이동
}

// 점수판 윈도우 초기화
void ScoreWinInit(){
  score.sco_win = newwin(14, 15, 1, 38);
   // 맵 생성
  curs_set(0);   // 커서 가림
  wattron(score.sco_win, COLOR_PAIR(7));
  wborder(score.sco_win, '|','|','-','-','+','+','+','+');
  mvwprintw(score.sco_win, 2, 2, "Score Board");
  mvwprintw(score.sco_win, 5, 2, "B: ");
  mvwprintw(score.sco_win, 5, 5, "%d", snake.length);
  mvwprintw(score.sco_win, 5, 7, "/");
  mvwprintw(score.sco_win, 5, 8, "%d", snake.Max);
  mvwprintw(score.sco_win, 7, 2, "+: ");
  mvwprintw(score.sco_win, 7, 5, "%d", snake.growth);
  mvwprintw(score.sco_win, 9, 2, "-: ");
  mvwprintw(score.sco_win, 9, 5, "%d", snake.poison);
  mvwprintw(score.sco_win, 11, 2, "G: ");
  mvwprintw(score.sco_win, 11, 5, "%d", snake.G);
  wattroff(score.sco_win, COLOR_PAIR(7));
  wrefresh(score.sco_win);

}

// 미션 윈도우 초기화
void MissionWinInit(){
  mission.mis_win = newwin(14, 15, 15, 38);
   // 맵 생성
  curs_set(0);   // 커서 가림
  wattron(mission.mis_win, COLOR_PAIR(7));
  wborder(mission.mis_win, '|','|','-','-','+','+','+','+');
  mvwprintw(mission.mis_win, 2, 2, "Mission");
  mvwprintw(mission.mis_win, 5, 2, "B: ");
  mvwprintw(mission.mis_win, 5, 5, "%d", mission.B);
  mvwprintw(mission.mis_win, 5, 7, "(");
  if(snake.length >= mission.B) mvwprintw(mission.mis_win, 5, 8, "O");
  mvwprintw(mission.mis_win, 5, 9, ")");

  mvwprintw(mission.mis_win, 7, 2, "+: ");
  mvwprintw(mission.mis_win, 7, 5, "%d", mission.growth);
  mvwprintw(mission.mis_win, 7, 7, "(");
  if(snake.growth >= mission.growth) mvwprintw(mission.mis_win, 7, 8, "O");
  mvwprintw(mission.mis_win, 7, 9, ")");

  mvwprintw(mission.mis_win, 9, 2, "-: ");
  mvwprintw(mission.mis_win, 9, 5, "%d", mission.poison);
  mvwprintw(mission.mis_win, 9, 7, "(");
  if(snake.poison >= mission.poison) mvwprintw(mission.mis_win, 9, 8, "O");
  mvwprintw(mission.mis_win, 9, 9, ")");

  mvwprintw(mission.mis_win, 11, 2, "G: ");
  mvwprintw(mission.mis_win, 11, 5, "%d", mission.G);
  mvwprintw(mission.mis_win, 11, 7, "(");
  if(snake.G >= mission.G) mvwprintw(mission.mis_win, 11, 8, "O");
  mvwprintw(mission.mis_win, 11, 9, ")");

  wattroff(mission.mis_win, COLOR_PAIR(7));
  wrefresh(mission.mis_win);

}

// 윈도우 초기화
void WinInit(){
  snake.win = newwin(snake.mapline+2, snake.mapline+2, 1, 1);
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
  snake.length = 3;   // 길이 초기화
  snake.head = KEY_LEFT;   // 방향 초기화

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



/*********************snake************************/

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



/*********************move************************/

//방향키 받는 함수
int press() {
  if (kbhit()) {
    return getch();
  }
}

//머리 방향 초기화
void Turn(int a){
  if (a == KEY_LEFT && snake.head != KEY_RIGHT) {
    snake.head = KEY_LEFT;
  }
  else if (a == KEY_RIGHT && snake.head != KEY_LEFT) {
    snake.head = KEY_RIGHT;
  }
  else if (a == KEY_UP && snake.head != KEY_DOWN) {
    snake.head = KEY_UP;
  }
  else if (a == KEY_DOWN && snake.head != KEY_UP) {
    snake.head = KEY_DOWN;
  }
}

// 상태 업데이트
void StateUpdate(){
  //if(snake.x == snake.vec1[2] && snake.y == snake.vec1[1]) GateOn();

  vector <int> vec1;     //임시 1차원 벡터
  vec1.push_back(snake.y);
  vec1.push_back(snake.x);
  snake.vec.insert(snake.vec.begin(),vec1);
  if(snake.vecGrowthItem.end()==find(snake.vecGrowthItem.begin(), snake.vecGrowthItem.end(), vec1)) {
    snake.vec.pop_back();  //길어지는 아이템을 먹지 못한 경우
  } else { snake.length++; snake.growth++; }   //아이템을 먹은 경우
  if (snake.vecPoisonItem.end()!=find(snake.vecPoisonItem.begin(), snake.vecPoisonItem.end(), vec1)) {
    snake.vec.pop_back();  //짧아지는 아이템을 먹은 경우
    snake.length--;
    snake.poison++;
  }
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



/*********************item************************/

void InsertGrowthItemTime() {
  snake.InsertGrowthItemTime = clock();
}
void DeleteGrowthItemTime() {
  snake.DeleteGrowthItemTime = clock();
}
void InsertPoisonItemTime() {
  snake.InsertPoisonItemTime = clock();
}
void DeletePoisonItemTime() {
  snake.DeletePoisonItemTime = clock();
}

//먹으면 길어지는 아이템
void InsertGrowthItem() {
  InsertGrowthItemTime();
  srand(time(NULL));
  vector <int> vec2 = {0, 0};  //임시 1차원 벡터
  do {
    vec2.pop_back();
    vec2.pop_back();
    int a = rand() % snake.mapline + 1;
    int b = rand() % snake.mapline + 1;
    vec2.push_back(a);
    vec2.push_back(b);
  } while (snake.vec.end()!=find(snake.vec.begin(), snake.vec.end(), vec2) || snake.vecPoisonItem.end()!=find(snake.vecPoisonItem.begin(), snake.vecPoisonItem.end(), vec2));
  snake.vecGrowthItem.push_back(vec2);
  wattron(snake.win, COLOR_PAIR(4));
  mvwprintw(snake.win, vec2[0], vec2[1], "5");
  wattroff(snake.win, COLOR_PAIR(4));
  wrefresh(snake.win);
}

//아이템 삭제
//길어지는 아이템 삭제
void DeleteGrowthItem() {
  DeleteGrowthItemTime();
  mvwprintw(snake.win, snake.vecGrowthItem[0][0], snake.vecGrowthItem[0][1], "0");
  snake.vecGrowthItem.erase(snake.vecGrowthItem.begin());
  wrefresh(snake.win);
}


//먹으면 짧아지는 아이템
void InsertPoisonItem() {
  InsertPoisonItemTime();
  srand(time(NULL)+1000000);
  vector <int> vec3 = {0, 0};  //임시 1차원 벡터
  do {
    vec3.pop_back();
    vec3.pop_back();
    int a = rand() % snake.mapline + 1;
    int b = rand() % snake.mapline + 1;
    vec3.push_back(a);
    vec3.push_back(b);
  } while (snake.vec.end()!=find(snake.vec.begin(), snake.vec.end(), vec3) || snake.vecGrowthItem.end()!=find(snake.vecGrowthItem.begin(), snake.vecGrowthItem.end(), vec3));
  snake.vecPoisonItem.push_back(vec3);
  wattron(snake.win, COLOR_PAIR(3));
  mvwprintw(snake.win, vec3[0], vec3[1], "6");
  wattroff(snake.win, COLOR_PAIR(3));
  wrefresh(snake.win);
}

//짧아지는 아이템 삭제
void DeletePoisonItem() {
  DeletePoisonItemTime();
  mvwprintw(snake.win, snake.vecPoisonItem[0][0], snake.vecPoisonItem[0][1], "0");
  snake.vecPoisonItem.erase(snake.vecPoisonItem.begin());
  wrefresh(snake.win);
}



/*********************gate************************/

void InsertGateTime() {
  snake.InsertGateTime = clock();
}

void DeleteGateTime() {
  snake.DeleteGateTime = clock();
}

void InsertGate(){
  InsertGateTime();
  srand(time(NULL));


  // (y, x) -> v1(ran, 0) v2(ran, 0)
  while (1) {
    snake.vec1.push_back(rand() % 5 + 1);
      if(snake.vec1[0] == 1){  // 상or하, y, 0
        snake.vec1.push_back(0);
        snake.vec1.push_back(rand() % (snake.mapline-2) + 2);
      }
      else if (snake.vec1[0] == 3) {
        snake.vec1.push_back(snake.mapline+1);
        snake.vec1.push_back(rand() % (snake.mapline-2) + 2);
      }
      else if(snake.vec1[0] == 2 ){  // 좌or우, 0, x
        snake.vec1.push_back(rand() % (snake.mapline-2) + 2);
        snake.vec1.push_back(0);
      }
      else if(snake.vec1[0] == 4) {
        snake.vec1.push_back(rand() % (snake.mapline-2) + 2);
        snake.vec1.push_back(snake.mapline+1);
      }
      else if(snake.vec1[0] == 5) {
        snake.vec1.push_back(rand() % (snake.mapline-2) + 2);
        snake.vec1.push_back(rand() % (snake.mapline-2) + 2);
      }

    // (y, x) -> v1(ran, 0) v2(ran, 0)
    snake.vec2.push_back(rand() % 5 + 1);
      if(snake.vec2[0] == 1){  // 상or하, y, 0
        snake.vec2.push_back(0);
        snake.vec2.push_back(rand() % (snake.mapline-2) + 2);
      }
      else if (snake.vec2[0] == 3) {
        snake.vec2.push_back(snake.mapline+1);
        snake.vec2.push_back(rand() % (snake.mapline-2) + 2);
      }
      else if(snake.vec2[0] == 2 ){  // 좌or우, 0, x
        snake.vec2.push_back(rand() % (snake.mapline-2) + 2);
        snake.vec2.push_back(0);
      }
      else if(snake.vec2[0] == 4) {
        snake.vec2.push_back(rand() % (snake.mapline-2) + 2);
        snake.vec2.push_back(snake.mapline+1);
      }
      else if(snake.vec2[0] == 5) {
        snake.vec2.push_back(rand() % (snake.mapline-2) + 2);
        snake.vec2.push_back(rand() % (snake.mapline-2) + 2);
      }
    if (snake.vec1 != snake.vec2) break;
  }
  wattron(snake.win, COLOR_PAIR(5));
  mvwprintw(snake.win, snake.vec1[1], snake.vec1[2], "7");
  wattroff(snake.win, COLOR_PAIR(5));
  wattron(snake.win, COLOR_PAIR(6));
  mvwprintw(snake.win, snake.vec2[1], snake.vec2[2], "7");
  wattroff(snake.win, COLOR_PAIR(6));
  wrefresh(snake.win);
}

// 게이트 삭제
void DeleteGate(){
  //DeleteGateTime();
  if (snake.vec1[0] == 5) {
    mvwprintw(snake.win, snake.vec1[1], snake.vec1[2], "0");
  } else {
    wattron(snake.win, COLOR_PAIR(2));
    mvwprintw(snake.win, snake.vec1[1], snake.vec1[2], "1");
    wattroff(snake.win, COLOR_PAIR(2));
  }
  if (snake.vec2[0] == 5) {
    mvwprintw(snake.win, snake.vec2[1], snake.vec2[2], "0");
  } else {
    wattron(snake.win, COLOR_PAIR(2));
    mvwprintw(snake.win, snake.vec2[1], snake.vec2[2], "1");
    wattroff(snake.win, COLOR_PAIR(2));
  }
  wrefresh(snake.win);
  snake.vec1.clear();
  snake.vec2.clear();
  snake.gateCheck = 0;
}

void GateOn(){
  snake.gateCheck = 1;
  DeleteGateTime();
  if(snake.vec1[0] == 1 || snake.vec1[0] == 2 || snake.vec1[0] == 3 || snake.vec1[0] == 4 || snake.vec1[0] == 5){  // 상
    if(snake.vec2[0] == 1){
      //snake.ch == KEY_DOWN;
      snake.head = KEY_DOWN;
      snake.x = snake.vec2[2];
      snake.y = snake.vec2[1]+1;
    }
    if(snake.vec2[0] == 2){
      //snake.ch == KEY_RIGHT;
      snake.head = KEY_RIGHT;
      snake.x = snake.vec2[2]+1;
      snake.y = snake.vec2[1];
    }
    if(snake.vec2[0] == 3){
      //snake.ch == KEY_UP;
      snake.head = KEY_UP;
      snake.x = snake.vec2[2];
      snake.y = snake.vec2[1]-1;
    }
    if(snake.vec2[0] == 4){
      //snake.ch == KEY_DOWN;
      snake.head = KEY_LEFT;
      snake.x = snake.vec2[2]-1;
      snake.y = snake.vec2[1];
    }
    if(snake.vec2[0] == 5){ //게이트 출구가 모서리가 아닐때
      vector <int> vecup = {snake.vec2[1]-1, snake.vec2[2]};
      vector <int> vecdown = {snake.vec2[1]+1, snake.vec2[2]};
      vector <int> vecleft = {snake.vec2[1], snake.vec2[2]-1};
      vector <int> vecright = {snake.vec2[1], snake.vec2[2]+1};
      if (snake.head == KEY_UP) { //전에 진행방향이 위쪽일때
        if (snake.vecWall.end()==find(snake.vecWall.begin(), snake.vecWall.end(), vecup)) { //원래 진행방향에 벽이 없을때
          snake.head = KEY_UP;
          snake.x = snake.vec2[2];
          snake.y = snake.vec2[1]-1;
        }
        else if (snake.vecWall.end()==find(snake.vecWall.begin(), snake.vecWall.end(), vecright)) { //시계방향에 벽이 없을때
          snake.head = KEY_RIGHT;
          snake.x = snake.vec2[2]+1;
          snake.y = snake.vec2[1];
        }
        else if (snake.vecWall.end()==find(snake.vecWall.begin(), snake.vecWall.end(), vecleft)) { //반시계방향에 벽이 없을때
          snake.head = KEY_LEFT;
          snake.x = snake.vec2[2]-1;
          snake.y = snake.vec2[1];
        }
        else {
          snake.head = KEY_DOWN;
          snake.x = snake.vec2[2];
          snake.y = snake.vec2[1]+1;
        }
      }
      else if (snake.head == KEY_DOWN) { //전에 진행방향이 아래쪽일때
        if (snake.vecWall.end()==find(snake.vecWall.begin(), snake.vecWall.end(), vecdown)) { //원래 진행방향에 벽이 없을때
          snake.head = KEY_DOWN;
          snake.x = snake.vec2[2];
          snake.y = snake.vec2[1]+1;
        }
        else if (snake.vecWall.end()==find(snake.vecWall.begin(), snake.vecWall.end(), vecleft)) { //시계방향에 벽이 없을때
          snake.head = KEY_LEFT;
          snake.x = snake.vec2[2]-1;
          snake.y = snake.vec2[1];
        }
        else if (snake.vecWall.end()==find(snake.vecWall.begin(), snake.vecWall.end(), vecright)) { //반시계방향에 벽이 없을때
          snake.head = KEY_RIGHT;
          snake.x = snake.vec2[2]+1;
          snake.y = snake.vec2[1];
        }
        else {
          snake.head = KEY_UP;
          snake.x = snake.vec2[2];
          snake.y = snake.vec2[1]-1;
        }
      }
      else if (snake.head == KEY_LEFT) { //전에 진행방향이 왼쪽일때
        if (snake.vecWall.end()==find(snake.vecWall.begin(), snake.vecWall.end(), vecleft)) { //원래 진행방향에 벽이 없을때
          snake.head = KEY_LEFT;
          snake.x = snake.vec2[2]-1;
          snake.y = snake.vec2[1];
        }
        else if (snake.vecWall.end()==find(snake.vecWall.begin(), snake.vecWall.end(), vecup)) { //시계방향에 벽이 없을때
          snake.head = KEY_UP;
          snake.x = snake.vec2[2];
          snake.y = snake.vec2[1]-1;
        }
        else if (snake.vecWall.end()==find(snake.vecWall.begin(), snake.vecWall.end(), vecdown)) { //반시계방향에 벽이 없을때
          snake.head = KEY_DOWN;
          snake.x = snake.vec2[2];
          snake.y = snake.vec2[1]+1;
        }
        else {
          snake.head = KEY_RIGHT;
          snake.x = snake.vec2[2]+1;
          snake.y = snake.vec2[1];
        }
      }
      else if (snake.head == KEY_RIGHT) { //전에 진행방향이 오른쪽일때
        if (snake.vecWall.end()==find(snake.vecWall.begin(), snake.vecWall.end(), vecright)) { //원래 진행방향에 벽이 없을때
          snake.head = KEY_RIGHT;
          snake.x = snake.vec2[2]+1;
          snake.y = snake.vec2[1];
        }
        else if (snake.vecWall.end()==find(snake.vecWall.begin(), snake.vecWall.end(), vecdown)) { //시계방향에 벽이 없을때
          snake.head = KEY_DOWN;
          snake.x = snake.vec2[2];
          snake.y = snake.vec2[1]+1;
        }
        else if (snake.vecWall.end()==find(snake.vecWall.begin(), snake.vecWall.end(), vecup)) { //반시계방향에 벽이 없을때
          snake.head = KEY_UP;
          snake.x = snake.vec2[2];
          snake.y = snake.vec2[1]-1;
        }
        else {
          snake.head = KEY_LEFT;
          snake.x = snake.vec2[2]-1;
          snake.y = snake.vec2[1];
        }
      }
    }
  }
  snake.G++;
}




/*********************result************************/

// 실패 조건 판단
void fail(){
  // wall에 닿는 경우
  if(snake.x == 0 || snake.x == (snake.mapline+1) || snake.y == 0 || snake.y == (snake.mapline+1))
    if(snake.x != snake.vec1[2] || snake.y != snake.vec1[1]) snake.fail = false;
  // body에 닿는 경우
  for (int i=4; i < snake.length; i++){
    if(snake.y == snake.vec[i][0] && snake.x == snake.vec[i][1]){
      snake.fail = false;
      break;
    }
  }
  // 길이가 3 미만
  if(snake.length < 3) snake.fail = false;
}

// 성공 조건 판단
void success(){
  if(snake.length >= mission.B && snake.growth >= mission.growth && snake.poison >= mission.poison && snake.G >= mission.G) snake.success = true;
}

void result_win(){
  snake.res = newwin(snake.mapline+2, snake.mapline+2, 1, 1);
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



/*********************main************************/

int main(){
  OptionInit();
  TermInit();
  ScoreWinInit();
  MissionWinInit();
  WinInit();

  bool t = true;
  int a;
  SnakeInit(); // snake 초기화

  InsertGate();
  while(snake.fail&&snake.success!=true) {          //무한 반복
    usleep(100000);    //0.5초 대기
    a = press();           //방향키 입력
    deletesnake(); //x이전 뱀 출력된거 삭제
    Turn(a);         //머리 방향 초기화
    move();         //머리 위치 이동
    if(snake.x == snake.vec1[2] && snake.y == snake.vec1[1]) GateOn();
    StateUpdate();
    ScoreWinInit();
    MissionWinInit();
    fail();   // 실패 조건 판단
    success();  // 성공 조건 판단
    printsnake(); //새로운 뱀 출력
    if ((1.0)*(clock()-snake.InsertGrowthItemTime)>4000) InsertGrowthItem();   //4초인거같지만 2초마다 생성
    if (snake.vecGrowthItem.size()>3) DeleteGrowthItem();                     //아이템의 지속시간은 6초
    if ((1.0)*(clock()-snake.InsertPoisonItemTime)>4000) InsertPoisonItem();
    if (snake.vecPoisonItem.size()>3) DeletePoisonItem();
    if ((1.0)*(clock()-snake.InsertGateTime)/1000.0>4) InsertGate();
    if (snake.gateCheck == 1 && (1.0)*(clock()-snake.DeleteGateTime)> (300*snake.length)) DeleteGate();
  }

  delwin(snake.win);
  result_win();
  getch();
  endwin();
  return 0;
}
