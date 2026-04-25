//queue.c for non-preemptive processes
//First Come First Serve (FCFS) scheduling algorithm implementation

#include <stdio.h>
#include <stdlib.h>
#include "process.h"

// create_queue:
//     allocate Queue struct
//     head = tail = NULL, size = 0
//     return queue

printf("Creating queue...\n");

Queue *create_queue() {
    Queue *q = (Queue *)malloc(sizeof(Queue));
    if (q == NULL) { 
        fprintf(stderr, "Error: Could not allocate memory for queue\n");
        return NULL;
    }
    q->head = q->tail = NULL;
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

printf("Enqueuing process...\n");

void enqueue(Queue *q, Process *process) {
    if (q == NULL || process == NULL) {
        fprintf(stderr, "Error: Queue or process is NULL\n");
        return;
    }
    Node *node = (Node *)malloc(sizeof(Node)); //allocating new node
    if (node == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for node\n");
        return;
    }
    node->process = process;
    node->next = NULL;
    if (q->tail) {
        q->tail->next = node;
    } else {
        q->head = node;  //empty queue, node is also head
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

printf("Dequeuing process...\n");

Process *dequeue(Queue *q) {
    if (q == NULL || q->head == NULL) {
        return NULL; //empty queue
    }
    Node *node = q->head; //save head node
    q->head = node->next; //move head to next
    Process *process = node->process; //get process to return
    if (q->head == NULL) {
        q->tail = NULL; //queue now empty
    }
    free(node);
    q->size--;
    return process;
}

// peek(queue):
//     return head.proc (or NULL if empty)   ← no removal

printf("Peeking at front of queue...\n");

Process *peek(Queue *q) {
    if (q == NULL || q->head == NULL) {
        return NULL; //empty queue
    }
    return q->head->process; //return process at head without removing
}

// free_queue(queue):
//     while head exists → dequeue()         ← drain all nodes
//     free queue struct

printf("Freeing queue...\n");

Process *free_queue(Queue *q) {
    if (q == NULL) {
        return NULL;
    }
    while (q->head) {
        dequeue(q); //drain all nodes
    }
    free(q); //free queue struct
    return NULL;
}