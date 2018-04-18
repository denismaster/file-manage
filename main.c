#include <stdlib.h>
#include "data_types.h"
#include <stdio.h>
#include <string.h>

#define LINESIZE 256
#define PCOUNT 5
#define DELETED 0
#define NODELETED 1

void print_data(Data_p);

/*вывод содержимого структуры данных*/
extern void add_to_list(List_p, Data_p);

//добавить элемент в список
extern void print_list(List_p);

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
Tree_p add_tnode(Tree_p, Data_p);

//добавление узла к дереву
void printTree(Tree_p);

//вывод содержимого дерева
void infix_travers(Tree_p, List_p);

//обход дерева и доавлени инфы в список
Tree_p init_tree(Tree_p, char *, int *);

//считывает файл со списком файлов
void free_tnode(Tree_p);

// освобождение памяти выделеной под узел
int my_delete_tnode(Tree_root_p, char *);

// удаление заданного узала из дерева
Data_p search_tnode(Tree_p, char *);

//поиск значения по ключу
Tree_p read_tnode(Tree_p root, int *state);

void print_search_data(Data_p);

Tree_p add_tnode(Tree_p node, Data_p data) {
    int cond;
    if (node == NULL) {
        node = (Tree_p) malloc(sizeof(Tree));
        node->info = data;
        node->left = node->right = NULL;
    } else if ((cond = strcmp(data->name, node->info->name)) == 0) {}
    else if (cond < 0)
        node->left = add_tnode(node->left, data);
    else node->right = add_tnode(node->right, data);
    return node;

} /* Фунуция выводит инфу дерева обходя прямым обходом  * Tree_p node - указатель на корень дерева*/
void printTree(Tree_p node) {
    if (node != NULL) {
        print_data(node->info);
        printTree(node->left);
        printTree(node->right);
    }
}

