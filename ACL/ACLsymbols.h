//=== ACLsymbols.h 2022/05/07 =====================================
//
//  Implement ACL symbol table and alias table
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

#ifndef ACLsymbols_h
#define ACLsymbols_h

struct symNode {					// symbol table node
    int            type;            //    type of symbol (fun|con
	char		   name[NAMESIZE];	// 	  name of symbol
	char		   defn[AGGRSIZE];	//    associated list of terms
    long           intval;          //    integer value
    double         fltval;          //    floating value
	struct symNode *next;			//    next node in table		
};

struct symNode *symtbl[120];	    // symbol table
struct symNode* symTop = NULL;      // top of symbol table
int symDepth = 0;                   // number of alloc symbols

void InitSymbols() {
    symDepth = 0;
}

void PrintSymbols() {
    char name[NAMESIZE];
    char defn[AGGRSIZE];
    int type;
    long intval;
    double fltval;
    
    printf("\nSymbols:\n");
    for(int i=0; i<symDepth; i++) {
        type = symtbl[i]->type;
        strcpy(name,symtbl[i]->name);
        strcpy(defn,symtbl[i]->defn);
        intval = symtbl[i]->intval;
        fltval = symtbl[i]->fltval;
        
        printf("type = %i\n",type);
        printf("name = %s\n",name);
        printf("defn = %s\n",defn);
        printf("intval = %li\n",intval);
        printf("fltval = %lf\n",fltval);
    }
    printf("\n");
}

struct symNode* FindName(char* name) {      // find name in symtbl
    struct symNode* n;
    int i = 0;
    //printf("\nFindName: name = %s\n",name);
    for (i=0; i<symDepth; i++) {
        n = symtbl[i];
        if (strcmp(name,n->name) == 0) {
            return n;                       // found it
        }
    }
    return NULL;                            // not found
}

struct symNode* PutFunction(char* name,char* defn) {
	struct symNode* newNode;
	
	if (FindName(name) == NULL) {
		newNode = malloc(sizeof(struct symNode));
        newNode->type = FUNTYP;
		strncpy(newNode->name,name,NAMESIZE);
        strncpy(newNode->defn,defn,AGGRSIZE);
		symtbl[symDepth++] = newNode;
        symTop = newNode;
		return newNode;
	}
	else {
		error("PutFunction","already defined",name);
		return NULL;
	}
}

struct symNode* PutConstant(char* name,long intval,double fltval ) {
    struct symNode* newNode;
    
    if (FindName(name) == NULL) {
        newNode = malloc(sizeof(struct symNode));
        newNode->type = CONTYP;
        newNode->intval = intval;
        newNode->fltval = fltval;
        strncpy(newNode->name,name,NAMESIZE);
        //strncpy(newNode->defn,defn,AGGRSIZE);
        symtbl[symDepth++] = newNode;
        symTop = newNode;
        return newNode;
    }
    else {
        error("PutConstant","already defined",name);
        return NULL;
    }
}

struct aliasNode {                      // alias table node
    int            type;                // function or constant
    char           alias[NAMESIZE];     //    aliased name
    char           actual[NAMESIZE];    //    actual name
    struct symNode *next;               //    next node in table
};

struct aliasNode* aliases[120];         // symbol table
struct aliasNode* aliasTop = NULL;      // top of alias table
int aliasDepth = 0;                     // number of alloc aliases

void InitAliases() {
    aliasDepth = 0;
}

void PrintAliases() {
    char alias[NAMESIZE];
    char actual[NAMESIZE];
    
    printf("\nAliases:\n");
    for(int i=0; i<aliasDepth; i++) {
        strcpy(alias,aliases[i]->alias);
        strcpy(actual,aliases[i]->actual);
        
        printf("alias:%s\n",alias);
        printf("actual:%s\n",actual);
    }
    printf("\n");
}

char* FindAlias(char* this) {           // find this in aliases
    struct aliasNode* n;
    int i = 0;
    
    if (tracing) {
        printf("\nFindAlias: this = %s\n",this);
        PrintAliases();
        getchar();
    }
    for (i=0; i<aliasDepth; i++) {
        n = aliases[i];        
        if (strcmp(this,n->alias) == 0) {
            return n->actual;           // found it
        }
    }
    return "";                          // not found
}

void strip(char* this, char* str) {     // strip lead/trail chars
    int i = 0;
    
    unsigned short len = strlen(this);
    for (i=0; i<len-2; i++) {
        str[i] = this[i+1];
    }
    str[i+1] = '\n';
}

int PutAlias(char* alias,char* actual) {
    struct aliasNode* newNode;
    //char name[NAMESIZE];
    unsigned long len;
    
    if (!strcmp(FindAlias(alias),"")) {
        newNode = malloc(sizeof(struct aliasNode));
        len = strlen(alias);
        strcpy(newNode->alias,alias);
        strcpy(newNode->actual,actual);
        aliases[aliasDepth++] = newNode;
        aliasTop = newNode;
        return OK;
    }
    else {
        error("PutAlias","already defined",alias);
        return FAIL;
    }
}
#endif //ACLsymbols
//=== end ACLsymbols.h ============================================
