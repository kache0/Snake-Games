#ifndef HEADER_FILE
#define HEADER_FILE
//定義常量
#define U 1 //上
#define D 2 //下
#define L 3 //左
#define R 4 //右

//定義全局變量

//蛇身體的一個節點
typedef struct snake
{
    int x;
    int y;
    struct snake* next;
} snake;

//設定全局變數 
int score = 0, add = 10, level_now = 1; //總得分 每次食物得到的分數 現在的等級
int endgamestatus = 0; //遊戲結束的情況1.撞到牆2.咬到自己3.主動exit
int sleeptime = 200; //每次運行時間間隔
int status = R; //蛇的前進狀態
snake *head, *food; //蛇頭指針 食物指針
snake *ptr; //遍歷蛇身指針

void gotoxy(int x, int y); //游標位置
int color(int c); //文字顏色
void welcome(); //開始介面
void Map(); //地圖
void scoreAndTips(); //分數和提示
void level(); //等級顯示
void initSnake(); //初始化蛇身體
void createFood(); //隨機生成食物
int biteSelf(); //檢查有沒有咬到自己
void crossWall(); //蛇撞到牆的情況
void speedUp(); //按f1加速
void speedDown(); //按f2減速
void snakeMove(); //控制蛇方向
void keyboardContorl(); //控制鍵盤按鍵
void lostDraw(); //失敗遊戲畫面
void endGame(); //結束遊戲畫面
void choose(); //結束後選擇
void explation(); //遊戲說明



#endif