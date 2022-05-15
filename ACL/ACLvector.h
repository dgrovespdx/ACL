//=== ACLvector.h 2022/05/07 =========================================
//
//  ACL vector definitions
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
//=====================================================================

#ifndef ACLvector_h
#define ACLvector_h

struct vecNode {                        // vector node
    unsigned short type;                //    Int or flt
    unsigned short dim;                 //    dimension
    long		   integer[VECSIZE];    //    int values
    double		   floater[VECSIZE];    //    flt values
    struct symNode *next;               //    next node
};

struct vecNode* vectors[120];           // defined vectors
struct vecNode* vecTop = NULL;          // top of vector list
int vecDepth = 0;                       // number of vectors

struct vecNode* NewVector() {           // constructor
    struct vecNode* p;
    
    p = (struct vecNode*)malloc(sizeof(struct vecNode));
    for (int i=0; i<4; i++) {
        p->integer[i] = 0;
        p->floater[i] = 0.0;
    }
    return p;
}

void PutVector(struct vecNode* this) {   // this to vectors
    vectors[vecDepth++] = this;
    return;
}

void PrintVector(struct vecNode* this) {  // print given term struct
    unsigned short n;
    unsigned short type;
    
    type = this->type;
    n    = this->dim;
    printf("<");
    for (int i=0; i<n; i++) {
        if (i > 0) {printf(" ");}
        printf("%.2lf",this->floater[i]);
    }
    printf(">");
}
    
#endif /* ACLvector_h */
//=== end ACLvector.h ================================================
