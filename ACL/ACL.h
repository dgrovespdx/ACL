//=== ACL.h 2022/05/13 =============================================
//
//    Master ACL definitions
//
//==================================================================
#ifndef ACL_h
#define ACL_h

#define COMMENT    ';'					// source code comment
#define ACLCODE    'ACL'                // ACL file identifier
#define LINESIZE    120                 // max length input line
#define AGGRSIZE    64                  // max aggregate length
#define STRSIZE     64                  // max string length
#define NAMESIZE    24                  // max len of name string
#define VECSIZE     6                   // max vector dimension
#define VERSION     0.50                // curent version number

                                    	// ACL data types
enum {NULTYP,							//     null type
	  INTTYP,                           //     128-bit integer
      FLTTYP,                           //     128-bit float
      RATTYP,                           //     rational
      CPXTYP,                           //     complex
      STRTYP,                           //     string
      TVALTYP,                          //     truth value T|F
      VECTYP,                           //     vector
      AGGTYP,                           //     aggregate
      LISTYP,                           //     list
      SETTYP,                           //     set
      MATTYP,                           //     matrix
      LAMTYP,                           //     lambda
      FUNTYP,                           //     named function
      CONTYP};                          //     constant
      
#define NUMERIC	INTTYP||FLTTYP||RATTYP||IMGTYP

                                        // internal return codes
enum {OK=0,                             //     normal
      UNKNOWN,                          //     unknown term
      UNDEFLOW,                         //     stack underflow
      MISMATCH,                         //     type mismatch
      QUIT,                             //     user quit|exit
      EOL,                              //     end of line
      FAIL=-1};                         //     operation failed

char aggbeg[] = "{([";                  // aggregate begin chars
char aggend[] = "})]";                  // aggregate ending chars

enum {IDLE=0,							// master state values
	  INTERACTING,
	  COMPILING,
	  RUNNING};

#define KNRM  "\x1B[0m"                 // printf colors
#define KRED  "\x1B[31m"				// (don't work!)

										// ACL globals
bool silent  = false;				    // controls compiler msgs.
bool tracing = false;                   // controls exe tracing
short ACLstate = IDLE;                  // current ACL state
char ACLpath[64] = "/Users/don/Projects/ACL/"; // path to ACL folder
char ACLprog[64] = "/Users/don/Projects/ACL/"; // path to ACL programs
 
										// print error messages
void error(char* func, char* reason, char* word) {  
    printf("\n\n%s: %s %s\n\n", func,reason,word);
}
#endif //ACL_h
//=== end ACL.h =====================================================
