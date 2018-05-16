#include <stdlib.h>
#include "data_types.h"
#include <stdio.h>
#include <string.h>

#define LINESIZE 256
#define PCOUNT 5
#define DELETED 0
#define NODELETED 1

void printData(Data_p);

/*вывод содержимого структуры данных*/
extern void addToList(List_p, Data_p);

//добавить элемент в список
extern void printList(List_p);

//напечатать список
extern int remove_list_elem(List_p, int);

//удаляет i-ый элемент списка
extern void invprint_list(List_p);

//Напечатать список в обратном порядке
extern void bubbleSort(List_p);

//Сортировка выбором списка
void exchange(Lnode_p, Lnode_p);

//Поменять местами указатели
void print_help();

//вывести подсказку
Tree_p addTreeNode(Tree_p, Data_p);

//добавление узла к дереву
void printTree(Tree_p);

//вывод содержимого дерева
void traverseTree(Tree_p, List_p);

//обход дерева и доавлени инфы в список
Tree_p initFromFile(Tree_p, char *, int *);

//считывает файл со списком файлов
void free_tnode(Tree_p);

// освобождение памяти выделеной под узел
int deleteTreeNode(Tree_root_p, char *);

// удаление заданного узала из дерева
Data_p findTreeNode(Tree_p, char *);

//поиск значения по ключу
Tree_p readTreeNode(Tree_p root, int *state);

/* Function to bubble sort the given linked list */
void sort(List_p list);

/* Function to swap data of two nodes a and b*/
void swap(Lnode_p a, Lnode_p b);

void printFoundData(Data_p);

int readLine(char *buff, int size, FILE *fp) {
    buff[0] = '\0';
    buff[size - 1] = '\0';             /* mark end of buffer */
    char *tmp;

    if (fgets(buff, size, fp) == NULL) {
        *buff = '\0';                   /* EOF */
        return 0;
    } else {
        /* remove newline */
        if ((tmp = strrchr(buff, '\n')) != NULL) {
            *tmp = '\0';
        }
    }
    return 1;
}

Tree_p addTreeNode(Tree_p node, Data_p data) {
    int cond;
    if (node == NULL) {
        node = (Tree_p) malloc(sizeof(Tree));
        node->info = data;
        node->left = node->right = NULL;
    } else {
        cond = strcmp(data->name, node->info->name);
        if (cond < 0)
            node->left = addTreeNode(node->left, data);
        else
            node->right = addTreeNode(node->right, data);
    }
    return node;
}

/* Фунуция выводит инфу дерева обходя прямым обходом
 * Tree_p node - указатель на корень дерева
 */
void printTree(Tree_p node) {
    if (node != NULL) {
        printData(node->info);
        printTree(node->left);
        printTree(node->right);
    }
}

/* функция обхода дерева
 * и добавления результата в список
 */
void traverseTree(Tree_p node, List_p list) {
    if (node != NULL) {
        addToList(list, node->info);
        traverseTree(node->left, list);
        traverseTree(node->right, list);
    }
}

/* Считываниет файл с именем file_name  * и записывает в дерево находящееся по указателю Tree_p
 * Формат файла:
 * <имя>.<тип> <дата создания> <последняя модификация> <размер> <количество ссылок>
 * */
Tree_p initFromFile(Tree_p root, char *file_name, int *state) {
    FILE *file;
    Data_p data;
    char *buf[PCOUNT];
    char s[LINESIZE];
    char *token;
    int i;
    *state = 0;
    file = fopen(file_name, "rt");
    if (file == NULL) {
        printf("Can't open file.");
        return NULL;
    }
    while (readLine(s, LINESIZE, file)) {
        i = 0;
        for (token = strtok(s, " "); token != NULL; token = strtok(NULL, " ")) {
            if (i < PCOUNT)
                *(buf + i++) = strdup(token);
        }
        if (i <= PCOUNT) {
            data = (Data_p) malloc(sizeof(Data));
            data->name = *buf;
            strtok(strdup(*buf), ".");
            token = strtok(NULL, ".");
            data->type = token != NULL ? token : "none";
            data->date = *(buf + 1);
            data->mod = *(buf + 2);
            data->size = (unsigned int) atoi(*(buf + 3));
            data->treat = (unsigned int) atoi(*(buf + 4));
            root = addTreeNode(root, data);
            *state = 1;
        }
    }
    return root;
}

/* удаление вершины из дерева
 * Если result = 0 - поле не найдено
 * Если result = 1 - поле найдено и удалено
 */
