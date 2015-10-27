
// Initialise data

NanoData top = newData ( "Example database" );
         top = getData ( "Example database" );

// Add data to data

addNum ( "Meaning of life (exact)",   42.1,                                   top );
addInt ( "Meaning of life (rounded)", 42,                                     top );
addTxt ( "Question of life",          "How many roads must a man walk down?", top );
addSub ( "January 2014",                                                      top );

// Get data from data

Num meaningNum = getNum ( 1, top );
Int meaningInt = getInt ( 2, top );
Txt question   = getTxt ( 3, top );
Txt meaningKey = getKey ( 1, top );
 
    meaningNum = valNum ( meaningKey.val,              top );
    meaningInt = valInt ( "Meaning of life (rounded)", top );
    question   = valTxt ( "Question of life",          top );
NanoData jan   = valSub ( "January 2014",              top );

// Add data to subdatas

addInt ( "Date",        201401, jan );
addNum ( "Temperature", 27.1,   jan );
addNum ( "Humidity",    0.118,  jan );

// Saving to disk

saveAll ( "~/Data" );
loadAll ( "~/Data" );

getYaml ( top );
/*
Example database:
 Meaning of life (exact): 42.1
 Meaning of life (rounded): 42
 Question of life: How many roads must a man walk down?
 January 2014:
  Date: 201401
  Temperature: 27.1
  Humidity: 0.118
*/

// Search data (later, and only maybe)

// Error handling: option 1 - Option-type
// + Concise.
// - Extra structures (for client).

MaybeNum n = valNum_d ( meaningKey, top );
meaningNum = n.err? -1.0 : n.val;

Num n = numData ( 1, top ),
    m = numData ( 2, top );

Num num = getNum ( 1, top );

validate ( a, b, c, ... );

meaningNum = valNum_d ( meaningKey, top ).val;
meaningNum = getNum_d ( 1,          top ).val;

/*

Discussion
==========
- Use long/double so everything is 64-bit. Check this. If so, can use arrays.
  (Need some 32-bit compatibility tricks later, I guess.)
 (Internally use a typedef: num_d, int_d.)
- NanoData is an ID (probably an long). Avoids all memory management in client.
- Rich text will be a whole data. (Most basically of spans.)
- Internally, use queues to make thread-safe.
 (1. Don't write if there are readers.)
 (2. Write one at a time.)
 (3. Don't read if there are writers. Add to a pending queue instead.)
 (4. Maybe keep an eye out on pending so writers don't block too long.)

*/

// Error handling: option 2 - return error, pass address
// + No extra structures.
// - Need to initialise vars beforehand.
// - No chaining.

valNum_d ( meaningKey, top, &meaningNum );

if ( getNum_d ( 1, top, &meaningNum ) );

// Error handling: option 3 - try/catch
// + Batch processing.
// + Only slight changes in syntax.
// + Checks everything - no need to manually test afterward.
// + Can have die-hard versions of functions.
// - Extra lines.
// - Awkward wording.
// - One error spoils whole batch.

NanoDo batch = do_d();

meaningNum = valNum_d ( meaningKey, top, batch );
meaningInt = valInt_d ( "Mismatch", top, batch );

if ( err_d(batch) ) {
 // Don't use any of the variables!
}

meaningNum = valNumOrDie_d ( meaningKey, top );

// Error handling: option 4 - val returns index
// + Intuitive.
// + Simple.
// - Doesn't catch type mismatches for index-based gets.
// - Manual checks for everything.

int i = indexOf_d ( meaningKey, top );
if ( i >= 1 ) meaningNum = getNum_d ( i, top );
