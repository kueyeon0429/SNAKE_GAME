typedef struct SNAKE {
    int y;     // snake 위치 y좌표
    int x;     // snake 위치 x좌표
    int length;    // snake 길이, 최소 3
    int head;       // head방향
    clock_t standTime;   // 기준 시간
    clock_t currTime;   // 현재 시간
    WINDOW *win;   // 윈도우
} SNAKE;
