#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_PLAYERS 8
#define MAX_CARDS_PER_PLAYER 200
#define MAX_INPUT_CARDS 30
#define CARD_JOKER_BIG 14
#define CARD_JOKER_SMALL 15

typedef struct {
    int rank;   //1(A),2~10,11(J),12(Q),13(K),14大王,15小王
} Card;

typedef struct {
    Card hand[MAX_CARDS_PER_PLAYER];
    int cardCount;
} Player;

Player players[MAX_PLAYERS];
int playerCnt;
int deckTotal;
Card* deck;

// 将字符串转为牌rank，失败返回-1
int strToRank(const char *s)
{
    if(strcmp(s,"A")==0) return 1;
    if(strcmp(s,"J")==0) return 11;
    if(strcmp(s,"Q")==0) return 12;
    if(strcmp(s,"K")==0) return 13;
    if(strcmp(s,"大王")==0) return CARD_JOKER_BIG;
    if(strcmp(s,"小王")==0) return CARD_JOKER_SMALL;
    int num = atoi(s);
    if(num>=2 && num<=10) return num;
    return -1;
}

// rank转字符串
void rankToStr(int rank, char *buf)
{
    if(rank == 1) strcpy(buf,"A");
    else if(rank ==11) strcpy(buf,"J");
    else if(rank ==12) strcpy(buf,"Q");
    else if(rank ==13) strcpy(buf,"K");
    else if(rank == CARD_JOKER_BIG) strcpy(buf,"大王");
    else if(rank == CARD_JOKER_SMALL) strcpy(buf,"小王");
    else sprintf(buf,"%d",rank);
}

void buildDeck(int pack) {
    int i, idx = 0;
    deckTotal = pack * 54;
    deck = (Card*)malloc(sizeof(Card) * deckTotal);
    for(int p = 0; p < pack; p++){
        for(int r = 1; r <=13; r++){
            for(int s=0;s<4;s++){
                deck[idx++].rank = r;
            }
        }
        deck[idx++].rank = CARD_JOKER_BIG;
        deck[idx++].rank = CARD_JOKER_SMALL;
    }
}

void shuffleDeck(){
    for(int i = deckTotal-1; i>0; i--){
        int r = rand() % (i+1);
        Card t = deck[i];
        deck[i] = deck[r];
        deck[r] = t;
    }
}

void dealCard(){
    int idx = 0;
    for(int i=0;i<playerCnt;i++){
        players[i].cardCount = 0;
    }
    while(idx < deckTotal){
        for(int p=0;p<playerCnt && idx < deckTotal;p++){
            players[p].hand[players[p].cardCount++] = deck[idx++];
        }
    }
}

void sortHand(Player *p)
{
    int n = p->cardCount;
    for(int i = 0; i < n -1; i++)
    {
        for(int j = 0; j < n - i -1; j++)
        {
            if(p->hand[j].rank > p->hand[j+1].rank)
            {
                Card tmp = p->hand[j];
                p->hand[j] = p->hand[j+1];
                p->hand[j+1] = tmp;
            }
        }
    }
}

void printCard(Card c){
    char buf[10];
    rankToStr(c.rank, buf);
    printf("%s ", buf);
}

void printCardPile(Card pile[], int n)
{
    for(int i = 0; i < n; i++)
    {
        printCard(pile[i]);
    }
    printf("\n");
}

void showHand(int pid){
    printf("玩家%d手牌(%d张): ", pid+1, players[pid].cardCount);
    for(int i=0;i<players[pid].cardCount;i++){
        printCard(players[pid].hand[i]);
    }
    putchar('\n');
}

// 从玩家手牌删除指定一组牌（cards数组是要删掉的rank列表，cnt数量）
// 成功返回1，手牌缺少牌返回0
int removeCardsFromHand(int pid, int cards[], int cnt)
{
    Player *p = &players[pid];
    int temp[MAX_CARDS_PER_PLAYER];
    int tempSize = 0;
    // 复制一份待删除列表
    int delList[MAX_INPUT_CARDS];
    memcpy(delList, cards, sizeof(int)*cnt);

    for(int i=0;i<p->cardCount;i++)
    {
        int found = 0;
        for(int j=0;j<cnt;j++)
        {
            if(delList[j] == p->hand[i].rank)
            {
                delList[j] = -1; //标记已删除
                found = 1;
                break;
            }
        }
        if(!found)
        {
            temp[tempSize++] = p->hand[i].rank;
        }
    }
    // 检查是否全部删除成功
    for(int j=0;j<cnt;j++)
    {
        if(delList[j] != -1)
            return 0;
    }
    // 写回手牌
    p->cardCount = tempSize;
    for(int i=0;i<tempSize;i++)
        p->hand[i].rank = temp[i];
    sortHand(p);
    return 1;
}

