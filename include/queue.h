#ifndef QUEUE_H
#define QUEUE_H

typedef struct node_q {
    int id;
    int weight;
    struct node_q *next;
} node_q;

void create_gamequeue(node_q **front, node_q **rear);
void enqueue_game(int id, int weight, node_q **front, node_q **rear);
void dequeue_game(node_q **front, node_q **rear);
void free_queue(node_q **front, node_q **rear);

#endif