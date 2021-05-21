typedef struct MISSION{
  WINDOW *mis_win;
  int B; // Current length
  int growth;
  int poison;
  int G;
  //목표 달성 여부
  bool B_ = false;
  bool g_ = false;
  bool p_ = false;
  bool G_ = false;
} MISSION;
