//=== ACLterm.h 2022/04/27 ==========================================
//
//  ACL term object defintions
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

#ifndef ACLterm_h
#define ACLterm_h

struct term {                             // define an ACL term
    unsigned short  type;                 //   data type
    union {
        long            integer;          //   int value (or num part)
        long            denom;            //   denominator
        double          floater;          //   flt value (or real part)
        double          imag;             //   imaginary part
        char            tval;             //   truth value T|F
        char            string[AGGRSIZE]; //   string
        union {
            struct lisNode* lisptr;       //   ptr to aggregate
            struct setNode* setptr;
            struct vecNode* vecptr;
            struct matNode* matptr;
        };
        char            aggr[AGGRSIZE];   //   aggregate
    } data;
};
 
struct term* NewTerm() {                // constructor for term object
    struct term* p;
    
    p = (struct term*)malloc(sizeof(struct term));
    
    p->data.integer = 0;
    p->data.denom   = 1;
    p->data.floater = 0.0;
    p->data.imag    = 0.0;
    p->data.tval    =' ';
    strcpy(p->data.string,"");
    struct lisNode* lisptr = NULL;
    struct setNode* setptr = NULL;
    struct vecNode* vecptr = NULL;
    struct matNode* matptr = NULL;
    strcpy(p->data.aggr,"");
    return p;
}

void PrintTerm(struct term* this) {    // print given term struct
    printf("\nterm:\n");
    printf("type       = %i\n", this->type);
    printf("integer    = %li\n",this->data.integer);
    printf("denom      = %li\n",this->data.denom);
    printf("floater    = %lf\n",this->data.floater);
    printf("imag       = %lf\n",this->data.imag);
    printf("tval       = %c\n", this->data.tval);
    printf("string     = ");
    unsigned long len = strlen(this->data.string);
    for (int i=0; i<len; i++) {
        printf("%c",this->data.string[i]);
    }
    //printf("lisptr     = %p\n",struct lisNode* lisptr);
    printf("\naggregate  = ");
    len = strlen(this->data.aggr);
    for (int i=0; i<len; i++) {
        printf("%c",this->data.aggr[i]);
    }
    printf("\n\n");
}
    
#endif /* ACLterm_h */
//=== end ACLterm.h ===================================================
