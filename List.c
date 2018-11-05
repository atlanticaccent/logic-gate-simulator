#include "header.h"

/**
 * Adds a node holding the given data to a given list
 * @param list the list to add the node to
 * @param data the data to be held by the new node
 */
void addNode(ListHeader* list, Gate* data) {
    ListNode* insertion = malloc(sizeof(ListNode));
    insertion -> data = data;

    if (list -> length > 0) {                                                   //Check if the list has any entries yet
        insertion -> next = 0;
        list -> tail -> next = insertion;                                       //Point old tail.next to new tail
        list -> tail = insertion;                                               //Reassign tail to point to new tail
    } else {                                                                    //If the list has been unused up to this point, set both head and tail equal to the new node
        list -> head = insertion;
        list -> tail = insertion;
    }

    list -> length++;
}

/**
 * Frees the memory allocated to a list
 * @param list     the List whose memory is to be freed
 * @param freeData whether the data in a list is to be freed also - necessary distinction due to other lists potentially also having pointed to the same data, and already freed it before this list
 */
void destroyList(ListHeader* list, bool freeData) {
    ListNode* temp;                                                             //Keeps track of the next node to be freed, as current is lost at the end of this block
    for (ListNode* current = list -> head; current; current = temp) {
        temp = current -> next;
        if (freeData) destroyGate(current -> data);                             //Destroy the struct held as data in the node if directed to
        free(current);                                                          //No internal variables are malloced to the node when it is created, so the node can almost immediately be deconstructed
    }
    free(list);                                                                 //Once all nodes have been freed, free the list header
}
