#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <string.h>

#define MAP_WIDTH 10
#define CAT_HOME 1
#define SOUP_POT 8
#define SCRATCHER_POS 5
#define INIT_FRIENDSHIP 2

void printCatArt();
void clearScreen();
void printStatus(int soupCount, int friendship, const char* catName, int mood);
void printMoodDesc(int mood);
void printRoom(int catPos, int prevPos);
void printFriendshipDesc(int friendship);
int getInteractionChoice();
int rollDice();
void wait(int ms);
const char* getRandomSoup();
void moveCat(int* catPos, int* mood, int* soupCount, const char* catName, int prevPos);

int main() {
    srand((unsigned int)time(NULL));

    char* catName = (char*)malloc(20);
    if (catName == NULL) {
        printf("메모리 할당 실패\n");
        return 1;
    }

    int soupCount = 0;
    int friendship = INIT_FRIENDSHIP;
    int catPos = CAT_HOME;
    int prevPos = CAT_HOME;
    int mood = 3;
    int threshold = 0;

    printf("**** 야옹이와 수프 ****\n");
    printCatArt();

    printf("야옹이의 이름을 지어주세요: ");
    if (fgets(catName, 20, stdin) == NULL) {
        printf("입력 오류 발생. 프로그램을 종료합니다.\n");
        free(catName);
        return 1;
    }

    catName[strcspn(catName, "\n")] = '\0';
    printf("야옹이의 이름은 %s입니다.\n", catName);
    Sleep(1000);
    clearScreen();

    while (1) {
        printStatus(soupCount, friendship, catName, mood);

        threshold = 6 - friendship;
        int moodDice = rollDice();
        printf("6-%d: %d이하이면 그냥 기분이 나빠집니다.\n", friendship, threshold);
        printf("주사위를 굴립니다. 또르르...\n");
        wait(1500);
        printf("%d이(가) 나왔습니다!\n", moodDice);

        if (moodDice <= threshold) {
            if (mood > 0) {
                printf("%s의 기분이 나빠집니다: %d → %d\n", catName, mood, mood - 1);
                mood--;
            }
        }

        int choice = getInteractionChoice();

        if (choice == 0) {
            printf("아무것도 하지 않습니다.\n");
            printf("4/6의 확률로 친밀도가 떨어집니다.\n");
            wait(500);
            int dice = rollDice();
            printf("주사위를 굴립니다. 또르르...\n");
            wait(500);
            printf("%d이(가) 나왔습니다!\n", dice);
            if (dice <= 4) {
                if (friendship > 0) friendship--;
                printf("친밀도가 떨어집니다.\n");
            }
            else {
                printf("다행히 친밀도가 떨어지지 않았습니다.\n");
            }
        }
        else if (choice == 1) {
            printf("%s의 턱을 긁어주었습니다.\n", catName);
            printf("2/6의 확률로 친밀도가 높아집니다.\n");
            wait(500);
            int dice = rollDice();
            printf("주사위를 굴립니다. 또르르...\n");
            wait(500);
            printf("%d이(가) 나왔습니다!\n", dice);
            if (dice >= 5) {
                if (friendship < 4) friendship++;
                printf("친밀도가 높아집니다.\n");
            }
            else {
                printf("친밀도는 그대로입니다.\n");
            }
        }

        printf("현재 친밀도: %d\n", friendship);
        wait(1000);

        clearScreen();
        printStatus(soupCount, friendship, catName, mood);

        prevPos = catPos;
        moveCat(&catPos, &mood, &soupCount, catName, prevPos);
    }

    free(catName);
    return 0;
}

