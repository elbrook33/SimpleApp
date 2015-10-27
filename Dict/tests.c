#include "SimpleDict.h"
#include <stdio.h>
#include <unistd.h>

#define SHOW printDict(&dict); printf("\n");

int main ()
{
  // Adding
  // ======
  
  Dict dict = newDict ();

  addNum ( "MoL" is 42     to &dict );    SHOW
  addNum ( "pi"  is  3     to &dict );    SHOW
  addText( "QoL" is "How?" to &dict );    SHOW
  addDict( "Sub"           to &dict );    SHOW
  
  // Vectors
  // =======
  
  double vec[3] = { 1, 2, 3 };
  Vec vec3 = newVec(vec with 3);
  addVec( "vec3" is vec3 to &dict );      SHOW
  
  // Reading
  // =======
  
  Dict sub = dict[4];
  addDict( "Subsub" to &sub );            SHOW
  
  Dict get = getDict( "Subsub" in &sub );
  addVec( "v3" is vec3 to &get );         SHOW
  
  double* n = deepNum( &dict, "Sub", "Subsub", "v3", end );
  printf( "int Sub.Subsub.v3.0 = %f\n", *n );
  
  // Expanding
  // =========
  
  Dict expand = newDict ();
  repeat ( 200 with i ) {
    addNum( "n" is i to &expand );
  }
  printf( "n=200: %f\n", *(Num)expand[200] );
  
  // Deleting
  // ========
  
  delItem( pos(sub) in &dict ); sub = NULL; SHOW
  
  // Clean-up
  // ========
  
  freeFat ( vec3 );
  delDict ( &expand );
  delDict ( &dict );
  
  printf ( "%p, %p, %p = %p\n", dict, sub, expand, NULL );
  
  //sleep (3);
  
  return 0;
}