//printf("Name: %s; Type: %s; Date: %s; Mod: %s; Size: %u; Treat: %u\n"
// /*функция обхода конченного дерева 	 * *и добавления результата в список*/
void infix_travers(Tree_p node, List_p list) {
    if (node != NULL) {
        add_to_list(list, node->info);
        infix_travers(node->left, list);
        infix_travers(node->right, list);
    }
} /* Считываниет файл с именем file_name  * и записывает в дерево находящееся по указателю Tree_p  * формат файла:  * <имя> <тип> <дата создания> <последняя модификация> <размер> <количество ссылок> */ Tree_p
init_tree(Tree_p root, char *file_name, int *state) {
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
    while (fgets(s, LINESIZE, file)) {
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
            root = add_tnode(root, data);
            *state = 1;
        }
    }
    return root;
} /*удаление вершины из дерева  * Если result = 0 - поле не найдено  * Если result = 1 - поле найдено и удалено  */ int
my_delete_tnode(Tree_root_p root, char *key) {
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
} /*search function search by char *key in Tree_p tree*/
Data_p search_tnode(Tree_p node, char *key) {
    int cond, stop;
    stop = 0;
    Tree_p tmp = node;
    while (tmp != NULL && stop == 0) {
        if ((cond = strcmp(key, tmp->info->name)) < 0) tmp = tmp->left;
        else if (cond > 0)tmp = tmp->right; else stop = 1;
    }
    return tmp == NULL ? NULL : tmp->info;
} /*Считывание узла с клавиатуры и его добавление*/
Tree_p read_tnode(Tree_p root, int *state) {
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
    root = add_tnode(root, data);
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
    print_help();
    printf("Выберите требуемое действие:\n");
    while (flag) {
        c = getchar();
        switch (c) {
            case 'r': {
                if (loaded) {
                    printf("Введите имя удаляемого файла:\n");
                    scanf("%s", line);
                    result = my_delete_tnode(troot, line);
                    if (troot->root == NULL) loaded = 0;
                    if (result) printf("Узел удален.\n"); else printf("Узел не найден.\n");
                } else printf("Необходимо загрузить список файлов с помощью 'l'.\n");
                break;
            }
            case 'l': {
                printf("Введите имя файла:\n");
                scanf("%s", line);
                troot->root = init_tree(troot->root, line, &loaded);
                if (loaded) printf("Файл загружен.\n"); else printf("Файл незагружен.\n");
                break;
            }
            case 'p': {
                if (loaded) {
                    printf("Прямой обход дерева:\n");
                    //
                    printf("Name:\t\tType:\tDate:\t\tMod:\t\tSize:\tTreat:\n");
                    printTree(troot->root);
                } else printf("Необходимо загрузить список файлов с помощью 'l'.\n");
                break;
            }
            case 's': {
                if (loaded) {
                    list = (List_p) malloc(sizeof(List));
                    infix_travers(troot->root, list);
                    bubbleSort(list);
                    printf("Список файлов сортированный по размеру:\n"); //попробывать по имени
                    print_list(list);
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
                    data = search_tnode(troot->root, line);
                    if (data != NULL) print_search_data(data); else printf("Файл не найден.\n");
                } else printf("Необходимо загрузить список файлов с помощью 'l'.\n");
                break;
            case 'h':
                print_help();
                break;
            case 'a':
                printf("Введите параметры файла:\n");
                troot->root = read_tnode(troot->root, &loaded);
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
                           "r - удалить элемент из дерева", "p - напечатать прямой обход дерева",
                           "s - сортировка обменами по размеру", "f - найти элемент по ключу", "q - выйти",
                           "h - вывод этого сообщения"};
    int i;
    for (i = 0; i < 9; i++) printf("%s\n", *(help + i));
    return;
} /*Функция добавления элемента в список  * List_p - указатель на список  * Date_p - указатель на  */ extern void
add_to_list(List_p list, Data_p data) {
    Lnode_p tmp;
    if (!list->head) {
        list->head = (Lnode_p) malloc(sizeof(Lnode));
        list->head->next = list->head;
        list->head->prev = list->head;
        list->head->info = data;
        list->size = 0;
    } else {
        tmp = (Lnode_p) malloc(sizeof(Lnode));
        tmp->info = data;
        tmp->next = list->head;
        tmp->prev = list->head->prev;
        list->head->prev = tmp;
        tmp->prev->next = tmp;
        list->size++;
    }
} /*Функция печатает список  * List_p лист ссылка на голову списка  */
extern void print_list(List_p list) {
    Lnode_p tmp = list->head->next;
    int i;
    print_data(list->head->info);
    for (i = 1; tmp != list->head; i++) {
        print_data(tmp->info);
        tmp = tmp->next;
    }
} /*Функция удаляет i-ый элемент из списка  *List_p list- ссылка на список  *int number - номер удаляемого элемента  * если элемент удален возращает - 0, иначе 1;  */
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
}  /*Функция выводит список List_p list в концевом порядке*/
void invprint_list(List_p list) {
    Lnode_p tmp = list->head->prev;
    int i;
    for (i = list->size; tmp != list->head; i--) {
        print_data(tmp->info);
        tmp = tmp->prev;
    }
    print_data(list->head->info);
}

void bubbleSort(List_p list) {
    Data_p temp;
    Lnode_p head = list->head; // связанный список
    Lnode_p node, node2;

    for (node = head; node; node = node->next)
        for (node2 = head; node2; node2 = node2->next)
            if (node->info->size > node2->info->size) { // если число из node меньше числа из node2 то переставляем их
                temp = node->info;
                node->info = node2->info;
                node2->info = temp;
            }
}

void print_search_data(Data_p data) {
    if (data != NULL) {
        printf("Name: %s; Type: %s; Date: %s; Mod: %s; Size: %u; Treat: %u\n", data->name, data->type, data->date,
               data->mod, data->size, data->treat);
    }
}

void print_data(Data_p data) {
    if (data != NULL) {
        printf("%s\t%-5s\t%-5s\t%-5s\t%u\t%u\n", data->name, data->type, data->date, data->mod, data->size,
               data->treat);
    }
}