/*
 * sorted-list.c
 * Rayad Ali
 *
 */

#include <stdlib.h>
#include "sorted-list.h"

/*
 we will be using a single-linked list to store the data*/

struct Node{
    void * data; //data
    struct Node* next; // next node in linked list
};
typedef struct Node* NodePtr;

NodePtr newNode(void * data, NodePtr nextNode){
    NodePtr newNode = (NodePtr)malloc(sizeof(nextNode));
    if(newNode == NULL) return NULL;
    newNode->data = data;
    newNode->next = nextNode;
    return newNode;
}

void deleteNode(NodePtr node){
    free(node);
}

/*
 * Sorted list type.  You need to fill in the type as part of your implementation.
 */
struct SortedList // essentially our linked list
{
    NodePtr head; // start
    CompareFuncT comp; //using the compare function
    DestructFuncT destroy; // destroy objects
};
typedef struct SortedList* SortedListPtr;


/*
 * Iterator type for user to "walk" through the list item by item, from
 * beginning to end.  You need to fill in the type as part of your implementation.
 */
struct SortedListIterator // we need to keep track of our nodes
{
    NodePtr current;
    NodePtr previous;
};
typedef struct SortedListIterator* SortedListIteratorPtr;


/*
 * When your sorted list is used to store objects of some type, since the
 * type is opaque to you, you will need a comparator function to order
 * the objects in your sorted list.
 *
 * You can expect a comparator function to return -1 if the 1st object is
 * smaller, 0 if the two objects are equal, and 1 if the 2nd object is
 * smaller.
 *
 * Note that you are not expected to implement any comparator or destruct
 * functions.  You will be given a comparator function and a destruct
 * function when a new sorted list is created.
 */

typedef int (*CompareFuncT)( void *, void * );
typedef void (*DestructFuncT)( void * );

/*
 * SLCreate creates a new, empty sorted list.  The caller must provide
 * a comparator function that can be used to order objects that will be
 * kept in the list, and a destruct function that gets rid of the objects
 * once they are no longer in the list or referred to in an iterator.
 * 
 * If the function succeeds, it returns a (non-NULL) SortedListT object,
 * otherwise, it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df){
    	SortedListPtr slptr= (SortedListPtr)malloc(sizeof(struct SortedList)); // initialise new ptr, size of the sorted list
	if (cf == NULL)
		return NULL;
        slptr->head = NULL;
	slptr->comp = cf;
        slptr->destroy = df;
	return slptr;
};

/*
 * SLDestroy destroys a list, freeing all dynamically allocated memory.
 *
 * You need to fill in this function as part of your implementation.
 */
void SLDestroy(SortedListPtr list){
    NodePtr temp; //create temp LL to then free everything
    temp = list->head; // storing list into node
    if(temp == NULL) return; // if empty, already destroyed
    while(temp->next != NULL)
	{
            temp = temp->next; 
            free(list->head->data); // we have to specifically free the data and not JUST the node itself		
       	    free(list->head);
            list->head = temp; // set up for next iteration
	}
    // these 3 are for the last node in the LL
    free(list->head->data);
    free(list->head);
    free(list);
};

/*
 * SLCreateIterator creates an iterator object that will allow the caller
 * to "walk" through the list from beginning to the end using SLNextItem.
 *
 * If the function succeeds, it returns a non-NULL pointer to a
 * SortedListIterT object, otherwise it returns NULL.  The SortedListT
 * object should point to the first item in the sorted list, if the sorted
 * list is not empty.  If the sorted list object is empty, then the iterator
 * should have a null pointer.
 *
 * You need to fill in this function as part of your implementation.
 */

SortedListIteratorPtr SLCreateIterator(SortedListPtr list){
    SortedListIteratorPtr sliptr = (SortedListIteratorPtr)malloc(sizeof(struct SortedListIterator));		
    if (sliptr != NULL){	//should be ok but incase..
	sliptr->current = list->head;	//setting the first item in the iterator to be at the head of the LL
	if (sliptr->current!=NULL){
            return sliptr;	
	}					
	}
    else{
            return NULL;							
	}

}

