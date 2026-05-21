// queue.c

#include <stdio.h>
#include <stdlib.h>
#include "process.h"

// create_queue:
//     allocate Queue struct
//     head = tail = NULL, size = 0
//     return queue

Queue *create_queue() {
    Queue *q = (Queue *)malloc(sizeof(Queue));

    if (q == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for queue\n");
        return NULL;
    }

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;

    return q;
}

// enqueue(queue, process):
//     allocate new Node
//     node.proc = process
//     node.next = NULL
//     if queue has tail:
//         tail.next = node
//     else:
//         head = node        ← empty queue, node is also head
//     tail = node
//     size++

void enqueue(Queue *q, Process *process) {
    Node *node;

    if (q == NULL || process == NULL) {
        fprintf(stderr, "Error: Queue or process is NULL\n");
        return;
    }

    node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for node\n");
        return;
    }

    node->proc = process;
    node->next = NULL;

    if (q->tail != NULL) {
        q->tail->next = node;
    } else {
        q->head = node;
    }

    q->tail = node;
    q->size++;
}

// dequeue(queue):
//     if head is NULL → return NULL
//     save head node
//     head = head.next
//     if head is NULL → tail = NULL   ← queue now empty
//     free old node
//     size--
//     return saved process

Process *dequeue(Queue *q) {
    Node *node;
    Process *process;

    if (q == NULL || q->head == NULL) {
        return NULL;
    }

    node = q->head;
    q->head = node->next;
    process = node->proc;

    if (q->head == NULL) {
        q->tail = NULL;
    }

    free(node);
    q->size--;

    return process;
}

Process *peek(Queue *q) {
    if (q == NULL || q->head == NULL) {
        return NULL;
    }

    return q->head->proc;
}

void free_queue(Queue *q) {
    while (q != NULL && q->head != NULL) {
        dequeue(q);
    }

    free(q);
}
