//=== ACLprim.h 2022/04/27 ========================================
//
//  ACL primitive operations.
//
//  Implements ACL's built-in functions.
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
//=================================================================
#ifndef ACLprim_h
#define ACLprim_h

int type;
long x, y, z;
double xf, yf;
long intval;
double fltval;
struct term* termptr;

bool isNumber(int this) {
    if (this==INTTYP || this==FLTTYP ||
        this==CPXTYP || type==RATTYP)
         return true;
    else return false;
}

bool isString(int this) {
    if (this==STRTYP) {return true;}
    else              {return false;}
}

bool isTval(int this) {
    if (this==TVALTYP) {return true;}
    else               {return false;}
}

bool isAggregate(int this) {
    if (this==AGGTYP) {return true;}
    else              {return false;}
}

char predop(char* op) {                     // predicates
                                            // return Y op X
    if (Sdep < 2) {
        error("compile","stack underflow",op);
        return ' ';        
    }
    if (Xtype != Ytype) {
        error("compile","X and Y not same type",op);
        return (' ');
    }
    
    if(isNumber(Xtype)) {
        switch (Xtype){
        case INTTYP:
            if (!strcmp(op,"=?")) {
                if (X->term->data.integer == Y->term->data.integer)
                     {return ('T');}
                else {return ('F');}
                break;
            }
            if (!strcmp(op,"!=?")) {
                if (X->term->data.integer != Y->term->data.integer)
                     {return ('T');}
                else {return ('F');}
                break;
            }
            if (!strcmp(op,"<?")) {
                if (X->term->data.integer < Y->term->data.integer)
                     {return ('T');}
                else {return ('F');}
                break;
            }
            if (!strcmp(op,"<=?")) {
                if (X->term->data.integer <= Y->term->data.integer)
                     {return ('T');}
                else {return ('F');}
                break;
            }
            if (!strcmp(op,">?")) {
                if (X->term->data.integer > Y->term->data.integer)
                     {return ('T');}
                else {return ('F');}
                break;
            }
            if (!strcmp(op,">=?")) {
                if (X->term->data.integer >= Y->term->data.integer)
                     {return ('T');}
                else {return ('F');}
                break;
            }
        case FLTTYP:
            if (!strcmp(op,"=?")) {
                if (X->term->data.floater == Y->term->data.floater)
                     return ('T');
                else return ('F');
                break;
            }
            if (!strcmp(op,"!=?")) {
                if (X->term->data.floater != Y->term->data.floater)
                     {return ('T');}
                else {return ('F');}
                break;
                }
            }
        }
    if (isString(Xtype)) {
        if (!strcmp(X->term->data.string,Y->term->data.string))
             return ('T');
        else return ('F');
    }
    if (isTval(Xtype)) {
        if (X->term->data.tval == Y->term->data.tval)
             return ('T');
        else return ('F');
    }
    if (isAggregate(Xtype)) {
        if (!strcmp(X->term->data.aggr,Y->term->data.aggr))
             return ('T');
        else return ('F');
    }
    error("compile","incompatible types",op);
    return ' ';
}
                                        // return Y<op>X for integers
long intop(char* op, long x, long y) {
    if (!strcmp(op,"+")) {return y+x;}
    if (!strcmp(op,"-")) {return y-x;}
    if (!strcmp(op,"*")) {return y*x;}
    if (!strcmp(op,"/")) {return y/x;}
    if (!strcmp(op,"^2")) {return x*x;}
    if (!strcmp(op,"^3")) {return x*x*x;}
    if (!strcmp(op,"neg")) {return -x;}
    if (!strcmp(op,"sqrt")) {return sqrt((double)x);}
    return 0;
}
                                        // return Y<op>X for floats
double fltop(char* op,double x,double y) {
    if (!strcmp(op,"+")) {return y+x;}
    if (!strcmp(op,"-")) {return y-x;}
    if (!strcmp(op,"*")) {return y*x;}
    if (!strcmp(op,"/")) {return y/x;}
    if (!strcmp(op,"^2")) {return x*x;}
    if (!strcmp(op,"^3")) {return x*x*x;}
    if (!strcmp(op,"neg")) {return -x;}
    if (!strcmp(op,"sqrt")) {return sqrt(x);}
    return 0;
}
                                        // binary operations
                                        // return function value & type