/*
 * SLNextItem returns the next object in the list encapsulated by the
 * given iterator.  It should return a NULL when the end of the list
 * has been reached.
 *
 * One complication you MUST consider/address is what happens if a
 * sorted list encapsulated within an iterator is modified while that
 * iterator is active.  For example, what if an iterator is "pointing"
 * to some object in the list as the next one to be returned but that
 * object is removed from the list using SLRemove() before SLNextItem()
 * is called.
 *
 * You need to fill in this function as part of your implementation.
 */

void *SLNextItem(SortedListIteratorPtr sliptr){
    NodePtr temp = sliptr->current->next; // ptr to next node
    if(temp != NULL){
        return temp->data;
    }
    else return NULL;
    
    // need to consider if removed before this is called, currently this does not consider that
}

/*
 * SLGetItem returns the pointer to the data associated with the
 * SortedListIteratorPtr.  It should return 0 if the iterator
 * advances past the end of the sorted list.
 * 
 * You need to fill in this function as part of your implementation.
*/

void * SLGetItem( SortedListIteratorPtr iter ){
    if(iter->current != NULL){
        return iter->current->data;
    }
}
/*
 * SLInsert inserts a given object into a sorted list, maintaining sorted
 * order of all objects in the list.  If the new object is equal to a subset
 * of existing objects in the list, then the subset can be kept in any
 * order.
 *
 * If the function succeeds, it returns 1, othrewise it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

int SLInsert(SortedListPtr list, void *newObj){
    SortedListIteratorPtr sliptr = SLCreateIterator(list);
    if (sliptr == NULL) return 0;
    int i;
    void * currentData;
    NodePtr previous = sliptr->current;
    
    if((sliptr->current == NULL) || (*list->comp)(newObj,sliptr->current->data) == 1){
        list->head = newNode(newObj,sliptr->current);// emptyu list just created LL and insert into head
        SLDestroyIterator(sliptr);
        return 1;
    }
    currentData = SLNextItem(sliptr);
    
    while(sliptr->current  != NULL){
        i = (*list->comp)(newObj,currentData); //check which is bigger
        if(i>1) break; // keep searching
        else if(i == 0){
            SLDestroyIterator(sliptr);
            return 1;
        }
        else{
            previous = sliptr->current; //insert
            currentData = SLNextItem(sliptr);
        }
    }
    previous->next = newNode(newObj,sliptr->current); //creating that new node for the insertion
    if(previous->next == NULL) return 0;
    SLDestroyIterator(sliptr);
    return 1;
    
};


/*
 * SLRemove removes a given object from a sorted list.  Sorted ordering
 * should be maintained.  SLRemove may not change the object whose
 * pointer is passed as the second argument.  This allows you to pass
 * a pointer to a temp object equal to the object in the sorted list you
 * want to remove.
 *
 * If the function succeeds, it returns 1, otherwise it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

int SLRemove(SortedListPtr list, void *newObj){
    SortedListIteratorPtr dataToRemove = SLCreateIterator(list);
    if(dataToRemove == NULL) return 0;
    
    int i;
    void * currentData;
    NodePtr previous = dataToRemove->current;
    i=(*list->comp)(newObj,list->head->data); // getting the compare result
    
    //found the node in head
    if(i == 0){
        previous->next = dataToRemove->current->next; // literally makes the prev next node the node after next
        SLDestroyIterator(dataToRemove);
        return 1;
    }
    
    currentData = SLNextItem(dataToRemove);
    //iterate through list
    while(dataToRemove->current != NULL){
        i = (*list->comp)(newObj,currentData);
        //if found
        if(i == 0){
            previous->next = dataToRemove->current->next;
            SLDestroyIterator(dataToRemove);
            return 1;
        }
        else{
        previous = dataToRemove->current; // might not need this line
        currentData = SLNextItem(dataToRemove);
        }
    }
    SLDestroyIterator(dataToRemove); // clearing now that we're done and couldn't find it
    return 0;
}



/*
 * SLDestroyIterator destroys an iterator object that was created using
 * SLCreateIterator().  Note that this function should destroy the
 * iterator but should NOT affect the original list used to create
 * the iterator in any way.
 *
 * You need to fill in this function as part of your implementation.
 */

void SLDestroyIterator(SortedListIteratorPtr iter){
    if(iter->current != NULL){
        deleteNode(iter->current);
    }
    free(iter);
}




