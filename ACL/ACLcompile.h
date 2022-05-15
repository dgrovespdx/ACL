
//=== ACLcompile.h 2022/05/10 =====================================
//
//  ACL compiler
//
//  Usage: retcode = compile(inputBuffer)
//
//         The input buffer is tokenized and each token is
//         compiled into the ACL interpreter.
//
//         retcode = OK for normal completion.
//                   UNKNOWN if an unknown term detected.
//                   EOF if end-of-file detected.
//                   QUIT if user enters "quit" or "exit".
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

#ifndef ACLcompile_h
#define ACLcompile_h

extern int execute(char* ptr);

bool TestNumber(char c, char cnext) {  // return T if number
    if (strchr("0123456789",c)) {return true;}
    if (c=='+' && (strchr("0123456789",cnext))) {return true;}
    if (c=='-' && (strchr("0123456789",cnext))) {return true;}
    return false;
}

int getnumber(char*inp,int*ndx,char*token) {   // get function token
                                               // no embedded spaces
                                               // only valid numerics
    int itok = 0;
    char c = '\0';
    /*
    while (c == ' ') {
        c = inp[*ndx]; *ndx += 1;
    }
    */
    while (1) {
        c = inp[*ndx]; *ndx += 1;
        if (c!=' ' && c!='\n' && c!=';' && c!='>') {
            if(!strchr("+-.0123456789e",c)) {
                return FAIL;
            }
            token[itok++] = c;
        }
        else {break;}                            // either ' ' or \n found
        //c = inp[*ndx]; *ndx += 1;
    }
    token[itok] = '\0';
    if (c=='\n' || c==';' || c=='>') --*ndx;// put \n or ; back
    return OK;
}

void getfunction(char*inp,int*ndx,char*token) {  // get function token
                                                 // no embedded spaces
    int itok = 0;
    char c;
    
    //printf("\ngetfunction1: ndx = %i\n",*ndx);
    
    while (1) {
        c = inp[*ndx]; *ndx += 1;
        //printf("\ngetfunction2: c=%c ndx=%i\n",c,*ndx);
        if (c!=' ' && c!='\n' && c!=';') {
            token[itok++] = c;
        }
        else break;                         // either ' ' or \n found
    }
    token[itok] = '\0';
    if (c=='\n' || c==';') --*ndx;          // put \n or ; back
}

void getalias(char*inp,int*ndx,char*token) {  // get function token
                                              //    no embedded spaces
    int itok = 0;
    char c;
    
    //printf("\ngetalias1: ndx = %i\n",*ndx);
    
    while (1) {
        c = inp[*ndx]; *ndx += 1;
        //printf("\ngetfunction2: c=%c ndx=%i\n",c,*ndx);
        if (c!=' ' && c!='\n' && c!=';') {
            token[itok++] = c;
        }
        else break;                         // either ' ' or \n found
    }
    token[itok] = '\0';
    if (c=='\n' || c==';') --*ndx;          // put \n or ; back
}

void getstring(char*inp,int*ndx,char*token) {  // get string token
                        
    int itok = 0;
    char c;
    
    //printf("\ngestring1: ndx = %i",*ndx);
    
    c = inp[*ndx]; *ndx += 1;              // accum first char
    token[itok++] = c;
    while (1) {                            // accum rest of string
        c = inp[*ndx]; *ndx += 1;
        //printf("\ngetstring2: c=%c ndx=%i",c,*ndx);
        if (c!='"' && c!='\n' && c!=';') {
            token[itok++] = c;
        }
        else break;                         // either " or \n found
    }
    token[itok++] = '"';
    token[itok] = '\0';
    if (c=='\n' || c==';') --*ndx;           // put \n or ; back
}

char getRight(char c) {
    switch (c) {
        case '(': return ')'; break;
        case '{': return '}'; break;
        case '[': return ']'; break;
        case '<': return '>'; break;
        case '"': return '"'; break;
        default:  return OK;  break;
    }
}
                                            // get aggregate token
                                            //    may have embedded spaces
                                            //    and may be nested.
                                            //    terminates when bal = 0
