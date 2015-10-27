/*
 * ===============
 * SimpleDict (v2)
 * ===============
 *
 * Fully fat-pointer.
 *
 * Advantages
 * ----------
 *  + Hugely simplified chaining and API.
 *  + Common strings and numbers contain their labels, positions lengths and sizes.
 *  + Transparent error handling (not yet implemented).
 *  + Far simpler syntax (fewer -> and .s).
 *  + Half the number of types and layers.
 *  + Vectors (of doubles) for free, apparently.
 *
 * Disadvantages
 * -------------
 *  - Everything's a pointer, and everything's on the heap.
 *  - Syntax for dereferencing gets a bit verbose.
 *   (Only add and del need pointer to dict, but that only leaves get, so do all.)
 *   (It's an internal/external problem - i.e. it's nice for users - so leave it.)
 *  - You can only see you have the extra features if you know the source,
 *    i.e. it won't check at compile-time.
 * 
 * To do
 * -----
 *  * Use it in something! Parse Markdown!
 *  * An extra library of filter, map, search, join, replace, etc.
 *  * Keep refining error handling (and checks).
 *  * Memory leaks?
 *
 */

#ifndef SIMPLEDICT_H
#define SIMPLEDICT_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

// Language mods
// =============

#define in ,
#define on ,
#define is ,
#define to ,
#define from ,
#define with ,
#define and &&
#define or ||
#define end NULL

#define raise(ERR) RAISE(ERR)
#define RAISE(ERR, BLOB) setErr(ERR, BLOB); return 0;

#define check(BLOB)                           \
if(!BLOB) return 0; else if(err(BLOB)) {      \
  printf("Error: %s\n", err(BLOB)); return 0; }

#define foreach(COND) FOREACH(COND)
#define FOREACH(I, B) for(int I=1; I<=len(B); I++)
#define repeat(TIMES) REPEAT(TIMES)
#define REPEAT(N, I)  for(int I=1; I<=N; I++)
#define loop(COND)    LOOP(COND)
#define LOOP(I, A, B) for(int I=A; I<=B; I++)

//
// Data structures
// ===============
//

typedef double* Num;
typedef char* Text;
typedef void* Item;
typedef Item* Dict;
typedef enum { ANY=0, NUM, TEXT, DICT } Type;
typedef Num Vec;

// Fat pointers
// ------------

typedef struct Fat {
  const char* err;
  Text key;
  Type type;
  int length;
  int size;
  int pos;
} Fat;

Fat* getFat ( void* skinny ) {
  char* temp = (char*) skinny;
  temp -= sizeof(Fat);
  return (Fat*) temp;
}

const char* err (void* blob) { return getFat(blob)->err;    }
Text  key       (void* blob) { return getFat(blob)->key;    }
Type  type      (void* blob) { return getFat(blob)->type;   }
int   len       (void* blob) { return getFat(blob)->length; }
int   size      (void* blob) { return getFat(blob)->size;   }
int   pos       (void* blob) { return getFat(blob)->pos;    }

void setErr (const char* e, void* blob) { getFat(blob)->err    = e; }
void setKey (Text k,        void* blob) { getFat(blob)->key    = k; }
void setType(Type t,        void* blob) { getFat(blob)->type   = t; }
void setLen (int l,         void* blob) { getFat(blob)->length = l; }
void setSize(int s,         void* blob) { getFat(blob)->size   = s; }
void setPos (int p,         void* blob) { getFat(blob)->pos    = p; }
void setFat (Fat* f,        void* blob) {*getFat(blob)         =*f; }

bool freeFat (void* blob) {
  check(blob);
  Fat* fat = getFat(blob);
  if ( fat->key ) freeFat(fat->key);
  free(fat);
  return true;
}

bool bounds (int i, void* blob) { return i>=1 and i<=len(blob); }
bool isNum  (void* blob) { return type(blob) == NUM;  }
bool isText (void* blob) { return type(blob) == TEXT; }
bool isDict (void* blob) { return type(blob) == DICT; }

#define FAT_TYPE( NAME, TYPE, T, CHUNK )                    \
                                                            \
TYPE* allocate##NAME ( int size )                           \
{                                                           \
  char* temp = malloc(sizeof(Fat) + (size+1)*sizeof(TYPE)); \
  if ( !temp ) { printf("malloc failed\n"); return NULL; }  \
                                                            \
  temp += sizeof(Fat);                                      \
  setSize(size+1, temp);                                    \
  setLen (     0, temp);                                    \
  setType(     T, temp);                                    \
  setKey (  NULL, temp);                                    \
  setErr (  NULL, temp);                                    \
  setPos (     0, temp);                                    \
                                                            \
  return (TYPE*)temp;                                       \
}                                                           \
                                                            \
bool expand##NAME ( int add, TYPE** d )                     \
{                                                           \
  int amount = add > CHUNK? add : CHUNK;                    \
  char* temp = realloc (                                    \
    getFat(*d),                                             \
    sizeof(Fat) + (size(*d)+amount+1)*sizeof(TYPE)          \
  );                                                        \
  if( !temp ) { raise("realloc failed" on *d); }            \
                                                            \
  temp += sizeof(Fat);                                      \
  *d = (TYPE*)temp;                                         \
  setSize(size(*d) + amount, *d);                           \
  return true;                                              \
}

FAT_TYPE ( Num,  double, NUM,  128  );
FAT_TYPE ( Text, char,   TEXT, 1024 );
FAT_TYPE ( Dict, Item,   DICT, 64   );

void printFat ( void* blob ) {
  Fat* f = getFat ( blob );
  printf( "err %s - key %s - type %i - length %i - size %i - pos %i\n",
        f->err,  f->key,  f->type,  f->length,  f->size,  f->pos );
}

