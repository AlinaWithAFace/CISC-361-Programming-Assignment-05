//#include <stdio.h>
//#include <stdlib.h>
//#include <ucontext.h>
//
//typedef struct {
//    int thread_id; // level 0 (high priority) and 1 (low priority)
//    int thread_priority;
//    ucontext_t *thread_context;
//} node_t;
//
//typedef struct {
//    node_t *nodes;
//    int len;
//    int size;
//} threadHeap;
//
//void push(threadHeap *h, int priority, ucontext_t *thread_context) {
//    if (h->len + 1 >= h->size) {
//        h->size = h->size ? h->size * 2 : 4;
//        h->nodes = (node_t *) realloc(h->nodes, h->size * sizeof(node_t));
//    }
//    int i = h->len + 1;
//    int j = i / 2;
//    while (i > 1 && h->nodes[j].thread_priority > priority) {
//        h->nodes[i] = h->nodes[j];
//        i = j;
//        j = j / 2;
//    }
//    h->nodes[i].thread_priority = priority;
//    h->nodes[i].thread_context = thread_context;
//    h->len++;
//}
//
//ucontext_t *pop(threadHeap *h) {
//    int currentItem, j, k;
//    if (!h->len) {
//        return NULL;
//    }
//    ucontext_t *data = h->nodes[1].thread_context;
//
//    h->nodes[1] = h->nodes[h->len];
//
//    h->len--;
//
//    currentItem = 1;
//    while (currentItem != h->len + 1) {
//        k = h->len + 1;
//        j = 2 * currentItem;
//        if (j <= h->len && h->nodes[j].thread_priority < h->nodes[k].thread_priority) {
//            k = j;
//        }
//        if (j + 1 <= h->len && h->nodes[j + 1].thread_priority < h->nodes[k].thread_priority) {
//            k = j + 1;
//        }
//        h->nodes[currentItem] = h->nodes[k];
//        currentItem = k;
//    }
//    return data;
//}
////
////int main() {
////    threadHeap *h = (threadHeap *) calloc(1, sizeof(threadHeap));
////    push(h, 3, "Clear drains");
////    push(h, 4, "Feed cat");
////    push(h, 5, "Make tea");
////    push(h, 1, "Solve RC tasks");
////    push(h, 2, "Tax return");
////    int i;
////    for (i = 0; i < 5; i++) {
////        printf("%s\n", pop(h));
////    }
////    return 0;
////}