int getagg(char*inp,int*ndx,char*token) {
    int itok = 0;
    int code = OK;
    char c;
    char left; char right;                  // left & right delimiters
    int balance = 0;                        // 0 if lft & rght balance
    
    c = inp[*ndx]; *ndx += 1;               // get 1st char
    left = c;
    right = getRight(c);
    //printf("\ngetagg: left=%c right=%c\n",left,right);
    ++balance;
    
    token[itok++] = c;                       // accum first agg symbol
    while (1) {                              // accum rest of agg
        if (c == '\n') {
            if (balance == 0) break;
            else {
                code = UNKNOWN;
                break;
            }
            break;
        }
        c = token[itok++] = inp[*ndx]; *ndx += 1;
        if (c == right) {
            --balance;
            if (balance == 0) break;
        }
        else if (c == left) {
            ++balance;
        }
    }
    token[itok] = '\0';
    if (c == '\n') --*ndx;                    // put \n back
    return code;
}
                                              // get a vector
struct vecNode* getvector(char*inp,int*ndx) {
    struct vecNode* v;
    char num[STRSIZE];
    int ret; int n;
    char c;
    
    c = inp[*ndx]; *ndx += 1;
    if (c != '<') {return NULL;}            // not a vector
    v = NewVector();
    n = 0;
    c = inp[*ndx];
    
    while (c != '>') {
        ret = getnumber(inp,ndx,num);
        if (ret == OK) {
            v->floater[n] = atof(num);
            v->type = FLTTYP;
        }
        else {
            return NULL;
        }
        v->dim = ++n;                       // incr dimension
        if (n > VECSIZE) {break;}
        c = inp[*ndx]; *ndx += 1;
        if (c != '>') --*ndx;               // if not >, put it back
        //printf("\ngetvector: c=%c",c);
        //getchar();
    }
    if (n > 0) {
        return v;
    }
    else {return NULL;}
}

int getaggtype(char* token) {              // return aggregate type
    int c = *token;
    int d = *(token+1);
    
    if (c == '(') return LISTYP;
    if (c == '{') return SETTYP;
    if (c == '[') return LAMTYP;
    if (c == '<') {
        if (d == '<') return MATTYP;
        else          return VECTYP;
    }
    return UNKNOWN;
}

int defun(char* name) {                           // define a function
    
    char defn[AGGRSIZE];
    struct symNode* p;
    struct Snode* n;
    
    if(!silent && ACLstate==COMPILING)
        printf("\ncompiler: compiling %s",name);
    n = pop();                          // get the lambda
    strcpy(defn,n->term->data.aggr);
    Free(n);
    p = PutFunction(name,defn);         // add name to symbol table
    if(!silent && ACLstate==COMPILING)
        printf("\ncompiler: function %s defined\n",name);
    if (p !=NULL) return OK;
    else          return FAIL;
}

int defcon(char* name) {                 // define a constant

    struct symNode* p;
    struct Snode* n;
    long intval;
    double fltval;
    
    if(!silent && ACLstate==COMPILING)
        printf("\ncompiler: compiling %s",name);
    n = pop();                           // get the number
    intval = n->term->data.integer;
    fltval = n->term->data.floater;
    Free(n);
    p = PutConstant(name,intval,fltval); // add name to symbol table
    if(!silent && ACLstate==COMPILING)
        printf("\ncompiler: constant %s defined\n",name);
    if (p !=NULL) return OK;
    else          return FAIL;
}

int alias(char* actual) {               // define an alias
    struct aliasNode* p;
    struct Snode* n;
    char alias[NAMESIZE];
    char name[NAMESIZE];
    int ret;
    
    n = pop();                           // get the alias
    strcpy(alias,n->term->data.aggr);
    Free(n);
    strip(alias,name);
    
    ret = PutAlias(name,actual);           // add pair to aliases
    if(!silent && ACLstate==COMPILING)
        printf("\ncompiler: alias %s defined",name);
    return OK;
}

