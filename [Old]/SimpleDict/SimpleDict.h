#ifndef SIMPLEDICT_H
#define SIMPLEDICT_H

/*
 * ==========
 * SimpleDict
 * ==========
 *
 * A simple dict/database.
 *
 * Conventions
 * -----------
 *  - Prefix: d_ for internal types, variables and functions.
 *  - Errors: Primitives are wrapped in a struct with an error message flag.
 *  - Prefix: _ for API functions for wrapped "optionals" (for chaining).
 *
 * Problems
 * --------
 *  - realloc destroys all references. I need an extra layer of redirection.
 *
 * To do
 * -----
 *  - Serialising to yaml. sml.
 *  - indexOf
 *  - insert, join
 *
 * Features being considered
 * -------------------------
 *  - Basic searching: findNum, findText.
 *  - SimpleText
 *  - Thread-safe: Needs locks (or queues?).
 *    Need to ensure Dict pointers haven't been freed?
 *
 * Done
 * ----
 *  - d_Dict needs to be a pointer of d_Key, so its ID can be updated.
 *  - But everything's now starting to collapse under its own weight.
 *  - All Dicts saved in top level.
 *  - "Dict" is a key.
 *  - Keys are index + string pointer.
 *  - All string keys are copied when added. (And freed when deleted.)
 *  - Key lookups check that memory addresses are the same.
 *  - If not, they search through the parent list for address.
 *  - Type is an enum.
 *  - Copy text elements.
 *  - Removing/deleting elements.
 *  - Hash or binary: Key lookups from O(N) to O(1) or O(logN)
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <pthread.h>


// 
// Data structures
// ===============
//

// Primitives
// ----------

typedef struct  d_Item d_Item;
typedef double  d_Num;
typedef char*   d_Text;
typedef d_Item* d_Dict;
typedef d_Dict  d_Err;

typedef enum { ANY=0, NUM, TEXT, DICT } d_Type;


// Structs
// -------

typedef struct d_Item {
  d_Type type;
  d_Text key;
  union {
    d_Num  valNum;
    d_Text valText;
    d_Dict valDict;
 };
} d_Item;


// Checking macros
// ---------------

#define CHECK( BOOL, MESSAGE, RETURNTYPE ) \
if ( !( BOOL ) ) {                         \
  printf ( "Error: %s.\n", MESSAGE );      \
  return RETURNTYPE##Err ( MESSAGE );      \
}

#define VALID( MAYBE, RETURNTYPE )          \
CHECK ( !MAYBE.err, MAYBE.err, RETURNTYPE )


// Optionals
// ---------

#define D_MAYBE( TYPE, ERRVAL )                \
                                               \
typedef struct d_Maybe##TYPE {                 \
  const char* err;                             \
  union {                                      \
    d_##TYPE val;                              \
    d_Dict d_max;                              \
  };                                           \
} TYPE;                                        \
                                               \
TYPE TYPE##Err ( const char* error ) {         \
  TYPE tmp;                                    \
  tmp.err = error;                             \
  tmp.val = ERRVAL;                            \
  return tmp;                                  \
}                                              \
                                               \
TYPE TYPE##OK ( d_##TYPE val ) {               \
  TYPE tmp;                                    \
  tmp.err = 0;                                 \
  tmp.val = val;                               \
  return tmp;                                  \
}

D_MAYBE( Num,  0.0  )
D_MAYBE( Text, NULL )
D_MAYBE( Dict, NULL )
D_MAYBE( Type, ANY  )
D_MAYBE( Err,  NULL )


// Optionals helpers
// -----------------

const Err OK = { NULL, NULL };

bool validate ( int count, ... )
{
  va_list list;
  va_start ( list, count );
  for ( int i = 1; i <= count; i++ ) {
    if ( va_arg ( list, Err ).err ) {
      return false;
    }
  }
  va_end ( list );
  return true;
}


// Fat arrays
// ----------

int* fat ( void* array ) {
  int* temp = (int*)array;
  temp -= 2;
  return temp;
}

void* deFat ( int* fatArr ) {
  fatArr += 2;
  return fatArr;
}

int len ( void* array ) {
  int* temp = fat(array);
  return temp[0];
}

int size ( void* array ) {
  int* temp = fat(array);
  return temp[1];
}

/*
Index 0 is kept empty, so size check is:

Array: [0 1 2 3 4 ...]
length: 0 1 2 3 4 ...
size:   1 2 3 4 5 ...

I.e.: length == last index, and
      size must be strictly greater than length.
*/

