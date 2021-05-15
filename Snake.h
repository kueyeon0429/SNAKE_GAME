typedef struct SNAKE {
  int y;     // snake 위치 y좌표
  int x;     // snake 위치 x좌표
  int length;    // snake 길이, 최소 3
  int head;       // head방향
  int ch = KEY_LEFT;    // 방향키의 방향
  clock_t startTime;   // 시작 시간
  clock_t standTime;   // 기준 시간
  clock_t currTime;   // 현재 시간
  clock_t InsertGrowthItemTime;
  clock_t DeleteGrowthItemTime;
  clock_t InsertPoisonItemTime;
  clock_t DeletePoisonItemTime;
  vector<vector<int>> vec;   //뱀 위치 정보
  vector<vector<int>> vecGrowthItem; //길어지는 아이템 모음
  vector<vector<int>> vecPoisonItem; //짧아지는 아이템 모음
  bool fail;
  WINDOW *res;
  WINDOW *win;   // 윈도우
} SNAKE;