int deleteTreeNode(Tree_root_p root, char *key) {
    int cond, stop = 0, result;
    Tree_p cur, prev, tmp_cur, tmp_prev;
    cur = root->root;
    prev = NULL;
    result = 0;
    while (cur != NULL && stop == 0) {
        cond = strcmp(key, cur->info->name);
        if (cond < 0) {
            prev = cur;
            cur = cur->left;
        } else if (cond > 0) {
            prev = cur;
            cur = cur->right;
        } else stop = 1;
    }
    if (cur != NULL) {
        if (prev == NULL && cur->left == NULL && cur->right == NULL) {
            free(root->root);
            root->root = NULL;
        } else if (cur->right == NULL) {
            if (prev->left == cur) {
                prev->left = cur->left;
                free(cur);
            } else if (prev->right == cur) {
                prev->right = cur->left;
                free(cur);
            }
        } else if (cur->left == NULL) {
            if (prev->left == cur) {
                prev->left = cur->right;
                free(cur);
            } else if (prev->right == cur) {
                prev->right = cur->right;
                free(cur);
            }
        } else {
            tmp_prev = NULL;
            tmp_cur = cur->left;
            while (tmp_cur->right != NULL) {
                tmp_prev = tmp_cur;
                tmp_cur = tmp_cur->right;
            }
            if (tmp_prev != NULL) tmp_prev->right = NULL;
            tmp_cur->right = cur->right;
            if (prev == NULL) {
                root->root = tmp_cur;
                free(cur);
            } else if (prev->left == cur) {
                prev->left = tmp_cur;
                free(cur);
            } else if (prev->right == cur) {
                prev->right = tmp_cur;
                free(cur);
            }
        }
        result = 1;
    }
    return result;
}

/* search function search by char *key in Tree_p tree */
Data_p findTreeNode(Tree_p node, char *key) {
    int cond, stop;
    stop = 0;
    Tree_p tmp = node;
    while (tmp != NULL && stop == 0) {
        if ((cond = strcmp(key, tmp->info->name)) < 0) tmp = tmp->left;
        else if (cond > 0)tmp = tmp->right; else stop = 1;
    }
    return tmp == NULL ? NULL : tmp->info;
}

/*Считывание узла с клавиатуры и его добавление*/
Tree_p readTreeNode(Tree_p root, int *state) {
    Data_p data;
    char s[LINESIZE];
    char *token;
    data = (Data_p) malloc(sizeof(Data));
    printf("Введите имя: ");
    scanf("%s", s);
    data->name = strdup(s);
    strtok(strdup(s), ".");
    data->type = strtok(NULL, ".");
    if (data->type == NULL) data->type = "none";
    printf("Введите дату создания: ");
    scanf("%s", s);
    data->date = strdup(s);
    printf("Введите дату последней модификации: ");
    scanf("%s", s);
    data->mod = strdup(s);
    printf("Введите размер файла: ");
    scanf("%s", s);
    token = strdup(s);
    data->size = atoi(token);
    printf("Введите количество ссылок: ");
    scanf("%s", s);
    token = strdup(s);
    data->treat = atoi(token);
    root = addTreeNode(root, data);
    *state = 1;
    return root;
}

int main(int argc, char **argv) {
    system("chcp 65001");
    Data_p data = NULL;
    List_p list = NULL;
    int c, flag = 1, result = 0, loaded = 0;
    char line[LINESIZE];
    Tree_root_p troot = (Tree_root_p) malloc(sizeof(Tree_root));
    troot->root = NULL;
    print_help();
    printf("Выберите требуемое действие:\n");
    while (flag) {
        c = getchar();
        switch (c) {
            case 'r': {
                if (loaded) {
                    printf("Введите имя удаляемого файла:\n");
                    scanf("%s", line);
                    result = deleteTreeNode(troot, line);
                    if (troot->root == NULL) loaded = 0;
                    if (result) printf("Узел удален.\n"); else printf("Узел не найден.\n");
                } else printf("Необходимо загрузить список файлов с помощью 'l'.\n");
                break;
            }
            case 'l': {
                printf("Введите имя файла:\n");
                scanf("%s", line);
                troot->root = initFromFile(troot->root, line, &loaded);
                if (loaded) printf("Файл загружен.\n"); else printf("Файл незагружен.\n");
                break;
            }
            case 'p': {
                if (loaded) {
                    printf("Прямой обход дерева:\n"); //Заменить на концевой
                    //
                    printf("Name:\t\tType:\tDate:\t\tMod:\t\tSize:\tTreat:\n");
                    printTree(troot->root);
                } else printf("Необходимо загрузить список файлов с помощью 'l'.\n");
                break;
            }
            case 's': {
                if (loaded) {
                    list = (List_p) malloc(sizeof(List));
                    list->head=NULL;
                    list->tail=NULL;
                    traverseTree(troot->root, list);
                    bubbleSort(list);
                    printf("Список файлов сортированный по размеру:\n"); //попробывать по имени
                    printList(list);
                } else printf("Необходимо загрузить список файлов с помощью 'l'.\n");
                break;
            }
            case 'q': {
                flag = 0;
                break;
            }
            case 'f':
                if (loaded) {
                    printf("Введите имя файла, который хотите найти:\n");
                    scanf("%s", line);
                    data = findTreeNode(troot->root, line);
                    if (data != NULL) printFoundData(data); else printf("Файл не найден.\n");
                } else printf("Необходимо загрузить список файлов с помощью 'l'.\n");
                break;
            case 'h':
                print_help();
                break;
            case 'a':
                printf("Введите параметры файла:\n");
                troot->root = readTreeNode(troot->root, &loaded);
                break;
            default :
                printf("\nВыберите требуемое действие:\n");
                break;
        }
    }
    return 0;
}