bool canFit ( int length, void* array ) {
  return size(array) > length;
}

void setFat ( int length, int size, void* array ) {
  int* temp = fat(array);
  temp[0]   = length;
  temp[1]   = size;
}

void freeFat ( void* array ) {
  int* temp = fat(array);
  free(temp);
}

#define D_FAT( TYPE, ELEM, CHUNK )                                      \
                                                                        \
TYPE allocate##TYPE ( int size )                                        \
{                                                                       \
  ELEM* temp = malloc ( 2*sizeof(int) + size*sizeof(ELEM) );            \
  CHECK ( temp, "Allocate failed", TYPE )                               \
                                                                        \
  temp = deFat((int*)temp);                                             \
  setFat ( 0, size, temp );                                             \
                                                                        \
  return TYPE##OK ( temp );                                             \
}                                                                       \
                                                                        \
Err expand##TYPE ( int add, TYPE m )                                    \
{                                                                       \
  CHECK ( !canFit( len(m.val) + add, m.val ), "Expand unneeded", Err )  \
                                                                        \
  int multiple = ( len(m.val) + add ) / CHUNK + 1;                        \
  printf ( "Attempt expand from %i to %i (%i chunks).\n", len(m.val), multiple*CHUNK, multiple ); \
  ELEM* temp = realloc ( fat(m.val),                                    \
                         2*sizeof(int) + multiple*CHUNK*sizeof(ELEM) ); \
  CHECK ( temp, "Expand failed", Err )                                  \
                                                                        \
  temp = deFat((int*)temp);                                             \
  setFat ( len(temp), multiple*CHUNK, temp );                           \
  m.val = temp; \
                                                                        \
  return OK;                                                            \
}

const int defaultDictChunk = 64;
const int defaultTextChunk = 1024;

D_FAT ( Dict, d_Item, defaultDictChunk )
D_FAT ( Text, char,   defaultTextChunk )

Dict newDict () { return allocateDict ( defaultDictChunk ); }


//
// Internal functions
// ==================
//

// Arrays, strings and data
// ------------------------

Text copyText ( const char* str )
{
  int   len  = strlen ( str );
  Text  temp = allocateText ( len + 1 );
  CHECK(temp.val, "Allocate text failed", Text);

  strcpy ( temp.val, str );
  temp.val[len] = '\0';
  CHECK ( 0 == strcmp ( temp.val, str ), "Copy failed", Text )

  setFat ( len, len+1, temp.val );
  return temp;
}


//
// API functions
// =============
//

// Get by index
// ------------

#define D_AT( TYPE, T, FIELD )                               \
                                                             \
TYPE TYPE##At ( int i, Dict d )                              \
{                                                            \
  VALID ( d, TYPE )                                          \
  CHECK ( i >= 1 && i <= len(d.val), "Out of bounds", TYPE ) \
  CHECK ( !T || d.val[i].type == T,  "Type mismatch", TYPE ) \
  return TYPE##OK ( d.val[i].FIELD );                        \
}

D_AT( Num,  NUM,  valNum  )
D_AT( Text, TEXT, valText )
D_AT( Dict, DICT, valDict )
D_AT( Type, ANY,  type    )


// Get by key
// ----------

int findKey ( const char* key, d_Type type, Dict d ) {
  for ( int i = 1; i <= len(d.val); i++ ) {
    if( 0 == strcmp( key, d.val[i].key  ) &&
      ( !type  || type == d.val[i].type ) ) {
      return i;
    }
  }
  return 0;
}

#define D_GET( TYPE, T )                   \
                                           \
TYPE get##TYPE ( const char* key, Dict d ) \
{                                          \
  VALID ( d, TYPE )                        \
  int i = findKey ( key, T, d );           \
  if ( i ) {                               \
    return TYPE##At ( i, d );              \
  } else {                                 \
    return TYPE##Err ( "Key not found" );  \
  }                                        \
}                                          \
                                           \
