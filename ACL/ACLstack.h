// === ACLstack.h 2022/05/07 =======================================
//
//  ACL stack definitions
//
//  Implements a permutation stack as a linked list of terms.
//  Stack terms are added by push(), removed by pop(), and may
//  be reordered by other stack operations.
/*
 * Copyright (c) 2022 Don Groves
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
// =================================================================

#ifndef ACLstack_h
#define ACLstack_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define X Stop                          // X refers to top stack item
#define Y Stop->next                    // Y refers to second item
#define Z Stop->next->next              // Z refers to third item
#define W Stop->next->next->next        // W refers to fourth item
#define Xtype X->term->type             // types of each stack item
#define Ytype Y->term->type
#define Ztype Z->term->type
#define Wtype W->term->type

struct Snode {                          // define stack node object
    struct term*  term;                 //    pointer to term
    struct Snode* next;                 //    pointer to next node
    struct Snode* prev;                 //    pointer to previous node;
};
struct Snode* Stop = NULL;              // pointer to top stack node
struct Snode* newNode;
int Sdep = 0;                           // current stack depth

struct Snode* NewSnode() {              // Snode constructor
    struct Snode* p;
    p = (struct Snode*)malloc(sizeof(struct Snode));
    p->term = NULL;
    p->next = NULL;
    p->prev = NULL;
    return p;
}

void InitStack() {
    Sdep = 0;
}

void Free(struct Snode* this) {         // free an Snode ptr
    if (this->term->type == VECTYP)
        free(this->term->data.vecptr);
    free(this);
}                                       // stack methods
void push(struct term* this) {          // insert node at top of stack
    newNode = NewSnode();
    if (newNode == NULL) {              // check for malloc failure
        error("push","out of memory","");
        exit(-1);
    }
    if (Stop!=NULL) {Stop->prev = newNode;} // point current top back to new top
    if (Sdep > 0) {newNode->next = Stop;}   // populate new node
    else {newNode->next = NULL;}
    newNode->term = this;
    newNode->prev = NULL;
    Stop = newNode;                     // and stack it
    Sdep++;
}
struct Snode* pop() {                   // remove and return top node
    struct Snode* node;

    if (Stop == NULL) {
        error("pop","stack underflow","");
        exit(FAIL);
    }
    node = Stop;
    Stop = Stop->next;
    Sdep--;
    return node;
}
void swap() {                           // S[Z Y X] => S[Z X Y]
    struct Snode* x;
    struct Snode* y;
    
    if (Sdep >= 2) {
        x=pop(); y=pop();
        push(x->term);
        push(y->term);
        free(x); free(y);
    }
    else {error("swap", "stack underflow","");}
}
void swapYZ() {                           // S[Z Y X] => S[Y Z X]
    struct Snode* x;
    struct Snode* y;
    struct Snode* z;
    
    if (Sdep >= 3) {
        x=pop(); y=pop(); z=pop();
        push(y->term);
        push(z->term);
        push(x->term);
        free(x); free(y); free(z);
    }
    else {error("swap", "stack underflow","");}
}
void dup_() {                           // S[Z Y X] = S[Z Y X X]
    if (Sdep>0) {push(X->term);}
    else {error("dup", "stack underflow","");}
}
void drop() {                           //   S[Z Y X] => S[Z Y]
    struct Snode* temp;
    if (Sdep>0) {
        temp = Stop;
        Stop = Stop->next;
        Free(temp);
        Sdep--;
    }
    else {error("drop", "stack underflow","");}
}
void under() {                          // S[Z Y X Q] => S[Z Y Q X]
    struct Snode* x;
    struct Snode* y;
    if (Sdep >= 2) {
        x = pop(); y = pop();
        push(x->term);
        push(y->term);
        Free(x); Free(y);
    }
    else {error("under","stack underflow","");}
}
void rotup() {                            // S[Z Y X]=> S[Y X Z]
    struct Snode* x;
    struct Snode* y;
    struct Snode* z;
    if (Sdep>=3) {
        x=pop(); y=pop(); z=pop();
        push(y->term);
        push(x->term);
        push(z->term);
        Free(x); Free(y); Free(z);
    }
    else {error("rot","stack underflow", "");}
}
void rotdn() {                            // S[Z Y X]=> S[X Z Y]
    struct Snode* x;
    struct Snode* y;
    struct Snode* z;
    if (Sdep>=3) {
        x=pop(); y=pop(); z=pop();
        push(x->term);
        push(z->term);
        push(y->term);
        Free(x); Free(y); Free(z);
    }
    else {error("rot","stack underflow", "");}
}
void empty() {                          // clear the stack
    while (Sdep > 0) {drop();}
}
void PrintStack() {                     // print current stack
    //if (ACLstate != INTERACTING) {return;}
    printf("S[");
    if (Stop == NULL) {
        printf("]: ");
        return;
    }
    else  {
        struct Snode* tail = Stop;
        
        while (tail->next != NULL) {    // Traverse till tail of the linked list
            tail = tail->next;
        }
        while (tail != Stop) {                          // print all but top value
            if (tail->term->type==INTTYP) {
                printf("%ld ",tail->term->data.integer);
            }
            else if (tail->term->type==FLTTYP) {
                printf("%.2lf ",tail->term->data.floater);
            }
            else if (tail->term->type==TVALTYP) {
                printf("%c ",tail->term->data.tval);
            }
            else if (tail->term->type==CONTYP) {
                printf(".2%f ",tail->term->data.floater);
            }
            else if (tail->term->type==VECTYP) {
                PrintVector(tail->term->data.vecptr);
                printf(" ");
            }
            else {
                printf("%s ",tail->term->data.aggr);
            }
            tail = tail->prev;
        }
                                                // print top value
        if (Stop->term->type==INTTYP) {
            printf("%ld",Stop->term->data.integer);
        }
        else if(tail->term->type==FLTTYP) {
            printf("%.2lf",Stop->term->data.floater);
        }
        else if (tail->term->type==TVALTYP) {
            printf("%c",tail->term->data.tval);
        }
        else if (tail->term->type==CONTYP) {
            printf(" 2%f",tail->term->data.floater);
        }
        else if (tail->term->type==VECTYP) {
            PrintVector(tail->term->data.vecptr);
        }
        else {
            printf("%s",Stop->term->data.aggr);
        }
    }
    printf("]: ");
//    printterm(tail->term);
};
#endif //ACLstack_h  
// === end ACLstack.h ===========================================
