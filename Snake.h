typedef struct SNAKE {
  int y;     // snake 위치 y좌표
  int x;     // snake 위치 x좌표
  int length;    // snake 길이, 최소 3
  int head;       // head방향
  int ch = KEY_LEFT;    // 방향키의 방향
  clock_t standTime;   // 기준 시간
  clock_t currTime;   // 현재 시간
  vector<vector<int>> vec; //뱀의 최대 길이 30  //뱀 위치 정보
  bool fail;  // 게임지속
  WINDOW *win;   // 윈도우
  WINDOW *res;
  clock_t InsertItemTime;   //아이템 추가 임시 시간
  clock_t DeleteItemTime;   //아이템 삭제 임시 시간
  clock_t InsertGateTime;  // 게이트 생성 시간
  clock_t DeleteGateTime;
  vector<vector<int>> vecitem; //출현한 아이템 모음
  vector<int> vec1;  //  진입: 4방향, y, x
  vector<int> vec2;  //  진출: 4방향, y, x
} SNAKE;