TYPE _get##TYPE ( Text key, Dict d )       \
{                                          \
  VALID ( key, TYPE )                      \
  return get##TYPE ( key.val, d );         \
}

D_GET ( Num,  NUM  )
D_GET ( Text, TEXT )
D_GET ( Dict, DICT )
D_GET ( Type, ANY  )


// Add item (to end)
// -----------------

#define D_ADD( TYPE, T, VALPREP, VAL )                      \
                                                            \
Err add##TYPE ( const char* key, d_##TYPE val, Dict d )     \
{                                                           \
  VALID ( d, Err )                                          \
  while ( !canFit( len(d.val)+1, d.val ) )                  \
  {                                                         \
    CHECK ( !expandDict( 1, d ).err, "Expand failed", Err ) \
  }                                                         \
  Text keyCopy = copyText ( key );                          \
  VALID ( keyCopy, Err )                                    \
  VALPREP                                                   \
  setFat ( len(d.val)+1, size(d.val), d.val );              \
  d.val[len(d.val)].type      = T;                          \
  d.val[len(d.val)].key       = keyCopy.val;                \
  d.val[len(d.val)].val##TYPE = VAL;                        \
  return OK;                                                \
}                                                           \
                                                            \
Err _add##TYPE ( Text key, TYPE val, Dict d )               \
{                                                           \
  VALID ( key, Err )                                        \
  VALID ( val, Err )                                        \
  return add##TYPE ( key.val, val.val, d );                 \
}

D_ADD ( Num,  NUM ,, val )
D_ADD ( Text, TEXT,
    Text valCopy = copyText ( val );
    VALID ( valCopy, Err ),
                     valCopy.val )
D_ADD ( Dict, DICT,, val )


// Adding dicts
// ------------

Dict addSub ( const char* key, Dict d ) {
  VALID ( d, Dict )
  Dict temp = newDict ();
  VALID ( temp, Dict )
  Err tryAdd = addDict ( key, temp.val, d );
  VALID ( tryAdd, Dict )
  return temp;
}

Dict _addSub ( Text key, Dict d ) {
  VALID ( key, Dict )
  return addSub ( key.val, d );
}


// Deleting elements
// -----------------

void d_pop ( int remove, Dict d )
{
  for ( int i = remove+1; i <= len(d.val); i++ ) {
    d.val[i-1] = d.val[i];
  }
  setFat ( len(d.val)-1, size(d.val), d.val );
}

Err delDict ( Dict d );

Err delAt ( int i, Dict d )
{
  VALID ( d, Err )
  CHECK ( i >= 1 && i <= len(d.val), "Out of bounds on delete", Err )
  switch ( d.val[i].type ) {
    case TEXT:
      freeFat ( d.val[i].valText );
      break;
    case DICT:
      delDict ( DictOK( d.val[i].valDict ) );
      break;
  }
  freeFat ( d.val[i].key );
  d_pop ( i, d );
  return OK;
}

Err delKey ( const char* key, Dict d ) {
  VALID ( d, Err )
  int i = findKey ( key, ANY, d );
  return delAt ( i, d );
}

Err delDict ( Dict d )
{
  VALID ( d, Err )
  for ( int i = 1; i <= len(d.val); i++ ) {
    delAt ( i, d );
  }
  freeFat ( d.val );
  return OK;
}


// Serialising
// -----------

void getYaml ( Dict d ) {
  printf ( "{ " );
  for ( int i = 1; i <= len(d.val); i++ ) {
    printf ( "%s: ", d.val[i].key );
    switch ( d.val[i].type ) {
      case DICT:
        getYaml ( DictOK( d.val[i].valDict ) );
        break;
      case TEXT:
        printf ( "%s", d.val[i].valText );
        break;
      case NUM:
        printf ( "%f", d.val[i].valNum );
        break;
    }
    if ( i < len(d.val) ) printf ( "," );
    printf ( " " );
  }
  printf ( "}" );
}


// Saving and loading from files
// -----------------------------

Err saveAll ( const char* path ) {
  return OK;
}

Err loadAll ( const char* path ) {
  return OK;
}

#endif
