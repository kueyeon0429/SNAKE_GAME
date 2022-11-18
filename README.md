# SNAKE GAME

## Manual
벽에 부딪히지 않도록 방향키를 이용해 snake의 진행방향을 조종하면서 미션을 수행하는 게임

### Component
벽 : ![image](https://user-images.githubusercontent.com/66217855/202678298-1d70560c-9aac-4199-97be-020d34736c2f.png)  
뱀 : ![image](https://user-images.githubusercontent.com/66217855/202678354-86fe6277-3de7-4af6-adeb-59e0cb38a18c.png) (빨강색 : 머리, 노랑색 : 몸통)  
게이트 입구 : ![image](https://user-images.githubusercontent.com/66217855/202678457-54293cff-2624-454d-a5db-f1a9ac520d56.png)  
게이트 출구 : ![image](https://user-images.githubusercontent.com/66217855/202678503-df7575f0-6469-42dd-96ba-1367dca01fef.png)  
GrowthItem : ![image](https://user-images.githubusercontent.com/66217855/202678547-90df0636-7bff-402d-973c-b8d83581a0c9.png)  
PoisonItem : ![image](https://user-images.githubusercontent.com/66217855/202678591-09b1047d-82b9-4d97-a8ef-0583f1f9d405.png)   

### How to play the game
1. 터미널의 가로의 길이는 54 이상, 세로의 길이는 40 이상으로 맞춘다.(윈도우의 최소 크기 54x40)
2. 뱀은 왼쪽 상단에서 왼쪽 방향으로 진행되며 시작합니다
3. 오른쪽 미션 창에 미션을 모두 달성하면 성공합니다.
4. 벽(흰색)에 부딪히거나 게이트의 출구(빨강색)로 들어가거나 몸의 길이가 3보다 작아지면 실패합니다.
5. 게임을 실패할 시 fail, 성공할 시 success 화면이 뜨며 방향키를 입력할 시 fail일 경우 현재 단계 재시도, success일 경우 다음 단계로 넘어갑니다.
6. 게임을 처음부터 진행하고 싶은 경우 재시작하길 바랍니다.

***
##
| 20203065 박규연 | 20203107 유태근 |

## Skills of use
C++, ncurses

## Files
- **Snake.cpp** 게임 실행
- **Snake.h** Snake 구조체
- **Mission.h** Mission 보드 구조체
- **Scoreboard.h** Score 보드 구조체

## Complie
```
g++ -std=c++11 -o Snake Snake.cpp -lncursesw
```

## Run
```
./Snake
```

## Demo
![image](https://user-images.githubusercontent.com/66217855/202676265-8089191c-fdf0-42c0-8942-9e2265027e45.png)
[YouTube](https://www.youtube.com/watch?v=TRd4rVZvlJE)
