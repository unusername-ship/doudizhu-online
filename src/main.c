#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 引入 Windows 系统库，专门用来解决乱码
#ifdef _WIN32
#include <windows.h>
#endif

int main() {
    // 强制让终端使用 UTF-8 编码
    #ifdef _WIN32
    SetConsoleOutputCP(65001);
    #endif

    int deck[54];

    // 1. 生成牌
    for (int i = 0; i < 54; i++) {
        deck[i] = i + 1;
    }

    // 2. 随机种子
    srand((unsigned)time(NULL));

    // 3. 洗牌
    for (int i = 53; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }

    // 4. 打印（全中文，绝不乱码）
    printf("==== 洗牌后的牌堆 ====\n");
    
    const char *suits[] = {"黑桃", "红桃", "梅花", "方块"}; 
    const char *ranks[] = {"3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A", "2"};

    for (int i = 0; i < 54; i++) {
        int card = deck[i];

        if (card == 53) {
            printf("小王 ");
        } else if (card == 54) {
            printf("大王 ");
        } else {
            int suit_index = (card - 1) / 13;
            int rank_index = (card - 1) % 13;
            printf("%s%s ", suits[suit_index], ranks[rank_index]);
        }
        
        if ((i + 1) % 10 == 0) {
            printf("\n");
        }
    }
    printf("\n======================\n");

    return 0;
}