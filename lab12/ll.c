#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ll.h"

struct list {
    char *value;
    struct list *next;
    struct list *prev;
};

const char *list_node_value(list_t *node) {
    return node->value;
}

list_t *list_first(list_t *list) {
    return list;
}

list_t *list_last(list_t *list) {
    return list->prev;
}

list_t *list_next(list_t *node) {
    return node->next;
}

list_t *list_previous(list_t *node) {
    return node->prev;
}

static void failed_allocation(void) {
    fprintf(stderr, "Out of memory.\n");
    abort();
}

static list_t *new_node(const char *value) {
    list_t *node = malloc(sizeof(list_t));
    if (!node) failed_allocation();
    // First change:
    node->value = malloc(strlen(value) + 1);       // strlen returns the lenght of string without counting its null terminator
    //
    
    if (!node->value) failed_allocation();
    strcpy(node->value, value);
    return node;
}

list_t *list_insert_before(list_t *list, list_t *node, const char *value) {
    list_t *insert_node = new_node(value);
    insert_node->prev = node->prev;
    insert_node->next = node;
    insert_node->next->prev = insert_node;
    insert_node->prev->next = insert_node;
    if (list == node) {
        return insert_node;
    } else {
        return list;
    }
}

list_t *list_append(list_t *list, const char *value) {
    if (list) {
        (void) list_insert_before(list, list, value);
        return list;
    } else {
        list_t *node = new_node(value);
        node->prev = node->next = node;
        return node;
    }
}

list_t *list_prepend(list_t *list, const char *value) {
    if (list) {
        return list_insert_before(list, list, value);
    } else {
        list_t *node = new_node(value);
        node->prev = node->next = node;
        return node;
    }
}

list_t *list_remove(list_t *list, list_t *node) {
    list_t *list_next = list->next;
    node->prev->next = node->next;
    node->next->prev = node->prev;

    
    if (list != node) {
        
        //printf("Address before Free %x\n", node);
        //printf("Contenct of node before Free: %x\n", *node);
        
        //2nd Change
        free(node->value);              // We also need to free the allocated memory for vlue portion of the node
        free(node);
        
        //printf("Address %x\n", node);
        //printf("Contenct of node: %x\n", *node);
        
        return list;
        
        //3rd change:
    } else {
        //return list->next == list ? NULL : list->next;
        int null_flag = 0;
        if(list == list_next)
            null_flag = 1;
        free(node->value);
        free(node);
        if(null_flag)
            return NULL;
        else
            return list_next;
    }
}

void list_free(list_t *list) {
    if (list) {
        while (list_remove(list, list_last(list)) != NULL) {}
    }
}

void list_foreach(list_t *list, void (*function)(const char*)) {
    if (list) {
        list_t *cur = list_first(list);
        do {
            function(cur->value);
            cur = cur->next;
        } while (cur != list_first(list));
    }
}
