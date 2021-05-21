typedef struct SNAKE {
  int y;     // snake 위치 y좌표
  int x;     // snake 위치 x좌표
  int length;    // snake 길이, 최소 3
  int head;       // head방향
  int ch = KEY_LEFT;    // 방향키의 방향
  clock_t standTime;   // 기준 시간
  clock_t currTime;   // 현재 시간
  bool fail;  // 게임지속
  WINDOW *win;   // 윈도우
  WINDOW *res;
  clock_t InsertGrowthItemTime;
  clock_t DeleteGrowthItemTime;
  clock_t InsertPoisonItemTime;
  clock_t DeletePoisonItemTime;
  clock_t InsertGateTime;  // 게이트 생성 시간
  clock_t DeleteGateTime;
  vector<vector<int>> vec;   //뱀 위치 정보
  vector<vector<int>> vecGrowthItem; //길어지는 아이템 모음
  vector<vector<int>> vecPoisonItem; //짧아지는 아이템 모음
  vector<int> vec1;  //  진입: 4방향, y, x
  vector<int> vec2;  //  진출: 4방향, y, x
  int gateCheck = 0; //임시변수
  int mapline = 35;
} SNAKE;
