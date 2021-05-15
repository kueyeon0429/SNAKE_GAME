typedef struct SNAKE {
  int y;     // snake 위치 y좌표
  int x;     // snake 위치 x좌표
  int length;    // snake 길이, 최소 3
  int head;       // head방향
  int ch = KEY_LEFT;    // 방향키의 방향
  clock_t standTime;   // 기준 시간
  clock_t currTime;   // 현재 시간
  vector<vector<int>> vec; //뱀의 최대 길이 30  //뱀 위치 정보
  WINDOW *win;   // 윈도우
} SNAKE;
