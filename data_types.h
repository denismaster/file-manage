//
// Created by denismaster on 17.04.2018.
//

#ifndef FILES_DATA_TYPES_H
#define FILES_DATA_TYPES_H
#ifndef DATA_TYPES_H
#define    DATA_TYPES_H
#ifdef    __cplusplus
extern "C" {
#endif
#ifdef    __cplusplus
}
#endif
#endif    /* DATA_TYPES_H */
/*Описание структур данных*/
typedef struct data {
    char *name; /*имя файла формат <имя> не более 20 символов*/
    char *type; /*тип файла - расширение файла, неболее 10 символов*/
    char *date; /*дата создания файла*/
    char *mod;  /*дата посдедней модификации*/
    unsigned size; /*размер файла*/
    unsigned treat; //Количество ссылок
} Data;
typedef struct data *Data_p;
typedef struct list_node {
    struct data *info;
    struct list_node *next;
    struct list_node *prev;
} Lnode;
typedef struct list_node *Lnode_p;
typedef struct list {
    Lnode_p head;
    unsigned size;
} List;
typedef struct list *List_p;
typedef struct tree {
    struct data *info;
    struct tree *left;
    struct tree *right;
} Tree;
typedef struct tree *Tree_p;
typedef struct tree_root {
    struct tree *root;
} Tree_root;
typedef struct tree_root *Tree_root_p;
/*конец описания структур данных*/
/*Описание прототипов функций*/
/* Функции для работы со списками */
void add_to_list(List_p, Data_p); //добавить элемент в список
void print_list(List_p);         //напечатать список
void delete_list(List_p);         //удалить список
int remove_list_elem(List_p, int); //удаляет i-ый элемент списка
void insertion_sort(List_p); //Сортировка вставками списка
void invprint_list(List_p); //Напечатать список в обратном порядке
/* Конец объявления функций для списков */
/*Функции для раюоты с деревом*/
Tree_p add_tnode(Tree_p, Data_p); //добавление узла к дереву
void print_tree(Tree_p); //вывод содержимого дерева
void infix_travers(Tree_p, List_p); //обход дерева и доавлени инфы в список
Tree_p init_tree(Tree_p, char *, int *); //считывает файл со списком файлов
void destroy_tnode(Tree_p); // освобождение памяти выделеной под узел
int my_delete_tnode(Tree_root_p, char *); // удаление заданного узала из дерева
Data_p search_tnode(Tree_p, char *); //поиск значения по ключу
Tree_p read_tnode(Tree_p root, int *state);
/* Конец объявления функций для работы с деревом */
/*Функции для работы с данными*/
void print_data(Data_p); /*вывод содержимого структуры данных*/
/*конец описания деклараций функций*/

#endif //FILES_DATA_TYPES_H
