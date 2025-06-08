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
        printf("�޸� �Ҵ� ����\n");
        return 1;
    }

    int soupCount = 0;
    int friendship = INIT_FRIENDSHIP;
    int catPos = CAT_HOME;
    int prevPos = CAT_HOME;
    int mood = 3;
    int threshold = 0;

    printf("**** �߿��̿� ���� ****\n");
    printCatArt();

    printf("�߿����� �̸��� �����ּ���: ");
    if (fgets(catName, 20, stdin) == NULL) {
        printf("�Է� ���� �߻�. ���α׷��� �����մϴ�.\n");
        free(catName);
        return 1;
    }

    catName[strcspn(catName, "\n")] = '\0';
    printf("�߿����� �̸��� %s�Դϴ�.\n", catName);
    Sleep(1000);
    clearScreen();

    while (1) {
        printStatus(soupCount, friendship, catName, mood);

        threshold = 6 - friendship;
        int moodDice = rollDice();
        printf("6-%d: %d�����̸� �׳� ����� �������ϴ�.\n", friendship, threshold);
        printf("�ֻ����� �����ϴ�. �Ǹ���...\n");
        wait(1500);
        printf("%d��(��) ���Խ��ϴ�!\n", moodDice);

        if (moodDice <= threshold) {
            if (mood > 0) {
                printf("%s�� ����� �������ϴ�: %d �� %d\n", catName, mood, mood - 1);
                mood--;
            }
        }

        int choice = getInteractionChoice();

        if (choice == 0) {
            printf("�ƹ��͵� ���� �ʽ��ϴ�.\n");
            printf("4/6�� Ȯ���� ģ�е��� �������ϴ�.\n");
            wait(500);
            int dice = rollDice();
            printf("�ֻ����� �����ϴ�. �Ǹ���...\n");
            wait(500);
            printf("%d��(��) ���Խ��ϴ�!\n", dice);
            if (dice <= 4) {
                if (friendship > 0) friendship--;
                printf("ģ�е��� �������ϴ�.\n");
            }
            else {
                printf("������ ģ�е��� �������� �ʾҽ��ϴ�.\n");
            }
        }
        else if (choice == 1) {
            printf("%s�� ���� �ܾ��־����ϴ�.\n", catName);
            printf("2/6�� Ȯ���� ģ�е��� �������ϴ�.\n");
            wait(500);
            int dice = rollDice();
            printf("�ֻ����� �����ϴ�. �Ǹ���...\n");
            wait(500);
            printf("%d��(��) ���Խ��ϴ�!\n", dice);
            if (dice >= 5) {
                if (friendship < 4) friendship++;
                printf("ģ�е��� �������ϴ�.\n");
            }
            else {
                printf("ģ�е��� �״���Դϴ�.\n");
            }
        }

        printf("���� ģ�е�: %d\n", friendship);
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
            printf("����� �ſ� ���� %s��(��) ������ ���մϴ�.\n\n", catName);
        }
        else {
            printf("%s��(��) �̹� ���� �ֽ��ϴ�. ���ڸ��� �ӹ��ϴ�.\n\n", catName);
        }
    }
    else if (*mood == 1) {
        if (SCRATCHER_POS > 0 && SCRATCHER_POS < MAP_WIDTH - 1) {
            if (currentPos < SCRATCHER_POS) {
                (*catPos)++;
                printf("%s��(��) �ɽ��ؼ� ��ũ��ó������ �̵��մϴ�.\n\n", catName);
            }
            else if (currentPos > SCRATCHER_POS) {
                (*catPos)--;
                printf("%s��(��) �ɽ��ؼ� ��ũ��ó������ �̵��մϴ�.\n\n", catName);
            }
            else {
                printf("%s��(��) ��ũ��ó �տ� �̹� �ֽ��ϴ�. ���ڸ��� �ӹ��ϴ�.\n\n", catName);
            }
        }
        else {
            printf("��Ÿ��� ��� ����� �ſ� �������ϴ�.\n\n");
            if (*mood > 0) (*mood)--;
        }
    }
    else if (*mood == 2) {
        printf("%s��(��) ��� ���� �Ļ��� ���� �ֽ��ϴ�.\n\n", catName);
    }
    else if (*mood == 3) {
        if (currentPos < SOUP_POT) {
            (*catPos)++;
            printf("%s��(��) ������ �θ��� ������ ���鷯 ���ϴ�.\n\n", catName);
        }
        else {
            printf("%s��(��) �̹� ���� �տ� �ֽ��ϴ�. ���ڸ��� �ӹ��ϴ�.\n\n", catName);
        }
    }

    if (*catPos == SOUP_POT && currentPos != SOUP_POT) {
        const char* soup = getRandomSoup();
        printf("%s��(��) %s�� ��������ϴ�!\n", catName, soup);
        (*soupCount)++;
        printf("������� ���� ����: %d��\n\n", *soupCount);
    }

    printRoom(*catPos, prevPos);
    wait(3000);
    clearScreen();
}

void printCatArt() {
    printf("\n");
    printf("      ������ ��\n");
    printf("     | ��_��_| \n");
    printf("    �� �ߣ�x��\n");
    printf("   /�������� | \n");
    printf("  /�� ;���� ;\n");
    printf(" ������|��|��| \n");
    printf(" /�ߣ�|,,|��| \n");
    printf(" ��죪�\n");
    printf("\n");
}

void clearScreen() {
    system("cls");
}

void printStatus(int soupCount, int friendship, const char* catName, int mood) {
    printf("==================== ���� ���� ===================\n");
    printf("������� ���� ����: %d��\n", soupCount);
    printf("%s�� ���(0~3): %d\n", catName, mood);
    printMoodDesc(mood);
    printf("������� ����(0~4): %d\n", friendship);
    printFriendshipDesc(friendship);
    printf("==================================================\n\n");
}

void printMoodDesc(int mood) {
    switch (mood) {
    case 0: printf("����� �ſ� ���޴ϴ�.\n"); break;
    case 1: printf("�ɽ����մϴ�.\n"); break;
    case 2: printf("�Ļ��� �����ϴ�.\n"); break;
    case 3: printf("������ �θ��ϴ�.\n"); break;
    }
}

void printFriendshipDesc(int friendship) {
    if (friendship == 0)
        printf("�翡 ���� ������ �Ⱦ��մϴ�.\n");
    else if (friendship == 1)
        printf("���� ���Ǳ� ����Դϴ�.\n");
    else if (friendship == 2)
        printf("�׷����� ������ �����Դϴ�.\n");
    else if (friendship == 3)
        printf("�Ǹ��� ����� �����ް� �ֽ��ϴ�.\n");
    else if (friendship == 4)
        printf("���� �������Դϴ�.\n");
}

int getInteractionChoice() {
    int choice;
    int firstPrompt = 1;

    while (1) {
        if (firstPrompt) {
            printf("\n� ��ȣ�ۿ��� �Ͻðڽ��ϱ�? 0. �ƹ��͵� ���� ����  1. �ܾ��ֱ�\n>> ");
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
    if (r == 0) return "���ڼ���";
    else if (r == 1) return "����̼���";
    else return "����ݸ�����";
}