//
// API functions
// =============
//

// Helpers
// -------

Dict newDict () {
  return allocateDict ( 64 );
}

Num newNum ( double val ) {
  Num num = allocateNum ( 1 ); check(num);
  *num = val;
  setLen( 1, num );
  return num;
}

Num newVec ( double* vec, int length ) {
  Num num = allocateNum ( length ); check(num);
  memcpy( num from vec, length*sizeof(double) );
  setLen( length, num );
  return num;
}

Num copyVec( Vec vec ) { return newVec ( vec, len(vec) ); }

Text copyText ( const char* source ) {
  int  len  = strlen ( source );
  Text text = allocateText ( len ); check(text);
  strcpy( text from source );
  text[len] = '\0';
  setLen( len, text );
  return text;
}

// Adding
// ------

// addItem needs pointer of Dict because expand could change where it's stored.

Item addItem ( Item item, Dict* d )
{
  if (size(*d) <= len(*d)+1) { expandDict(1, d); }
  if (size(*d) <= len(*d)+1) { raise("realloc failed" on *d); }
  
  setLen( len(*d)+1, *d );
  (*d)[len(*d)] = item;
  
  setPos( len(*d), item );
  return item;
}

#define FAT_ADD( NAME, FIELD, VARS, INIT ) \
                                           \
NAME add##NAME VARS                        \
{                                          \
  NAME FIELD = INIT;                       \
  setKey( copyText(label), FIELD );        \
  return addItem ( FIELD, d );             \
}

FAT_ADD( Num,  num,  (const char* label, double val, Dict* d),      newNum(val)   )
FAT_ADD( Text, text, (const char* label, const char* val, Dict* d), copyText(val) )
FAT_ADD( Dict, dict, (const char* label, Dict* d),                  newDict()     )
FAT_ADD( Vec,  num,  (const char* label, Vec val, Dict* d),         copyVec(val)  )

// Getting
// -------

int indexOf ( const char* label, Dict* d )
{
  foreach( i in *d ) {
    if ( 0 == strcmp( label, key((*d)[i]) ) ) {
      return i;
    }
  }
  raise ( "Key not found" on *d );
}

Item getItem ( const char* label, Dict* d )
{
  int i = indexOf( label in d ); check(*d);
  if( !bounds( i in *d ) ) { raise("Item not found" on *d); }
  return (*d)[i];
}

#define FAT_GET( NAME )                                       \
                                                              \
NAME get##NAME ( const char* label, Dict* d )                 \
{                                                             \
  NAME temp = getItem ( label in d ); check(*d); check(temp); \
  if ( !is##NAME(temp) ) { raise("Type mismatch" on *d); }    \
  return temp;                                                \
}

FAT_GET ( Num  )
FAT_GET ( Text )
FAT_GET ( Dict )

#define FAT_DEEP( NAME )                         \
                                                 \
NAME deep##NAME ( Dict* d, ... )                 \
{                                                \
  Dict sub = *d;                                 \
  const char *label, *next;                      \
  va_list args;                                  \
  va_start ( args, d );                          \
  label = va_arg( args, const char* );           \
  while ( (next = va_arg( args, const char* )) ) \
  {                                              \
    sub = getDict ( label, &sub ); check(sub);   \
    label = next;                                \
  }                                              \
  va_end ( args );                               \
  return get##NAME ( label, &sub );              \
}

FAT_DEEP ( Num  )
FAT_DEEP ( Text )
FAT_DEEP ( Dict )

//#define getInt  (int)*getNum
//#define deepInt (int)*deepNum
// Removed because these cases are better served with typed structs,
// and they'll fail hard trying to dereference NULL.

// Deleting
// --------

bool shift ( int hole, Dict* d )
{
  loop( i from hole+1 to len(*d) )
  {
    setPos(i-1, (*d)[i]);
    (*d)[i-1] = (*d)[i];
  }
  setLen( len(*d)-1, *d );
  return true;
}

bool delDict ( Dict* d );

bool delItem ( int i, Dict* d )
{
  if( !bounds(i in *d) ) { raise("delItem: Out of bounds" on *d); }
  switch( type((*d)[i]) )
  {
    case DICT:
      delDict( (Dict*)(*d+i) );
      break;
    
    case NUM:
    case TEXT:
      freeFat( (*d)[i] );
      break;
    
    default:
      raise( "delItem: No type" on *d );
      break;
  }
  shift(i, d);
  return true;
}

// delDict needs pointer of Dict because it'll be set to NULL once freed.

bool delDict ( Dict* d )
{
  foreach( i in *d ) {
    delItem(i, d); check(*d);
  }
  freeFat(*d);
  *d = NULL;
  return true;
}

// Serialising
// -----------

void printDict ( Dict* d )
{
  printf( "{ " );
  foreach( i in *d )
  {
    printf( "\"%s\": ", key((*d)[i]) );
    switch( type((*d)[i]) )
    {
      case NUM:;
        Num n = (*d)[i];
        if ( len(n) > 1 )
        {
          printf( "[ " );
          loop( j from 0 to len(n)-1 ) // From 0 (only for vec).
          {
            printf( "%f", n[j] );
            if ( j < len(n)-1 ) printf( "," );
            printf( " " );
          }
          printf( "]" );
        }
        else
        {
          printf( "%f", *n );
        }
        break;
      
      case TEXT:
        printf( "\"%s\"", (Text)(*d)[i] );
        break;
      
      case DICT:
        printDict( (Dict*)*d+i );
        break;
      
      default:
        printf( "unknown %i", type((*d)[i]) );
    }
    if ( i < len(*d) ) printf( "," );
    printf( " " );
  }
  printf( "}" );
}

#endif
