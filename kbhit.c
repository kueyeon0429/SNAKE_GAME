static inline bool
kbhit(void){
    int ch;
    bool ret;

    nodelay(stdscr, TRUE);

    ch = getch();
    if ( ch == ERR ) {
        ret = false;
    } else {
        ret = true;
        ungetch(ch); // 마지막에 받은 문자를 버퍼에 다시 넣어서 다음 getch()가 받을 수 있도록 합니다.
    }

    nodelay(stdscr, FALSE);
    return ret;
}


// 출처: https://blackinkgj.github.io/ncurses/
