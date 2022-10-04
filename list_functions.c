//
// Created by artem on 15.10.2020.
//



#include <stdio.h>
#include <stdlib.h>
#include "mylist.h"
#include "ship_header.h"


node * init_node()
{
    node *list;

    if ((list = (node*)malloc(sizeof(node))) == NULL)
        exit(1);
    list->next = NULL;
    list->prev = NULL;
    return(list);
}

void delete_node(node *ship)
{
    delete_info(ship->data);
    free(ship);
}

node *load(FILE *fptr)
{
    node *ptr_now = NULL, *ptr_prev = NULL;

    while (endin(fptr) != 1)
    {
        ptr_now = init_node();
        ptr_now->data = Fread_info(fptr);
        ptr_now->prev = ptr_prev;
        ptr_now->next = NULL;
        if (ptr_prev != NULL)
            ptr_prev->next = ptr_now;
        ptr_prev = ptr_now;
    }

    return ptr_now;
}

node *search_and_delete(node *head)
{
    if (head == NULL) {
        printf("ПУСТАЯ СТРУКТУРА!!!\n");
        getchar();  //для игнорирования '\n'
        return NULL;
    }
    else
        head = tohead(head);


    int id;
    char *patt;

    printf("1) название корабля\n");
    printf("2) название верфи изготовителя\n");
    printf("3) год спуска на воду\n");
    printf("4) история участия в военных компаниях\n");
    printf("5) численность экипажа\n");
    printf("6) состояние корабля\n");

    printf("введите идентификатор строки по которой хотите осуществить поиск : ");
    id = read_int(stdin);

    printf("введите данные по которым будет производится удаление : ");
    patt = read_str(stdin);

    getchar(); //игнорирование последущего \n

    node *list_ptr, *delete_ptr/*в случае краевого условия когда удаляем последний элемент*/;
    while (head != NULL)
        if (find(id ,patt, head->data))
        {
            printf("DELETED:\n");
            Uwrite_info(head->data);
            printf("\n");

            if (head->prev != NULL)
                head->prev->next = head->next;
            if (head->next != NULL)
                head->next->prev = head->prev;

            list_ptr = head->prev;
            delete_ptr = head;
            head = head->next;
            delete_node(delete_ptr);
        }
        else
        {
            list_ptr = head;
            head = head->next;
        }
    free(patt);

    return list_ptr;
}

node *add(node *tail)
{
    int counter = 0;
    char status;

    if (tail != NULL)
        tail = totail(tail);

    do {
        node *temp_list = init_node();
        temp_list->data = Uread_info();
        temp_list->prev = tail;
        if (tail != NULL)
            tail->next = temp_list;
        temp_list->next = NULL;
        tail = temp_list;

        counter++;
        printf("записаны %d структур, продолжить ввод?(Y\\N): ", counter);
        status = getchar();
        if ((status != 'y') && (status != 'Y') && (status != 'N') && (status != 'n'))
            exit(5);
    } while ((status != 'N') && (status != 'n'));

    printf("ВСЕГО ЗАПИСАНО: %d \n\n", counter);

    getchar();
    return tail;
}

int write_list(FILE* fp, node* head)
{
    if (head == NULL)
        return 0;
    else
        head = tohead(head);

    node *ptr_for_delete;
    while (head != NULL)
    {
        ptr_for_delete = head;
        Fwrite_info(head->data, fp);
        head = head->next;
        delete_node(ptr_for_delete);
    }
}

int find_ships(node *head)
{
    if (head == NULL) {
        printf("ПУСТАЯ СТРУКТУРА!!!\n");
        getchar();  //для игнорирования '\n'
        return 0;
    }
    else
        head = tohead(head);

    int id;
    char *patt;

    printf("1) название корабля\n");
    printf("2) название верфи изготовителя\n");
    printf("3) год спуска на воду\n");
    printf("4) история участия в военных компаниях\n");
    printf("5) численность экипажа\n");
    printf("6) состояние корабля\n");

    printf("введите идентификатор строки по которой хотите осуществить поиск : ");
    id = read_int(stdin);

    printf("введите данные по которым будет производится поиск : ");
    patt = read_str(stdin);
    getchar();  //для игнорирования '\n'

    int counter =0;
    while (head != NULL)
    {
        if (find(id, patt, head->data))
        {
            counter++;
            printf("Найдено %d совпадение: \n", counter);
            Uwrite_info(head->data);
        }

        head = head->next;
    }
    free(patt);
    printf("\nВСЕГО НАЙДЕНО: %d \n", counter);

    return 0;
}


//helper functions
node *tohead(node * ptr)
{
    while (ptr->prev != NULL)
        ptr = ptr->prev;
    return ptr;
}

node *totail(node *ptr)
{
    while (ptr->next != NULL)
        ptr = ptr->next;
    return ptr;
}
//end of helper functions
