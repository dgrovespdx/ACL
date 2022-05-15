//=== ACLexecute.h 2022/05/10 =====================================
//
//  ACL execution definitions
//
//  Executes ACL functions according to their definition.
//
//  Usage: retcode = execute(function).
//
//  Primitive functions are executed directly. Non-primitive
//  functions are searched for in the ACL symbol table.
//
//  retcode = OK for normal operation.
//          = UNKNOWN if function is not found.
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
//==================================================================

#ifndef ACLexecute_h
#define ACLexecute_h

bool isNumeric(struct Snode* this) {        // return T if X is numeric
    if (X->term->type == INTTYP ||
        X->term->type == FLTTYP ||
        X->term->type == RATTYP ||
        X->term->type == CPXTYP)
         return true;
    else return false;
}

int execute(char* this) {                   // execute given function
    struct symNode* p;
    struct term* t;
    char name[NAMESIZE];
    char defn[AGGRSIZE];
    char c = ' ';
    long intval = 0;
    double fltval = 0.0;
    int type = 0;
    bool doit = false;
    int ret = 0;
    
    if (tracing) {
        printf("\nexecute: this = %s\n",this);
        getchar();
    }
                                            // stack operations
    if (!strcmp(this,"pop"))    {pop();    return OK;}
    if (!strcmp(this,"dup"))    {dup_();   return OK;}
    if (!strcmp(this,"drop"))   {drop();   return OK;}
    if (!strcmp(this,"swap"))   {swap();   return OK;}
    if (!strcmp(this,"swapYZ")) {swapYZ(); return OK;}
    if (!strcmp(this,"under"))  {under();  return OK;}
    if (!strcmp(this,"rotup"))  {rotup();  return OK;}
    if (!strcmp(this,"rotdn"))  {rotdn();  return OK;}
    if (!strcmp(this,"empty"))  {empty();  return OK;}

    if (strlen(this)==1 && !strcmp(this,"!")) {
        fact();
        return OK;
    }
    if (strstr("=?!=?>?>=?<?<=?",this)) {   // predicates
        c = predop(this);
        if (c != ' ') {
            termptr = (struct term*)malloc(sizeof(struct term));
            termptr->type = TVALTYP;
            termptr->data.tval = c;
            drop(); drop(); push(termptr);
            return OK;
        }
        else return FAIL;
    }
    
    if (strstr("+-*/%^",this) && isNumeric(X)) {
        {ret = binaryop(this,&intval,&fltval,&type); doit = true;}
    }
    else if (strstr("^2^3negsqrt",this)) {
        {ret = unaryop(this,&intval,&fltval,&type); doit = true;}
    }
    
    if (ret == UNDERFLOW) {
        error("execute","stack underflow ",this);
        return FAIL;
    }
    if (ret == MISMATCH) {
        error("execute","mismatched types",this);
        return FAIL;
    }
    if (doit) {
        termptr = (struct term*)malloc(sizeof(struct term));
        if (type == INTTYP) {
            termptr->type = INTTYP;
            termptr->data.integer = intval;
            //PrintTerm(termptr);
        }
        else {
            termptr->type = FLTTYP;
            termptr->data.floater = fltval;
            //PrintTerm(termptr);
        }
        push(termptr);
        return OK;
    }
    if (strstr("+-*.",this)) {
        ret = vecop(this);
        return OK;
    }
    if (!strcmp(this,"T") || (!strcmp(this,"F"))) {
        termptr = NewTerm();
        termptr->type = TVALTYP;
        termptr->data.tval = *this;
        push(termptr);
        return OK;
    }
    if (!strcmp(this,"tracing")) {
        tracing = !tracing;
        return OK;
    }
    if (!strcmp(this,"or"))      {or();     return OK;}
    if (!strcmp(this,"and"))     {and();    return OK;}
    if (!strcmp(this,"not"))     {not();    return OK;}
    if (!strcmp(this,"B"))       {B();      return OK;}
    if (!strcmp(this,"!"))       {fact();   return OK;}
    if (!strcmp(this,"if"))      {if_();    return OK;}
    if (!strcmp(this,"ifelse"))  {ifelse(); return OK;}
    if (!strcmp(this,"for"))     {for_();   return OK;}
    if (!strcmp(this,"while"))   {while_(); return OK;}
    if (!strcmp(this,"incr"))    {incr();   return OK;}
    if (!strcmp(this,"decr"))    {decr();   return OK;}
    if (!strcmp(this,"sin"))     {trigop(this); return OK;}
    if (!strcmp(this,"cos"))     {trigop(this); return OK;}
    if (!strcmp(this,"tan"))     {trigop(this); return OK;}
    if (!strcmp(this,"d2r"))     {d2r();    return OK;}
    if (!strcmp(this,"r2d"))     {r2d();    return OK;}
    if (!strcmp(this,"get"))     {get();    return OK;}
    if (!strcmp(this,"depth"))   {depth();  return OK;}
    if (!strcmp(this,"print"))   {print();  return OK;}
    
    p = FindName(this);                     // search symbol table
    if (p != NULL) {                        // execute fun defn
        unsigned short len = strlen(p->defn);
        strncpy(defn,p->defn,len);
        defn[len-1] = '\n';                 // eliminate final ]
        if (p->type == FUNTYP) {
            compile(&defn[1]);              // execute function
            return OK;
        }
        /*
        t = NewTerm();
        unsigned short len = strlen(p->defn);
        strncpy(t->data.aggr,p->defn,len);
        t->type = LAMTYP;
        push(t);
        B();
        }
        */
        else {                              // push constant
            t = (struct term*)malloc(sizeof(struct term));
            t->type = FLTTYP;
            t->data.integer = p->intval;
            t->data.floater = p->fltval;
            push(t);
            return OK;
        }
    }
                                        
    strncpy(name,FindAlias(this),NAMESIZE);
    if (strcmp(name,"")) {                 // if alias found
        execute(name);                     //    execute it
        return OK;
    }

    error("exec","unknown term",this);
    return UNKNOWN;
}

#endif //ACLexecute_h
//=== end ACLexecute.h ========================================
 
