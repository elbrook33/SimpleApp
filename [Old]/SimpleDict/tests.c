#include "SimpleDict.h"
#include <stdio.h>

int main ()
{
  // Part 1: Init
  // ------------
  
  Dict dict = newDict ();

  if ( validate ( 1, dict ) ) {
    printf ( "Dict ready (%i/%i).\n", len(dict.val), size(dict.val) );
  } else {
    printf ( "Error: %s.\n", dict.err );
  }


  // Part 2: Add and get (for primitives)
  // ------------------------------------
  
  addNum  ( "Meaning of life", 42,                       dict );
  addText ( "Prompt",          "The meaning of life is", dict );

  Num meaning = getNum  ( "Meaning of life", dict );
  Text prompt = getText ( "Prompt",          dict );

  if ( validate ( 2, meaning, prompt ) ) {
    printf ( "%s %f.\n", prompt.val, meaning.val );
  } else {
    printf ( "Errors: %s, %s.\n", prompt.err, meaning.err );
  }


  // Part 3: Sub-dicts
  // -----------------

             addSub  ( "January 20, 2015", dict );
  Dict sub = getDict ( "January 20, 2015", dict );

  if ( validate ( 1, sub ) ) {
    printf ( "Sub-dict (%i/%i).\n", len(sub.val), size(sub.val) );
  } else {
    printf ( "Error: %s.\n", sub.err );
  }

              addNum ( "Temperature (C)", 8.1,       sub    );
  Num temp1 = getNum ( "Temperature (C)", DictAt( 3, dict ) );

  if ( validate ( 1, temp1 ) ) {
    printf ( "The temperature was %f C.\n", temp1.val );
  } else {
    printf ( "Error: %s.\n", temp1.err );
  }


  // Part 4: Expanding
  // -----------------

  Dict big = newDict ();

  for ( int i = 1; i <= 200; i++ ) {
    addNum ( "x", 3.2, big );
  }

  Num n100 = NumAt ( 100, big );

  if ( validate ( 2, big, n100 ) ) {
    printf ( "Item 100/%i = %f.\n", size(big.val), n100.val );
  } else {
    printf ( "Error: %s.\n", n100.err );
  }

  // Part 5: Deleting
  // ----------------

  addText ( "New text",   "Starts at index = 2", sub );
  addNum  ( "New number", 3,                     sub );
  delAt   ( 1,                                   sub );

  Num nx = NumAt ( 3, sub ); // Out of bounds.
  Num n1 = NumAt ( 2, sub );

  if ( validate ( 1, n1 ) ) {
    printf ( "Num: %f.\n", n1.val );
  } else {
    printf ( "Error: %s.\n", n1.err );
  }

        delKey  ( "January 20, 2015", dict );
  sub = getDict ( "January 20, 2015", dict );

  if ( validate ( 1, sub ) ) {
    printf ( "Removed sub: %i.\n", size(sub.val) );
  } else {
    printf ( "5.2 Error: %s.\n", n1.err );
  }

  nx  = NumAt ( 2, sub );

  if ( validate ( 1, nx ) ) {
    printf ( "Sub-dict (%i/%i).\n", len(sub.val), size(sub.val) );
  } else {
    printf ( "5.3 Error: %s.\n", nx.err );
  }


  // Part 6: Serialising
  // -------------------
  
  sub = addSub ( "Jan 22, 2030", dict );
        addNum ( "A", 3,         sub  );
        addNum ( "B", 4,         sub  );
  sub = addSub ( "3.14am",       sub  );
  sub = addSub ( "12 sec",       sub  );
        delKey ( "Jan 22, 2030", dict );

  sub = addSub ( "Jan 23, 2030",     dict );
        addNum ( "A", 6,             sub  );
        addNum ( "B", 7,             sub  );
  sub = addSub ( "7.14am",           sub  );
        addText( "21.1 sec", "Soon", sub  );
        addText( "21.2 sec", "Now",  sub  );

  getYaml( dict );
  printf ( "\n" );


  // Part 7: Saving and loading from files
  // -------------------------------------


  return 0;
}