int binaryop(char* op,long *intret,double *fltret,int *typret) {
    struct Snode* x;
    struct Snode* y;
    long xintval;  long yintval;
    double xfltval; double yfltval;
    int xtype; int ytype;
    
    if (Sdep < 2) {return UNDERFLOW;}       // ensure 2+ stack entries
    if (Xtype!=INTTYP && Xtype!=FLTTYP) {return MISMATCH;}
    
    x = pop(); y = pop();                   // fetch X and Y
    xtype = x->term->type;                  // get types
    ytype = y->term->type;
    
    if (xtype==INTTYP && ytype==INTTYP) {       // both ints
        xintval = x->term->data.integer;
        yintval = y->term->data.integer;
        *intret = intop(op,xintval,yintval);
        *fltret = 0.0;
        *typret = INTTYP;
    }
    else if (xtype==FLTTYP && ytype==FLTTYP) {  // both floats
        xfltval = x->term->data.floater;
        yfltval = y->term->data.floater;
        *fltret = fltop(op,xfltval,yfltval);
        *intret = 0;
        *typret = FLTTYP;
    }
    else if (xtype==INTTYP && ytype==FLTTYP) {  // mixed types use flt
        xfltval = x->term->data.integer;
        yfltval = y->term->data.floater;
        *fltret = fltop(op,xfltval,yfltval);
        *intret = 0;
        *typret = FLTTYP;
    }
    else if (xtype==FLTTYP && ytype==INTTYP) {  // mixed types use flt
        xfltval = x->term->data.floater;
        yfltval = y->term->data.integer;
        *fltret = fltop(op,xfltval,yfltval);
        *intret = 0;
        *typret = FLTTYP;
    }
    Free(x); Free(y);
    return OK;
}
                                        // return function value & type
int unaryop(char* op,long *intret,double *fltret,int *type) {
    struct Snode* x;
    long xintval;
    double xfltval;

    if (Sdep < 1) {return FAIL;}        // ensure 1+ stack entries
    x = pop();                          // fetch X
    switch (x->term->type) {
    case INTTYP:
        xintval = x->term->data.integer;
        *intret = intop(op,xintval,0);
        *fltret = 0.0;
        *type = INTTYP;
        free(x);
        return OK;
    case FLTTYP:
        xfltval = x->term->data.floater;
        *fltret = fltop(op,xfltval,0);
        *type = FLTTYP;
        free(x);
        return OK;
        break;
    default:
        free(x);
        return FAIL;;
    }
    return OK;
}

int vecop(char* op) {                           // perform vector operations
    int xdim; int ydim;
    double xfltval;
    double yfltval;
    double ans;
    double sum = 0;
    struct term* t;
    struct vecNode* v;
    struct Snode* x;
    struct Snode* y;
    double ax; double ay; double az;
    double bx; double by; double bz;
    
    if (Sdep < 2) {
        error("execute","stack underflow",op);
        return UNDERFLOW;
    }
    if (Xtype!=VECTYP || Ytype!=VECTYP) {
        error("execute","type mismatch",op);
        return MISMATCH;
    }
    xdim = X->term->data.vecptr->dim;
    ydim = Y->term->data.vecptr->dim;
    if (xdim != ydim) {
        error("execute","vectors not same dimension",op);
        return MISMATCH;
    }
    x = pop(); y = pop();
    t = NewTerm();
    if ((!strcmp(op,"+")) || (!strcmp(op,"-") ||
       (!strcmp(op,"*")))) {
        t->type = VECTYP;
        v = NewVector();
        v->type = FLTTYP;
        v->dim = xdim;
        for (int i=0; i<xdim; i++) {
            xfltval = x->term->data.vecptr->floater[i];
            yfltval = y->term->data.vecptr->floater[i];
            if (!strcmp(op,"+") || (!strcmp(op,"-"))) {
                ans = fltop(op,xfltval,yfltval);            // add or subtract
                v->floater[i] = ans;
            }
            else if (!strcmp(op,"*")) {                     // cross product
                if (xdim != 3) {
                    error("execute","must be dim 3",op);
                }
                v->dim = 3;
                ax = x->term->data.vecptr->floater[0];
                ay = x->term->data.vecptr->floater[1];
                az = x->term->data.vecptr->floater[2];
                bx = y->term->data.vecptr->floater[0];
                by = y->term->data.vecptr->floater[1];
                bz = y->term->data.vecptr->floater[2];
                v->floater[0] = ay*bz - az*by;
                v->floater[1] = az*bx - ax*bz;
                v->floater[2] = ax*by - ay*bx;
            }
        }
        //PrintVector(v); printf("\n");
        t->data.vecptr = v;
    }
    else if (!strcmp(op,".")) {
        for (int i=0; i<xdim; i++) {                        // dot product
            xfltval = x->term->data.vecptr->floater[i];
            yfltval = y->term->data.vecptr->floater[i];
            ans = fltop("*",xfltval,yfltval);
            sum += ans;
            //printf("\nvecop: ans=%f sum=%f\n",ans,sum);
        }
        t->data.floater = sum;
        t->type = FLTTYP;
    }
    push(t);
    Free(x); Free(y);
    return OK;
}