void returnCards(int pid, Card pile[], int n){
    for(int i=0;i<n;i++){
        players[pid].hand[players[pid].cardCount++] = pile[i];
    }
    sortHand(&players[pid]);
}

int checkLie(Card pile[], int n, int targetRank){
    for(int i=0;i<n;i++){
        int r = pile[i].rank;
        if(r != targetRank && r != CARD_JOKER_BIG && r != CARD_JOKER_SMALL){
            return 0;
        }
    }
    return 1;
}

//清空stdin缓冲区
void clearStdin()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main(){
    srand((unsigned)time(NULL));
    int packCnt;
    printf("===== 游戏【真真假假】=====\n");
    printf("请输入扑克牌副数：");
    scanf("%d", &packCnt);
    printf("请输入玩家数量(最多%d人)：", MAX_PLAYERS);
    scanf("%d", &playerCnt);
    if(playerCnt <=0 || playerCnt > MAX_PLAYERS){
        printf("玩家数量非法\n");
        return 1;
    }
    clearStdin(); //清换行

    buildDeck(packCnt);
    shuffleDeck();
    dealCard();
    free(deck);

    for(int i=0; i<playerCnt; i++){
        sortHand(&players[i]);
    }

    int maxCard = 0;
    for(int i=0;i<playerCnt;i++){
        if(players[i].cardCount > maxCard)
            maxCard = players[i].cardCount;
    }
    int candidates[MAX_PLAYERS];
    int candCnt =0;
    for(int i=0;i<playerCnt;i++){
        if(players[i].cardCount == maxCard)
            candidates[candCnt++] = i;
    }
    int curStarter = candidates[rand() % candCnt];
    printf("\n手牌最多为%d张，随机选定玩家%d作为第一个回合开局人\n", maxCard, curStarter+1);

    int gameOver = 0;
    int winner = -1;

    while(!gameOver){
        printf("\n==== 新回合开始！由玩家%d开局 ====\n", curStarter+1);
        int roundTargetRank;
        int curPlayer = curStarter;
        int roundRunning = 1;

        Card roundPile[MAX_CARDS_PER_PLAYER];
        int roundPileSize = 0;

        showHand(curPlayer);
        char line[256];
        Card playedPile[MAX_CARDS_PER_PLAYER];
        int prevPlayedCnt;
        int claimRank;

        while(1){
            printf("玩家%d，请输入你要打出的牌（空格分隔，例如：2 2 2 或 大王 3 3 3）：\n", curPlayer+1);
            fgets(line, sizeof(line), stdin);
            char *tok = strtok(line, " \n");
            int inputRanks[MAX_INPUT_CARDS];
            int inputCnt = 0;
            int parseOk = 1;
            while(tok != NULL && inputCnt < MAX_INPUT_CARDS)
            {
                int r = strToRank(tok);
                if(r == -1)
                {
                    parseOk = 0;
                    break;
                }
                inputRanks[inputCnt++] = r;
                tok = strtok(NULL, " \n");
            }
            if(!parseOk || inputCnt <=0)
            {
                printf("输入格式错误！支持数字、A、J、Q、K、大王、小王。重新输入。\n");
                continue;
            }
            // 尝试从手牌移除
            if(!removeCardsFromHand(curPlayer, inputRanks, inputCnt))
            {
                printf("你的手牌没有足够的这些牌！重新输入。\n");
                continue;
            }
            // 把打出的牌存入playedPile
            for(int i=0;i<inputCnt;i++)
            {
                playedPile[i].rank = inputRanks[i];
            }
            prevPlayedCnt = inputCnt;

            printf("请输入本轮宣称的牌面（如2、A、J，大王小王不能作为宣称）：");
            char claimBuf[20];
            scanf("%s", claimBuf);
            clearStdin();
            claimRank = strToRank(claimBuf);
            if(claimRank == CARD_JOKER_BIG || claimRank == CARD_JOKER_SMALL || claimRank == -1)
            {
                printf("宣称不能是大王/小王，输入无效，牌退回手牌！\n");
                returnCards(curPlayer, playedPile, prevPlayedCnt);
                continue;
            }
            roundTargetRank = claimRank;
            printf("玩家%d宣称：%d张 %s\n", curPlayer+1, inputCnt, claimBuf);

            // 加入回合总牌堆
            for(int i=0;i<prevPlayedCnt;i++){
                roundPile[roundPileSize++] = playedPile[i];
            }

            if(players[curPlayer].cardCount ==0){
                printf("玩家%d已经打空手牌！等待下家操作\n", curPlayer+1);
            }

            int prevPlayer = curPlayer;
            curPlayer = (curPlayer +1) % playerCnt;

            while(roundRunning){
                printf("\n---轮到玩家%d---\n", curPlayer+1);
                showHand(curPlayer);
                printf("选择操作: 1=质疑上家  2=跟牌\n输入选择：");
                int op;
                scanf("%d", &op);
                clearStdin();
                if(op == 1){
                    printf("玩家%d质疑玩家%d!\n", curPlayer+1, prevPlayer+1);
                    printf("【上家本次打出的牌】：");
                    printCardPile(playedPile, prevPlayedCnt);
                    printf("【本回合桌面上全部出牌】：");
                    printCardPile(roundPile, roundPileSize);

                    int isTruth = checkLie(playedPile, prevPlayedCnt, roundTargetRank);
                    if(isTruth == 0){
                        printf("【质疑成功！上家说谎】玩家%d拿回本回合桌面上所有牌\n", prevPlayer+1);
                        returnCards(prevPlayer, roundPile, roundPileSize);
                        curStarter = curPlayer;
                    }else{
                        printf("【质疑失败！上家没有说谎】玩家%d拿回本回合桌面上所有牌\n", curPlayer+1);
                        returnCards(curPlayer, roundPile, roundPileSize);
                        curStarter = prevPlayer;
                    }
                    roundRunning = 0;
                }else if(op == 2){
                    // 跟牌，输入打出的牌
                    char followLine[256];
                    Card newPile[MAX_CARDS_PER_PLAYER];
                    int followCnt = 0;
                    while(1){
                        printf("玩家%d，请输入跟牌的牌（空格分隔）：\n", curPlayer+1);
                        fgets(followLine, sizeof(followLine), stdin);
                        char *ftok = strtok(followLine, " \n");
                        int fRanks[MAX_INPUT_CARDS];
                        int fCnt =0;
                        int fParseOk =1;
                        while(ftok != NULL && fCnt < MAX_INPUT_CARDS)
                        {
                            int r = strToRank(ftok);
                            if(r == -1){
                                fParseOk = 0;break;
                            }
                            fRanks[fCnt++] = r;
                            ftok = strtok(NULL, " \n");
                        }
                        if(!fParseOk || fCnt <=0)
                        {
                            printf("输入错误，重新输入！\n");
                            continue;
                        }
                        if(!removeCardsFromHand(curPlayer, fRanks, fCnt))
                        {
                            printf("手牌没有这些牌，重新输入！\n");
                            continue;
                        }
                        followCnt = fCnt;
                        for(int i=0;i<followCnt;i++)
                            newPile[i].rank = fRanks[i];
                        break;
                    }
                    printf("玩家%d宣称：%d张 %s\n", curPlayer+1, followCnt, claimBuf);
                    //追加到回合总牌堆
                    for(int i=0;i<followCnt;i++){
                        roundPile[roundPileSize++] = newPile[i];
                    }
                    //更新上家本次打出的牌
                    prevPlayer = curPlayer;
                    memcpy(playedPile, newPile, sizeof(Card)*followCnt);
                    prevPlayedCnt = followCnt;

                    if(players[curPlayer].cardCount ==0){
                        printf("玩家%d已经全部打出！等待下家操作\n", curPlayer+1);
                    }
                    curPlayer = (curPlayer +1) % playerCnt;
                }

                //胜利检测
                for(int i=0;i<playerCnt;i++){
                    if(players[i].cardCount == 0){
                        winner = i;
                        gameOver = 1;
                        roundRunning = 0;
                    }
                }
            }
            break;
        }
    }
    printf("\n====游戏结束！玩家%d获胜！====\n", winner+1);
    return 0;
}