void moveCat(int* catPos, int* mood, int* soupCount, const char* catName, int prevPos) {
    int currentPos = *catPos;

    if (*mood == 0) {
        if (currentPos > CAT_HOME) {
            (*catPos)--;
            printf("기분이 매우 나쁜 %s은(는) 집으로 향합니다.\n\n", catName);
        }
        else {
            printf("%s은(는) 이미 집에 있습니다. 제자리에 머뭅니다.\n\n", catName);
        }
    }
    else if (*mood == 1) {
        if (SCRATCHER_POS > 0 && SCRATCHER_POS < MAP_WIDTH - 1) {
            if (currentPos < SCRATCHER_POS) {
                (*catPos)++;
                printf("%s은(는) 심심해서 스크래처쪽으로 이동합니다.\n\n", catName);
            }
            else if (currentPos > SCRATCHER_POS) {
                (*catPos)--;
                printf("%s은(는) 심심해서 스크래처쪽으로 이동합니다.\n\n", catName);
            }
            else {
                printf("%s은(는) 스크래처 앞에 이미 있습니다. 제자리에 머뭅니다.\n\n", catName);
            }
        }
        else {
            printf("놀거리가 없어서 기분이 매우 나빠집니다.\n\n");
            if (*mood > 0) (*mood)--;
        }
    }
    else if (*mood == 2) {
        printf("%s은(는) 기분 좋게 식빵을 굽고 있습니다.\n\n", catName);
    }
    else if (*mood == 3) {
        if (currentPos < SOUP_POT) {
            (*catPos)++;
            printf("%s은(는) 골골송을 부르며 수프를 만들러 갑니다.\n\n", catName);
        }
        else {
            printf("%s은(는) 이미 냄비 앞에 있습니다. 제자리에 머뭅니다.\n\n", catName);
        }
    }

    if (*catPos == SOUP_POT && currentPos != SOUP_POT) {
        const char* soup = getRandomSoup();
        printf("%s이(가) %s를 만들었습니다!\n", catName, soup);
        (*soupCount)++;
        printf("현재까지 만든 수프: %d개\n\n", *soupCount);
    }

    printRoom(*catPos, prevPos);
    wait(3000);
    clearScreen();
}

void printCatArt() {
    printf("\n");
    printf("      ／＞― フ\n");
    printf("     | 　_　_| \n");
    printf("    ／ ミ＿xノ\n");
    printf("   /　　　　 | \n");
    printf("  /　 ;　　 ;\n");
    printf(" │　　|　|　| \n");
    printf(" /＿＿|,,|＿| \n");
    printf(" ＼二つ\n");
    printf("\n");
}

void clearScreen() {
    system("cls");
}

void printStatus(int soupCount, int friendship, const char* catName, int mood) {
    printf("==================== 현재 상태 ===================\n");
    printf("현재까지 만든 수프: %d개\n", soupCount);
    printf("%s이 기분(0~3): %d\n", catName, mood);
    printMoodDesc(mood);
    printf("집사와의 관계(0~4): %d\n", friendship);
    printFriendshipDesc(friendship);
    printf("==================================================\n\n");
}

void printMoodDesc(int mood) {
    switch (mood) {
    case 0: printf("기분이 매우 나쁩니다.\n"); break;
    case 1: printf("심심해합니다.\n"); break;
    case 2: printf("식빵을 굽습니다.\n"); break;
    case 3: printf("골골송을 부릅니다.\n"); break;
    }
}

void printFriendshipDesc(int friendship) {
    if (friendship == 0)
        printf("곁에 오는 것조차 싫어합니다.\n");
    else if (friendship == 1)
        printf("간식 자판기 취급입니다.\n");
    else if (friendship == 2)
        printf("그럭저럭 쓸만한 집사입니다.\n");
    else if (friendship == 3)
        printf("훌륭한 집사로 인정받고 있습니다.\n");
    else if (friendship == 4)
        printf("집사 껌딱지입니다.\n");
}

int getInteractionChoice() {
    int choice;
    int firstPrompt = 1;

    while (1) {
        if (firstPrompt) {
            printf("\n어떤 상호작용을 하시겠습니까? 0. 아무것도 하지 않음  1. 긁어주기\n>> ");
            firstPrompt = 0;
        }
        else {
            printf(">> ");
        }

        if (scanf_s("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }

        if (choice == 0 || choice == 1) break;
    }

    return choice;
}

int rollDice() {
    return rand() % 6 + 1;
}

void wait(int ms) {
    Sleep(ms);
}

void printRoom(int catPos, int prevPos) {
    printf(" ##########\n");
    printf(" #H      B#\n");
    printf(" #");

    for (int i = 1; i < MAP_WIDTH - 1; i++) {
        if (i == catPos)
            printf("C");
        else if (i == prevPos)
            printf(".");
        else
            printf(" ");
    }

    printf("#\n ##########\n");
}

const char* getRandomSoup() {
    int r = rand() % 3;
    if (r == 0) return "감자수프";
    else if (r == 1) return "양송이수프";
    else return "브로콜리수프";
}
