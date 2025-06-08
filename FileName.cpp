#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <string.h>

#define MAP_WIDTH 10
#define CAT_HOME 1
#define SOUP_POT 8
#define INIT_FRIENDSHIP 2

void printCatArt();
void clearScreen();
void printStatus(int soupCount, int friendship);
void printRoom(int catPos, int prevPos);
void printFriendshipDesc(int friendship);
int getInteractionChoice();
int rollDice();
void wait(int ms);
const char* getRandomSoup();

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
        printStatus(soupCount, friendship);
        printRoom(catPos, prevPos);

        int choice = getInteractionChoice();

        if (choice == 0) {
            printf("아무것도 하지 않습니다.\n");
            printf("4/6의 확률로 친밀도가 떨어집니다.\n");
            wait(500);
            int dice = rollDice();
            printf("주사위를 굴립니다. 또르륵...\n");
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
            printf("주사위를 굴립니다. 또르륵...\n");
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
        printStatus(soupCount, friendship);
        printf("%s 이동: 집사와 친밀할수록 냄비 쪽으로 갈 확률이 높아집니다.\n", catName);
        int threshold = 6 - friendship;
        printf("주사위 눈이 %d 이상이면 냄비 쪽으로 이동합니다.\n", threshold);
        int dice = rollDice();
        wait(500);
        printf("주사위를 굴립니다. 또르륵...\n");
        wait(500);
        printf("%d이(가) 나왔습니다!\n", dice);

        prevPos = catPos;
        int moveDistance = 0;

        if (dice >= threshold) {
            if (catPos < SOUP_POT) {
                moveDistance = 1;
                catPos += moveDistance;
                if (catPos > SOUP_POT) catPos = SOUP_POT;
                printf("냄비 쪽으로 움직입니다.\n\n");
            }
        }
        else {
            if (catPos > CAT_HOME) {
                moveDistance = 1;
                catPos -= moveDistance;
                if (catPos < CAT_HOME) catPos = CAT_HOME;
                printf("집 쪽으로 움직입니다.\n\n");
            }
        }

        if (catPos == CAT_HOME) {
            printf("%s은(는) 자신의 집에서 편안함을 느낍니다.\n\n", catName);
        }
        else if (catPos == SOUP_POT) {
            const char* soup = getRandomSoup();
            printf("%s이(가) %s를 만들었습니다!\n", catName, soup);
            soupCount++;
            printf("현재까지 만든 수프: %d개\n\n", soupCount);
        }

        printRoom(catPos, prevPos);
        wait(2500);
        clearScreen();
    }

    free(catName);
    return 0;
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

void printStatus(int soupCount, int friendship) {
    printf("==================== 현재 상태 ===================\n");
    printf("현재까지 만든 수프: %d개\n", soupCount);
    printf("집사와의 관계(0~4): %d\n", friendship);
    printFriendshipDesc(friendship);
    printf("==================================================\n\n");
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

        if (choice == 0 || choice == 1) {
            break;
        }
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
