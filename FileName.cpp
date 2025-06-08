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
        printf("�޸� �Ҵ� ����\n");
        return 1;
    }

    int soupCount = 0;
    int friendship = INIT_FRIENDSHIP;
    int catPos = CAT_HOME;
    int prevPos = CAT_HOME;

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
        printStatus(soupCount, friendship);
        printRoom(catPos, prevPos);

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
        printStatus(soupCount, friendship);
        printf("%s �̵�: ����� ģ���Ҽ��� ���� ������ �� Ȯ���� �������ϴ�.\n", catName);
        int threshold = 6 - friendship;
        printf("�ֻ��� ���� %d �̻��̸� ���� ������ �̵��մϴ�.\n", threshold);
        int dice = rollDice();
        wait(500);
        printf("�ֻ����� �����ϴ�. �Ǹ���...\n");
        wait(500);
        printf("%d��(��) ���Խ��ϴ�!\n", dice);

        prevPos = catPos;
        int moveDistance = 0;

        if (dice >= threshold) {
            if (catPos < SOUP_POT) {
                moveDistance = 1;
                catPos += moveDistance;
                if (catPos > SOUP_POT) catPos = SOUP_POT;
                printf("���� ������ �����Դϴ�.\n\n");
            }
        }
        else {
            if (catPos > CAT_HOME) {
                moveDistance = 1;
                catPos -= moveDistance;
                if (catPos < CAT_HOME) catPos = CAT_HOME;
                printf("�� ������ �����Դϴ�.\n\n");
            }
        }

        if (catPos == CAT_HOME) {
            printf("%s��(��) �ڽ��� ������ ������� �����ϴ�.\n\n", catName);
        }
        else if (catPos == SOUP_POT) {
            const char* soup = getRandomSoup();
            printf("%s��(��) %s�� ��������ϴ�!\n", catName, soup);
            soupCount++;
            printf("������� ���� ����: %d��\n\n", soupCount);
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

void printStatus(int soupCount, int friendship) {
    printf("==================== ���� ���� ===================\n");
    printf("������� ���� ����: %d��\n", soupCount);
    printf("������� ����(0~4): %d\n", friendship);
    printFriendshipDesc(friendship);
    printf("==================================================\n\n");
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
    if (r == 0) return "���ڼ���";
    else if (r == 1) return "����̼���";
    else return "����ݸ�����";
}
