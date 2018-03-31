#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <conio.h>

int ActiveItem; //Элемент меню активный
#define KEY_UP = 72;//Код клавиши ввверх
#define KEY_DOWN = 80;//Код клавиши вниз
#define KEY_LEFT = 75;//Код клавиши влево
#define KEY_RIGHT = 77;//Код клавиши вправо
#define KEY_ENTER = 13;

#define MAX_MAINMENU = 6; //Максимальное количество элементов в главном меню
#define MIN_MAINMENU = 1; //Минимальное количество элементов в главном меню
#define MAX_VIEWTABLEMENU = 8;//Максимальное количество элементов в меню таблицы
#define MIN_VIEWTABLEMENU = 1;//Минимальное количество элементов в меню таблицы


void clearScreen()
{
    system("@cls||clear");
}

void drawMainMenu()
{
    printf("Программа для управления файлами!\n");
    printf("Выберите действие:\n");
    printf("1)Просмотр данных\n");
    printf("2)Добавление данных\n");
    printf("3)Редактирование данных\n");
    printf("4)Удаление данных\n");
    printf("5)Сортировать данные\n");
    printf("6)Выход\n");
}


void processMainMenu(){
    int k=0;
    char c1=0;
    ActiveItem = 1;
    while (k != 1)
    {
        clearScreen();
        drawMainMenu();
        c1 = _getch();
        if (!c1)
        {
            c1 = _getch();
        }
        switch (c1)
        {
            case KEY_UP: if (ActiveItem > MIN_MAINMENU) ActiveItem--; break;
            case KEY_DOWN: if (ActiveItem < MAX_MAINMENU) ActiveItem++; break;
            //case KEY_ENTER: break;
        }
    }

}

int main()
{
    system("chcp 1251");
    processMainMenu();
    return 0;
}