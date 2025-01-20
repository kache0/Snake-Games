#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <time.h>
#include <Windows.h>
#include <stdlib.h>
#include <conio.h>
#include <locale.h>
#include "header.h"

void gotoxy(int x, int y) { //設置游標位置

    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
int color(int c) { //更改文字顏色

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
    return 0;
}
void welcome() { //開始介面

    int num;
    gotoxy(43, 10);
    color(11);
    printf("貪 食 蛇");

    color(12);
    gotoxy(25, 22);
    printf("1.開始遊戲");
    gotoxy(45, 22);
    printf("2.遊戲說明");
    gotoxy(65, 22);
    printf("3.退出遊戲");

    gotoxy(48, 25);
    printf("輸入選項:");
    scanf("%d",&num); //輸入選項

    switch (num) {
        case 1:
            system("cls"); //清除畫面
            Map(); //創建地圖
            initSnake(); //初始化蛇身
            createFood(); //創建食物
            keyboardContorl(); //按鍵控制
            break;
        case 2:
            explation();
            break;
        case 3:
            exit(0); //退出
            break;
        default:
            gotoxy(40, 28);
            color(12);
            printf("請輸入選項! 如果輸入非數字 請重開程式");
            _getch(); //輸入任意鍵 清除畫面
            system("cls");
            welcome();
    }
}
void Map() { //生成地圖

    system("cls");
    int i, j;
    for(i = 0; i < 58; i += 2) {
        gotoxy(i, 0);
        color(5);
        printf("□");
        gotoxy(i, 26);
        printf("□");
    }

    for(i = 1; i < 26; i++) { 
        gotoxy(0, i);
        printf("□");
        gotoxy(56, i);
        printf("□");
    }

    for(i = 2; i < 56; i += 2) {

        for(j = 1; j < 26; j++) { 
            gotoxy(i, j);
            color(3);
            printf("■");
        }
    }
} 
void scoreAndTips() { //分數和提示

    gotoxy(74, 2);
    color(12);
    printf("得分: %d" ,score);
    gotoxy(74, 4);
    color(12);
    level(score, add);

    gotoxy(74, 8);
    color(14);
    printf("每吃一個食物的得分: %d分",add);
    gotoxy(74, 10);
    printf("不能碰牆 不能咬到自己");
    gotoxy(74, 12);
    printf("用 ↑ ↓ ← → 控制蛇的移動!");
    gotoxy(74, 14);
    printf("F1為加速 F2為減速");
    gotoxy(74, 16);
    printf("空白鍵暫停遊戲");

    gotoxy(74, 18);
    printf("ESC退出遊戲");
} 
void level() { //等級顯示
    
    if(score >= 0 && score < 50) level_now = 1;
    else if(score >= 50 && score < 200) level_now = 2;
    else if(score >= 200 && score < 500) level_now = 3;
    else if(score >= 500 && score < 1000) level_now = 4;
    else level_now = 5;

    switch (level_now) {
        case 1: printf("等級1"); break;
        case 2: printf("等級2"); break;
        case 3: printf("等級3"); break;
        case 4: printf("等級4"); break;
        case 5: printf("等級5"); break;
        default: break;
    }
} 
void initSnake() { //初始化蛇身體

    snake *tail;
    int i;
    tail = (snake*)malloc(sizeof(snake));
    tail->x = 24; //初始位置
    tail->y = 10;
    tail->next = NULL;
    
    for(i = 1; i <= 2; i++) {
        head = (snake*)malloc(sizeof(snake));
        head->next = tail; //蛇頭的下一個節點會變成蛇尾
        head->x = 24 + 2 * i;
        head->y = 10;
        tail = head; //蛇頭變蛇尾 
    }
    while(tail != NULL) {
        gotoxy(tail->x, tail->y);
        color(14);
        printf("◆");
        tail = tail->next; //蛇頭輸出到蛇尾
    }
} 
void createFood() { //隨機生成食物

    snake* food_1;
    srand((unsigned)time(NULL));
    food_1 = (snake*)malloc(sizeof(snake));
    food_1->x = 36;
    food_1->y = 9;

    while ((food_1->x % 2) != 0 || (food_1->y % 2) != 0) {
        food_1->x = rand() % 52 + 2;
        food_1->y = rand() % 24 + 1;
    }

    ptr = head;
    while(ptr->next == NULL) {
        if(ptr->x == food_1->x && ptr->y == food_1->y) {
            free(food_1);
            createFood();
        }        
        ptr = ptr->next;
    }
    gotoxy(food_1->x, food_1->y);
    food = food_1;
    color(12);
    printf("◆");
} 
int biteSelf() { //檢查有沒有咬到自己

    snake *self; //定義蛇身節點
    self = head->next; //self蛇頭之外的蛇身節點
    while(self != NULL) {
        //如果self和蛇身的節點重合
        if(self->x == head->x && self->y == head->y) {
            return 1;
        }
        self = self->next;
    }
    return 0;

} 
void crossWall() { //蛇撞到牆的情況
    
    if(head->x == 0 || head->x == 56 || head->y == 0 || head->y == 26) {
        endgamestatus = 1; //蛇頭撞牆
        endGame(); //遊戲結束畫面
    }
    

} 
void speedUp() { //按f1加速
    
    if(sleeptime >= 50) { //如果時間間隔大於50
        sleeptime -= 10;  //時間間隔-10
        add += 2;         //食物得分+2
    }

} 
void speedDown() { //按f2減速

    if(sleeptime <= 350) { //如果時間間隔小於350
        sleeptime += 30;   //時間間隔+30
        add -= 2;          //食物得分-2
    }

} 
void snakeMove() { //控制蛇方向
    
    snake* nexthead;
    crossWall();
    nexthead = (snake*)malloc(sizeof(snake)); //下一步開闢空間
    if(status == U) { //向上走
        nexthead->x = head->x; //向上前進 x座標不見 y座標-1
        nexthead->y = head->y - 1; 
        nexthead->next = head;
        head = nexthead;
        ptr = head;
        if(nexthead->x == food->x && nexthead->y == food->y) { //如果下一個位置的座標和食物相同
            while(ptr != NULL) {
                gotoxy(ptr->x,ptr->y);
                color(14);
                printf("◆"); //原來是食物的位置換成蛇身
                ptr = ptr->next; //指針ptr指向蛇身的下一位也要執行循環操作
            }
            score += add; //吃到食物 在總分加食物的分
            speedUp();
            createFood();
        } else {
            while(ptr->next->next != NULL) {
                gotoxy(ptr->x, ptr->y);
                color(14);
                printf("◆"); //蛇正常往前走
                ptr = ptr->next;
            }
            //ptr指向蛇尾 蛇尾的下一位就是蛇走過的位置
            gotoxy(ptr->next->x, ptr->next->y);
            color(3);
            printf("■");
            free(ptr->next); 
            ptr->next = NULL;
        }
    }
    if(status == D) { //向下走
        nexthead->x = head->x; //向下前進 x座標不動 y座標+1
        nexthead->y = head->y + 1; 
        nexthead->next = head;
        head = nexthead;
        ptr = head;
        if(nexthead->x == food->x && nexthead->y == food->y) { //如果下一個位置的座標和食物相同
            while(ptr != NULL) {
                gotoxy(ptr->x,ptr->y);
                color(14);
                printf("◆"); //原來是食物的位置換成蛇身
                ptr = ptr->next; //指針ptr指向蛇身的下一位也要執行循環操作
            }
            score += add; //吃到食物 在總分加食物的分
            speedUp();
            createFood();
        } else {
            while(ptr->next->next != NULL) {
                gotoxy(ptr->x, ptr->y);
                color(14);
                printf("◆"); //蛇正常往前走
                ptr = ptr->next;
            }
            //ptr指向蛇尾 蛇尾的下一位就是蛇走過的位置
            gotoxy(ptr->next->x, ptr->next->y);
            color(3);
            printf("■");
            free(ptr->next); 
            ptr->next = NULL;
        }
    }
    if(status == L) { //向左走
        nexthead->x = head->x - 2; //向左前進 x座標向左-2 y座標動
        nexthead->y = head->y; 
        nexthead->next = head;
        head = nexthead;
        ptr = head;
        if(nexthead->x == food->x && nexthead->y == food->y) { //如果下一個位置的座標和食物相同
            while(ptr != NULL) {
                gotoxy(ptr->x,ptr->y);
                color(14);
                printf("◆"); //原來是食物的位置換成蛇身
                ptr = ptr->next; //指針ptr指向蛇身的下一位也要執行循環操作
            }
            score += add; //吃到食物 在總分加食物的分
            speedUp();
            createFood();
        } else {
            while(ptr->next->next != NULL) {
                gotoxy(ptr->x, ptr->y);
                color(14);
                printf("◆"); //蛇正常往前走
                ptr = ptr->next;
            }
            //ptr指向蛇尾 蛇尾的下一位就是蛇走過的位置
            gotoxy(ptr->next->x, ptr->next->y);
            color(3);
            printf("■");
            free(ptr->next); 
            ptr->next =NULL;
        }
    }
    if(status == R) { //向右走
        nexthead->x = head->x + 2; //向右前進 x座標向右 y座標不動
        nexthead->y = head->y; 
        nexthead->next = head;
        head = nexthead;
        ptr = head;
        if(nexthead->x == food->x && nexthead->y == food->y) { //如果下一個位置的座標和食物相同
            while(ptr != NULL) {
                gotoxy(ptr->x,ptr->y);
                color(14);
                printf("◆"); //原來是食物的位置換成蛇身
                ptr = ptr->next; //指針ptr指向蛇身的下一位也要執行循環操作
            }
            score += add; //吃到食物 在總分加食物的分
            speedUp();
            createFood();
        } else {
            while(ptr->next->next != NULL) {
                gotoxy(ptr->x, ptr->y);
                color(14);
                printf("◆"); //蛇正常往前走
                ptr = ptr->next;
            }
            //ptr指向蛇尾 蛇尾的下一位就是蛇走過的位置
            gotoxy(ptr->next->x, ptr->next->y);
            color(3);
            printf("■");
            free(ptr->next); 
            ptr->next =NULL;
        }
    }

    if(biteSelf() == 1) { //判斷是否咬到自己
        endgamestatus = 2; //咬到自己遊戲結束
        endGame();
    }

}
void keyboardContorl() { //控制鍵盤按鍵
    
    while(1) {
        scoreAndTips();
        if(GetAsyncKeyState(VK_UP) && status != D) { //按上鍵 
            status = U;
        } else if(GetAsyncKeyState(VK_DOWN) && status != U) { //按下鍵 
            status = D;
        } else if(GetAsyncKeyState(VK_LEFT) && status != R) { //按左鍵 
            status = L;
        } else if  (GetAsyncKeyState(VK_RIGHT) && status != L) { //按右鍵 
            status = R;
        }

        if(GetAsyncKeyState(VK_SPACE)) {
            while(1) {
                Sleep(300);
                if(GetAsyncKeyState(VK_SPACE)) break;
            }
        } else if(GetAsyncKeyState(VK_ESCAPE)) { //按esc鍵 直接結束遊戲
            endgamestatus = 3;
            break;
        } else if(GetAsyncKeyState(VK_F1)) speedUp(); //F1加速 
          else if(GetAsyncKeyState(VK_F2)) speedDown(); //F2減速
        Sleep(sleeptime);
        snakeMove();
    }
} 
void lostDraw() { //失敗遊戲畫面

    system("cls");
    int i;

    gotoxy(17, 5);
    color(11);
    printf("+-----------------------------------");

    gotoxy(53, 5);
    color(11);
    printf("------------------------------------+");

    for(i = 6; i <= 19; i++) {
        color(11);
        gotoxy(17, i);
        printf("|");
        gotoxy(89, i);
        printf("|");
    }

    gotoxy(17,20);
    color(11);
    printf("+-----------------------------------");

    gotoxy(53,20);
    color(11);
    printf("------------------------------------+");

} 
void endGame() { //結束遊戲畫面
    
    
    if(endgamestatus == 1) {
        lostDraw();
        gotoxy(45, 10);
        color(12);
        printf("撞到牆了");
    } else if(endgamestatus == 2) {
        lostDraw();
        gotoxy(45, 10);
        color(12);
        printf("咬到自己了");        
    } else if(endgamestatus == 3) {
        lostDraw();
        gotoxy(45, 10);
        color(12);
        printf("GAME OVER!");        
    }
    gotoxy(44,13);
    color(11);
    printf("您的的得分是 %d", score);
    gotoxy(44, 15);
    color(14);
    level();
    choose();
} 
void choose() { //結束後選擇

    int num;
    gotoxy(30, 24);
    color(12);
    printf("繼續遊戲 -1");
    gotoxy(57, 24);
    color(11);
    printf("退出遊戲 -2");
    gotoxy(47, 28);
    color(11);
    printf("請選擇 1 或 2 :");
    scanf("%d", &num);
    switch(num) {
        case 1:
            system("cls");
            score = 0;
            sleeptime = 200;
            add = 10;
            welcome();
            break;
        case 2:
            exit(0);
            break;
        default: 
            gotoxy(35, 27);
            color(12);
            printf("輸入有誤 請重新輸入 1 或 2");
            system("pause > null");
            endGame();
            choose();
            break;
    }

} 
void explation() { //遊戲說明

    system("cls");
    int i, j = 1;

    color(13);
    gotoxy(44,3);
    printf("遊戲說明");
    color(2);
    for(i = 6; i <= 25; i++ ) {
        for(j = 20; j <= 75; j++) {
            gotoxy(j, i);
            if (i ==6 || i ==25) printf("=");
            else if (j == 20 || j == 75) printf("||");
        }
    }

    color(3);
    gotoxy(30,8);
    printf("1.不能穿牆 不能咬到自己");
    color(10);
    gotoxy(30,11);
    printf("2.分別用 ↑ ↓ ← → 控制蛇的移動");
    color(14);
    gotoxy(30,14);
    printf("F1可以加速\tF2可以減速");
    color(11);
    gotoxy(30, 17);
    printf("4.按空白鍵暫停 再按繼續");

    color(4);
    gotoxy(30, 20);
    printf("5.按任意鍵返回遊戲開始介面");
    _getch();
    system("cls");
    welcome();
} 



int main() {

    system("mode con cols=110 lines=40");
    setlocale(LC_ALL, ".UTF-8");
    welcome();
    keyboardContorl();
    endGame();
    return 0;

} 