void print_help() {
    static char *help[] = {"\nВозможные операции:", "l - загрузить файл в дерево", "a - добавить узел в дерево",
                           "r - удалить элемент из дерева", "p - напечатать концевой обход дерева",
                           "s - сортировка обменами по размеру", "f - найти элемент по ключу", "q - выйти",
                           "h - вывод этого сообщения"};
    int i;
    for (i = 0; i < 9; i++) printf("%s\n", *(help + i));
    return;
}
/* Функция добавления элемента в список
 * List_p - указатель на список
 * Date_p - указатель на
 */
void addToList(List_p list, Data_p data) {
    Lnode_p tmp;
    if (list->head==NULL) {
        //заполняем голову списка
        list->head = (Lnode_p) malloc(sizeof(Lnode));
        list->head->next = NULL;
        list->head->prev = NULL;
        list->head->info = data;
        //Указатель конца указывает пока что на голову
        list->tail = list->head;
        list->size = 1;
    } else {
        tmp = (Lnode_p) malloc(sizeof(Lnode));
        tmp->info = data;
        //предыдущий элемент равен последнему
        tmp->prev = list->tail;
        tmp->next = NULL;
        //следующий элемент последнего укаываем на новый элемент
        list->tail->next = tmp;
        //устанвливаем последний элемент равный нашему
        list->tail = tmp;
        list->size++;
    }
}
/*Функция печатает список
 * List_p лист ссылка на голову списка
 */
void printList(List_p list) {
    Lnode_p tmp = list->head;

    while(tmp!=NULL) {
        printData(tmp->info);
        tmp = tmp->next;
    }
}

/*Функция удаляет i-ый элемент из списка  *List_p list- ссылка на список  *int number - номер удаляемого элемента  * если элемент удален возращает - 0, иначе 1;  */
int remove_list_elem(List_p list, int number) {
    Lnode_p node_cur = list->head;
    Lnode_p node_prev = NULL;
    int state = NODELETED;
    int i;
    for (i = 1; i < list->size && i < number; i++) {
        node_prev = node_cur;
        node_cur = node_cur->next;
    }
    if (i == number && i > 0) {
        if (node_prev != NULL) {
            node_prev->next = node_cur->next;
            node_cur->next->prev = node_prev;
        } else {
            list->head->prev = list->head->next;
            list->head = list->head->next;
        }
        free(node_cur);
        list->size--;
        state = DELETED;
    }
    return state;
}
/* Выводит список List_p list в концевом порядке*/
void invprint_list(List_p list) {
    Lnode_p tmp = list->head;
    int i;
    while(tmp!=NULL) {
        printData(tmp->info);
        tmp = tmp->next;
    }
}

void bubbleSort(List_p list) {
    sort(list);
}

/* Bubble sort the given linked list */
void sort(List_p list)
{
    int swapped, i;
    Lnode_p ptr1;
    Lnode_p lptr = NULL;
    Lnode_p start = list->head; // связанный список
    /* Checking for empty list */
    if (start == NULL)
        return;
    do
    {
        swapped = 0;
        ptr1 = start;

        while (ptr1->next != lptr)
        {
            if (ptr1->info->size < ptr1->next->info->size)
            {
                swap(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    }
    while (swapped);
}

/* function to swap data of two nodes a and b*/
void swap(Lnode_p a, Lnode_p b)
{
    Data* temp = a->info;
    a->info = b->info;
    b->info = temp;
}

void printFoundData(Data_p data) {
    if (data != NULL) {
        printf("Name: %s; Type: %s; Date: %s; Mod: %s; Size: %u; Treat: %u\n", data->name, data->type, data->date,
               data->mod, data->size, data->treat);
    }
}

void printData(Data_p data) {
    if (data != NULL) {
        printf("%s\t%-5s\t%-5s\t%-5s\t%u\t%u\n", data->name, data->type, data->date, data->mod, data->size,
               data->treat);
    }
}