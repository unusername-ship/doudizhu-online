#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

// 工具函数：把数字翻译成牌面
void print_card(int card) {
    const char *suits[] = {"黑桃", "红桃", "梅花", "方块"}; 
    const char *ranks[] = {"3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A", "2"};

    if (card == 53) {
        printf("小王 ");
    } else if (card == 54) {
        printf("大王 ");
    } else {
        int suit_index = (card - 1) / 13;
        int rank_index = (card - 1) % 13;
        printf("%s%s ", suits[suit_index], ranks[rank_index]);
    }
}

void deal(int player[]);

// 核心工具：计算一张牌的“权重值”，用于排序
int get_card_weight(int card) {
    // 我们把 3 设定为权重 3，4 设定为 4... K设定为 13，A 设定为 14，2 设定为 15
    // 权重越大，说明牌越大，这样排出来的顺序就和人类习惯一样了
    if (card == 53) return 16; // 小王
    if (card == 54) return 17; // 大王
    
    int rank_index = (card - 1) % 13; // 0 到 12
    return rank_index + 3;            // 变成 3 到 15
}

int main() {
    #ifdef _WIN32
    SetConsoleOutputCP(65001);
    #endif

    int deck[54];
    // 增加三个玩家手牌数组，每人 17 张
    int player1[17];
    int player2[17];
    int player3[17];
    int bottom[3]; // 底牌

    // 1. 生成牌
    for (int i = 0; i < 54; i++) deck[i] = i + 1;

    // 2. 随机种子并洗牌
    srand((unsigned)time(NULL));
    for (int i = 53; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }

    // ==========================================
    // 3. 发牌阶段（把牌从 deck 数组分发给玩家）
    // ==========================================
    for (int i = 0; i < 17; i++) player1[i] = deck[i];          // 前17张给玩家1
    for (int i = 0; i < 17; i++) player2[i] = deck[i + 17];     // 中间17张给玩家2
    for (int i = 0; i < 17; i++) player3[i] = deck[i + 34];     // 后面17张给玩家3
    for (int i = 0; i < 3; i++)  bottom[i] = deck[i + 51];      // 最后3张做底牌

    // ==========================================
    // 4. 排序阶段（冒泡排序，让牌按从大到小排好）
    // ==========================================
    // 这里用玩家1的数组演示，玩家2、3同理
    deal(player1);
    deal(player2);
    deal(player3);

    for (int i = 0; i < 16; i++) {   
        for (int j = 0; j < 16 - i; j++) {  
            if (get_card_weight(player2[j]) > get_card_weight(player2[j + 1])) {
                int temp = player2[j];
                player2[j] = player2[j + 1];
                player2[j + 1] = temp;
            }
        }
    }
    for (int i = 0; i < 16; i++) { 
        for (int j = 0; j < 16 - i; j++) {
            if (get_card_weight(player3[j]) > get_card_weight(player3[j + 1])) {
                int temp = player3[j];
                player3[j] = player3[j + 1];
                player3[j + 1] = temp;
            }
        }
    }
    // ==========================================
    // 5. 打印测试
    // ==========================================
    printf("=== 玩家 1 ===\n");
    for (int i = 0; i < 17; i++) print_card(player1[i]);
    printf("\n\n");

    printf("=== 玩家 2 ===\n");
    for (int i = 0; i < 17; i++) print_card(player2[i]);
    printf("\n\n");

    printf("=== 玩家 3 ===\n");
    for (int i = 0; i < 17; i++) print_card(player3[i]);
    printf("\n\n");

    printf("=== 底牌 ===\n");
    for (int i = 0; i < 3; i++) print_card(bottom[i]);
    printf("\n");

    return 0;
}

void deal(int player[]){
    for (int i = 0; i < 16; i++) {             // 循环16次
        for (int j = 0; j < 16 - i; j++) {     // 每次把最大的牌“冒泡”到最前面
            // 如果前一张牌的权重 > 后一张牌的权重，就交换它们
            if (get_card_weight(player[j]) > get_card_weight(player[j + 1])) {
                int temp = player[j];
                player[j] = player[j + 1];
                player[j + 1] = temp;
            }
        }
    }
}