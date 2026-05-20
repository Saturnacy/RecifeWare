#include "queue.h"
#include <stdlib.h>

void create_gamequeue(node_q **front, node_q **rear) {
    *front = NULL;
    *rear = NULL;
}

void enqueue_game(int node_weight, node_q **front, node_q **rear) {
    node_q *new_node = (node_q *)malloc(sizeof(node_q));

    if (new_node != NULL) {
        new_node->id = node_weight;
        new_node->next = NULL;

        if (*front == NULL) {
            *front = new_node;
            *rear = new_node;
        } else {
            (*rear)->next = new_node;
            *rear = new_node;
        }
    }
}

void dequeue_game(node_q **front, node_q **rear) {
    if (*front != NULL) {
        node_q *temp = *front;
        *front = (*front)->next;
        free(temp);

        if (*front == NULL) *rear = NULL;
    }
}

void free_queue(node_q **front, node_q **rear) {
    while (*front != NULL) {
        dequeue_game(front, rear);
    }
}