int trigop(char* op) {                       // trigonometric operations
    double value = 0;
    struct Snode* x;
    struct term* t;
    
    if (Sdep < 1) {
        error("execute","stack underflow",op);
        return UNDERFLOW;
    }
    if (Xtype!=INTTYP && Xtype!=FLTTYP) {
        error("execute","type mismatch",op);
        return MISMATCH;
    }
    x = pop();
    t = NewTerm();
    if (x->term->type == INTTYP) {
        value = x->term->data.integer;
    }
    else if (x->term->type == FLTTYP) {
        value = x->term->data.floater;
    }
    if (!strcmp(op,"sin")) {
        t->data.floater = sin(value);
    }
    else if (!strcmp(op,"cos")) {
        t->data.floater = cos(value);
    }
    else if (!strcmp(op,"tan")) {
        t->data.floater = tan(value);
    };
    t->type = FLTTYP;
    push(t);
    Free(x);
    return OK;
}

void B() {                      // B combinator BZYX = X(YZ)
                                // if X is a \, execute it
                                // else do nothing
    struct Snode* x;
    char lambda[AGGRSIZE];
    unsigned long len;
    
    if (X->term->type == LAMTYP) {
        x = pop();
        strncpy(lambda,x->term->data.aggr,AGGRSIZE);
        len = strlen(lambda);
        lambda[len-1] = '\n';    // remove lead & trail []
        lambda[0] = ' ';
        compile(lambda);
        Free(x);
    }
    else {
        error("B","X not a lambda","");
    }
}

int if_() {                    // if X is T, execute
    struct Snode* x;
    
    if (Sdep < 2) {
        error("execute","stack underflow","if");
        return UNDERFLOW;
    }
    
    if (Xtype == TVALTYP) {
        x = pop();
        if (x->term->data.tval == 'T') {
            B();
        }
        else {
            drop();
        }
        Free(x);
        return OK;
    }
    else {
        error("if","type error","X not T|F");
        return MISMATCH;
    }
}

int ifelse() {               // if X is T, execute Y, else Z
    struct Snode* x;
     
    if (Sdep < 3) {
        error("execute","stack underflow","ifelse");
        return UNDERFLOW;
    }
    if (Ytype!=LAMTYP || Ztype!=LAMTYP) {
        error("execute","type mismatch","ifelse");
        return MISMATCH;
    }
    
    if (Xtype == TVALTYP) {
        x = pop();
        if (x->term->data.tval == 'T') {
            swap(); drop();
            B();
        }
        else {
            drop();
            B();
        }
        Free(x);
        return OK;
    }
    else {
        error("ifelse","type error","X not T|F");
        return MISMATCH;
    }
}

bool or() {
    if (Sdep < 2) {
        error("execute","stack underflow","or");
        return UNDERFLOW;
    }
    if ((X->term->type != TVALTYP) ||
        Y->term->type != TVALTYP) {
        error("or","types not boolean","or");
        return MISMATCH;
    }
    if (X->term->data.tval == 'F' &&
        Y->term->data.tval == 'F')
         Y->term->data.tval = 'F';
    else Y->term->data.tval = 'T';
        drop();
        return OK;
}

bool and() {
    struct term* t;
    if (Sdep < 2) {
        error("execute","stack underflow","and");
        return UNDERFLOW;
    }
    if ((X->term->type != TVALTYP) ||
        Y->term->type != TVALTYP) {
        error("or","X not boolean","and");
        return MISMATCH;
    }
    if (X->term->data.tval == 'T' &&
        Y->term->data.tval == 'T')
         Y->term->data.tval = 'T';
    else Y->term->data.tval = 'F';
    drop();
    return OK;;
}

bool not() {
    if (Sdep < 1) {
        error("execute","stack underflow","not");
        return UNDERFLOW;
    }
    if (X->term->type != TVALTYP) {
        error("or","type not boolean","not");
        return MISMATCH;
    }
    if (X->term->data.tval == 'T')
         X->term->data.tval = 'F';
    else X->term->data.tval = 'T';
    return OK;
}

int decr() {
    if (X->term->type == INTTYP) {
        X->term->data.integer = X->term->data.integer-1;
        return OK;
    }
    else {
        error("decr","type error","X not integer");
        return FAIL;
    }
}

int fact() {
    long fac = 1;
    struct Snode* x;
    struct term* t;
    
    if (Sdep < 1) {
        error("execute","stack underflow","!");
        return UNDERFLOW;
    }
    if (Xtype != INTTYP) {
        error("\nexecute","X not integer","!");
        return MISMATCH;
    }
   
    long n = X->term->data.integer;
    
    if (n > 20) {
        error("execute","n > 20","!");
        return FAIL;;
    }
    x = pop();
    t = NewTerm();
    
    for (int i=1; i<=n; i++) {
        fac = fac * i;
    }
    t->data.integer = fac;
    t->type = INTTYP;
    push(t);
    Free(x);
    return OK;
}

