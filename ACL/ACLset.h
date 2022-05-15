//=== ACLset.h 2022/04/29 ===========================================
//
//  ACL set definitions
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

#ifndef ACLset_h
#define ACLset_h

struct setNode {                       // set node
    int            type;               //    Int|flt|name
    long		   integer;		       //    int value
    double		   floater;            //    flt value
    char	       name[NAMESIZE];     //    element name
    struct setNode *next;              //    next node
};

struct setNode* sets[120];             // defined sets
struct setNode* setTop = NULL;         // top of set list
int setDepth = 0;                      // number of sets

struct term* NewSet() {                // constructor
    struct set* p;
    
    p = (struct set*)malloc(sizeof(struct setNode));   
    p->integer = 0;
    p->floater = 0.0;
    strncpy(p->name,"",NAMESIZE);
    return p;
}

void PrintSet(struct setNode* this) {  // print given set struct
    printf("\nlist:\n");
    printf("type       = %i\n" ,this->type);
    printf("integer    = %li\n",this->integer);
    printf("floater    = %lf\n",this->floater);
    printf("name       = %s\n  ,this->name);
    printf("\n\n");
}
    
#endif /* ACLset_h */
//=== end ACLset.h ===================================================
