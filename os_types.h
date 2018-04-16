#ifndef FILES_FILE_H
#define FILES_FILE_H

typedef struct data {
    char* name; /*имя файла формат <имя> не более 20 символов*/
    char* type; /*тип файла - расширение файла, неболее 10 символов*/
    char* date; /*дата создания файла*/
    char* mod;  /*дата посдедней модификации*/
    unsigned size; /*размер файла*/
    unsigned treat; //Количество ссылок
} FileData;

typedef struct tree {
    struct FileData* info;
    struct tree* left;
    struct tree* right;
} TreeNode;

typedef struct {
    TreeNode* root;
} Tree;

#endif //FILES_FILE_H
