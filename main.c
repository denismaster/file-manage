#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <conio.h>
#include "os_types.h"

//Код клавиши ввверх
#define KEY_UP 72
//Код клавиши вниз
#define KEY_DOWN 80
//Код клавиши влево
#define KEY_LEFT 75
//Код клавиши вправо
#define KEY_RIGHT 77
#define KEY_ENTER 13

int ActiveItem; //Элемент меню активный
//Максимальное количество элементов в главном меню
#define MAX_MAINMENU 6
//Минимальное количество элементов в главном меню
#define MIN_MAINMENU 1


void clearScreen() {
    system("@cls||clear");
}

char *drawTableMenuArrow(int n) {
    if (n == ActiveItem)
        return "> ";
    else
        return " ";
}

void drawMainMenu() {
    printf("Программа для управления файлами!\n");
    printf("Выберите действие:\n");
    printf("%sПросмотр данных\n", drawTableMenuArrow(1));
    printf("%sДобавление данных\n", drawTableMenuArrow(2));
    printf("%sРедактирование данных\n", drawTableMenuArrow(3));
    printf("%sУдаление данных\n", drawTableMenuArrow(4));
    printf("%sСортировать данные\n", drawTableMenuArrow(5));
    printf("%sВыход\n", drawTableMenuArrow(6));
}

void doMenuAction(int n)
{

    switch (n)
    {
        case 6: exit(0); break;
    }
}

void processMainMenu() {
    int k = 0;
    int c1 = 0;
    ActiveItem = 1;
    while (k != 1) {
        clearScreen();
        drawMainMenu();
        c1 = _getch();
        if (!c1) {
            c1 = _getch();
        }
        switch (c1) {
            case KEY_UP:
                if (ActiveItem > MIN_MAINMENU)
                    ActiveItem--;
                break;
            case KEY_DOWN:
                if (ActiveItem < MAX_MAINMENU)
                    ActiveItem++;
                break;
            case KEY_ENTER: doMenuAction(ActiveItem); break;
        }
    }

}

int main() {
    system("chcp 65001");
    processMainMenu();
    return 0;
}