int incr() {
    if (X->term->type == INTTYP) {
        X->term->data.integer = X->term->data.integer+1;
        return OK;
    }
    else {
        error("decr","type error","X not integer");
        return FAIL;
    }
}

int for_() {                        // [Z λ n] for => [Z']
    
    if (Sdep < 3) {
        error("execute","stack underflow","for");
        return UNDERFLOW;
    }

    if (X->term->type==INTTYP && Y->term->type==LAMTYP) {
        while (X->term->data.integer > 0) {
            rotdn();                // [n Z λ]
            dup_();                 // [n Z λ λ]
            swapYZ();               // [n λ Z λ]
            B();                    // [n λ Z']
            swap();                 // [n Z' λ]
            rotup();                // [Z' λ n]
            decr();                 // [Z' λ n-1]
        }
        drop(); drop();
        return OK;
    }
    else {
        error("for","type error","X not n or Y not λ");
        return MISMATCH;
    }
}

int while_() {                      // [Z ⋋ tval] while => [⋋(Z)]
    
    if (Sdep < 3) {
        error("execute","stack undeflow","while");
        return UNDERFLOW;
    }
    if (Xtype != TVALTYP) {
        error("execute","type mismatch","while");
        return MISMATCH;
    }
    while(X->term->data.tval == 'T') {
        pop();                      // [Z ⋋]
        dup_();                     // [Z ⋋ ⋋]
        rotup();                    // [⋋ ⋋ Z]
        swap();                     // [⋋ Z ⋋]
        B();                        // [⋋ Z' tval] (⋋ must push T|F)
        swapYZ();                   // [Z' ⋋ tval]
    }
    drop();                         // [Z' ⋋]
    drop();                         // [Z']
    return OK;
}

int d2r() {                          // convert degrees to radians
    struct Snode* x;
    struct term* t;
    double angle;
    
    if (Sdep < 1) {
        error("execute","stack underflow","d2r");
        return UNDERFLOW;
    }
    if (Xtype!=INTTYP && Xtype!=FLTTYP) {
        error("execute","type mismatch","d2r");
        return MISMATCH;
    }
    x = pop();
    t = NewTerm();
    if (x->term->type == INTTYP)
        angle = x->term->data.integer;
    else
        angle = x->term->data.floater;
    t->data.floater = angle * (3.1415926535/180);
    t->type = FLTTYP;
    push(t);
    Free(x);
    return OK;
}

int r2d() {                          // convert radians to degrees
    struct Snode* x;
    struct term* t;
    double angle;
    
    if (Sdep < 1) {
        error("execute","stack underflow","r2d");
        return UNDERFLOW;
    }
    if (Xtype!=INTTYP && Xtype!=FLTTYP) {
        error("execute","type mismatch","r2d");
        return MISMATCH;
    }
    x = pop();
    t = NewTerm();
    if (x->term->type == INTTYP)
        angle = x->term->data.integer;
    else
        angle = x->term->data.floater;
    t->data.floater = angle * (180/3.1415926535);
    t->type = FLTTYP;
    push(t);
    Free(x);
    return OK;
    return OK;
}

int depth() {
    struct Snode* x;
    int n;
    
    if (Sdep < 2) {
        error("execute","stack underflow","depth");
        return UNDERFLOW;
    }
    if (Xtype != INTTYP) {
        error("execute","type mismatch","depth");
        return MISMATCH;
    }
    x = pop();
    n = x->term->type;
    if (Sdep < (n+1)) {                 // if Sdep not enough
        error("execute","stack underflow","depth");
        drop();                         //  drop the lambda
    }
    Free(x);
    return OK;
}

void print() {                          // print X
    char str[STRSIZE];
    unsigned long len;
    
    if (Sdep < 1) {
        error("execute","stack underflow","print");
        return;
    }
    switch (Xtype){
    case STRTYP:
        strip(X->term->data.string,str);
        printf("%s\n",str);
        break;
    case INTTYP:
        printf("%li\n",X->term->data.integer);
        break;
    case FLTTYP:
        printf("%f\n",X->term->data.floater);
        break;
    default:
        error("print","print not yet implemented for this type","");
        //printf("\nprint: not implemented\n");
        break;
    } //end switch
    drop();
}

void get() {                                // get program input
    char *line = NULL;
    size_t size;
    size_t len = 0;
    int ret;
    
    size = getline(&line, &len, stdin);
    if (size > 0)
        ret = compile(line);
    else
        error("get","no input","");
}
#endif //ACLprim_h

//=== end ACLprim.h ===========================================