int compile(char* bfr) {                    // compile tokens
    int type = 0; int ret;
    struct term* termptr;
    struct vecNode* vecptr = NULL;
    char token[80] = "";
    char actual[NAMESIZE];
    char c; char cnext;
    int code = OK;;                         // return code
    int ndx = 0;                            // index into bfr
    
    if (tracing) {
        printf("\ncompile: bfr = %s\n",bfr);
        getchar();
    }
    
    while(1) {                              // loop until \n or EOF
        c = bfr[ndx];                       // examine next input char
   
        if (c == ' ') {
            while (c == ' ') {              // eat leading spaces
                c = bfr[ndx++];
            }
            --ndx;                          // put back first non-space
        }
        
        if (c=='\n' || c==';') {            // is it end of line?
            *token = c;
            code = EOL;
            break;                                
        }
        cnext = bfr[ndx+1];
        if (TestNumber(c,cnext)) {           // is it a number?
            ret = getnumber(bfr,&ndx,token);    //   yes - get number
            if (ret != OK) {
                error("compile","invalid numeric",token);
                code = FAIL;
                break;
            }
            //printf("\ncompile: number = %s\n",token);
            if (strstr(token,"."))               // check for '.'
                 type = FLTTYP;
            else type = INTTYP;
            }
        else if (c == '"') {                     // is it a string?
            getstring(bfr,&ndx,token);
            type = STRTYP;
        }
        else if (c == '<') {                    // is it a vector?
            if (cnext=='?') {                   // allow <?
                execute("<?");
                break;
            }
            if (cnext == '<') {                 // is it a matrix?
                printf("Matricies not yet implemented\n");
                break;
            }
            vecptr = getvector(bfr,&ndx);
            //PutVector(vecptr);
            type = VECTYP;
            if (vecptr == NULL) {
                error("compile","malformed term",token);
                break;
            }
            //PrintVector(vecptr);
            //getchar();
        }
        /*
        else if (c == '[') {                    // is it a lambda?
            ret = getlambda(bfr,&ndx,token);
            if (ret != OK) {
                error("compile","malformed term",token);
                code = UNKNOWN;
                break;
            }
            type = LAMTYP;
        }
        */
        else if (c == '(') {
            printf("Lists not yet implemented\n");
            break;
        }
        else if (c == '{') {
            printf("Sets not yet implemented\n");
            break;
        }
        else if (c == '\\') {
            printf("Lambda calculus not yet implemented\n");
            break;
        }
        else if (strchr(aggbeg,c)) {            // is it an aggregate?
            ret = getagg(bfr,&ndx,token);       //   yes - get token
            if (ret != OK) {
                error("compile","malformed term",token);
                code = UNKNOWN;
                break;
            }
            type = getaggtype(token);              //   get aggr type
        }
        else {
            getfunction(bfr,&ndx,token);           // must be function
            type = FUNTYP;
            if (!strcmp(token,"quit") ||           // is it quit|exit?
               (!strcmp(token,"exit"))) {
                code = QUIT;
                break;
                    
                }
            }
        
        //printf("\ncompile1: token=%s type=%i\n",token,type);
        if (type != FUNTYP) {                      // is it a literal?
            termptr = NewTerm();
            termptr->type = type;
            if (type == INTTYP) {                  // make int term
                termptr->data.integer = atol(token);
            }
            else if (type == FLTTYP) {              // make float term
                termptr->data.floater = atof(token);
            }
            else if (type == VECTYP) {            //t make vector term
                termptr->data.vecptr = vecptr;
            }
            else if (type == STRTYP) {              // make string term
                strncpy(termptr->data.string,token,AGGRSIZE);
            }
            else if (type == LAMTYP) {              // make lambda term
                //termptr->data.integer = 0;        // use aggr for now...
                //termptr->data.floater = 0.0;
                strncpy(termptr->data.aggr,token,AGGRSIZE);
            }
            else if (type > AGGTYP) {             // make aggr term
                termptr->data.integer = 0;
                termptr->data.floater = 0.0;
                strncpy(termptr->data.aggr,token,AGGRSIZE);
            }
            else {
                error("compile","unknown term",token);
                code = UNKNOWN;
                break;
            }
            push(termptr);
        }
        else {                                // must be function
            if (!strcmp(token,"defun")) {     // if defun get name
                getfunction(bfr,&ndx,token);
                defun(token);
                break;
            }
            if (!strcmp(token,"defcon")) {    // if defcon get name
                getfunction(bfr,&ndx,token);
                defcon(token);
                break;
            }
            if (!strcmp(token,"alias")) {     // if alias get name
                getfunction(bfr,&ndx,actual);
                alias(actual);
                break;
            }
            if (!strcmp(token,"silent")) {    // set silent mode
                silent = true;
                break;
            }
            execute(token);
        }
    }
    //free(bfr);
    return code;
}
#endif //ACLcompile_h

//=== end ACLcompiler.h =================================================
