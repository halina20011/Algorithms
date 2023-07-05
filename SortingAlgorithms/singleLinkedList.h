// Copyright (C) 2023  halina20011
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef SINGLELINKEDLIST
#define SINGLELINKEDLIST

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

struct Node{
    int data;
    struct Node *next;
};

void printNodes(struct Node *firstNode){
    struct Node *current = firstNode;

    int i = 0;
    while((current = current->next) != NULL){
        i++;
        printf("%i, ", current->data);
    }

    if(i){
        printf("\n");
    }
}

void add(struct Node **head, struct Node **tail, int data){
    struct Node *new = malloc(sizeof(struct Node));
    new->data = data;
    new->next = NULL;

    if(*head == NULL){
        *head = new;
        *tail = new;
    }
    else{
        (*tail)->next = new;
        *tail = new;
    }
}

void insert(struct Node **head, int data){
    struct Node *new = malloc(sizeof(struct Node));
    new->next = *head;
    new->data = data;

    *head = new;
}

int delete(struct Node **head, int index){
    struct Node **indirect = head;
    int data = INT_MIN;

    int i = 0;
    while(i < index && *indirect != NULL){
        indirect = (struct Node**)&((*indirect)->next);
        i += 1;
    }
    
    if(i == index && *indirect != NULL){
        struct Node *temp = *indirect;
        data = temp->data;
        *indirect = temp->next;
        free(temp);
    }

    return data;
}

int deleteFirst(struct Node **head){
    if(*head == NULL){
        return INT_MIN;
    }

    struct Node *temp = *head;
    *head = (*head)->next;

    int tempVal = temp->data;
    free(temp);
    return tempVal;
}

#endif
