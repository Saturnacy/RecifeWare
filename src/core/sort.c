#include "sort.h"
#include "queue.h"
#include <stdlib.h>
#include "raylib.h"

void assign_game_weights(node_q **front, int len) {
    if (*front != NULL) {
        node_q *current = *front;
        int count = 1;

        while (current != NULL) {
            if (count == len) {
                current->weight = 9999; 
            } else {
                current->weight = GetRandomValue(0, 100);
            }
            
            count++;
            current = current->next;
        }
    }
}

void insertion_sort_games(node_q **front, node_q **rear) {
    if (*front != NULL && (*front)->next != NULL) {
        node_q *sorted = NULL;
        node_q *current = *front;

        while (current != NULL) {
            node_q *next_node = current->next;

            if (sorted == NULL || current->weight < sorted->weight) {
                current->next = sorted;
                sorted = current;
            } else {
                node_q *search = sorted;
                
                while (search->next != NULL && search->next->weight < current->weight) {
                    search = search->next;
                }

                current->next = search->next;
                search->next = current;
            }

            current = next_node;
        }

        *front = sorted;

        node_q *temp = *front;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        *rear = temp;
    }
}