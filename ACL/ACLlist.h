//=== ACLlist.h 2022/05/10 ==========================================
//
//  ACL list definitions
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
//===================================================================

#ifndef ACLlist_h
#define ACLlist_h

struct lisNode {                       // list node
    struct term*	data;				//	  data value
    struct listNode next;               //    next node
};

struct listNode* lists[120];             // defined lists
struct listNode* lisTop = NULL;          // top of list
int lisDepth = 0;                       // number of lists

struct term* NewList() {                 // constructor
    struct list* p;   
    p = (struct list*)malloc(sizeof(struct listNode));
	p->data = NULL;
	p->next = NULL;   
    return p;
}

void PrintList(struct listNode* this) {  // print given list struct
    printf("\nlist:\n");
    printf("data = %p\n",this->data);
	printf("next = %p\n",this->next);
    printf("\n\n");
}
    
#endif /* ACLlist_h */
//=== end ACLlist.h ===================================================
