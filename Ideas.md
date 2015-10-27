SimpleDoc (Brubeck)
===================

The point
---------

* Like HTML/CoffeeScript but without loading a whole Chromium.
* Only C has comparable libraries.


Alternatives
------------

* I can use Cello (GC, dicts, threads, streams...), but write a syntactic sugar language and parser. Earlier, I wanted to make a whole language, including run-time. But (especially after looking at Nim) now I just want to interface with C. Like CoffeeScript. (Also, the reactive stuff is cool - but a long way off, and it's hard to read and understand how the parts fits together.)
* Tricky: namespacing functions when chained (won't have type info without reading headers). When to drop prefix and when to not? (Probably never. Comes later, anyway. Like possibly named fields (as just dummy text). Equality operator??? A = B, A == B, A eq B, A is B.)
* All function calls need to end with : because I don't know if they're functions.

----

```
#include <stdio.h>
```

app.update-UI: App [app] -> [bool]
=============
App.merge: App.layout with App.data


[app]
=====
* Layout [dict]
* Data   [dict]


[dict].merge: Layout, Data [dict] -> Success [bool]
============

`bool dict__merge(dict Layout, dict Data) {`

```
nvgBeginFrame(App.state.nvg, winWidth, winHeight, pxRatio);
nvgEndFrame  (App.state.nvg);
```

nvgBeginFrame: App.state.nvg, Layout|size|width, Layout|size|height, App.state.ratio
nvgEndFrame:   App.state.nvg

`nvgBeginFrame(App.state.nvg, getItem(getItem(Layout, "size"), "width"), getItem(getItem(Layout, "size"), "height"), App.state.ratio)`

nvgBeginFrame:
* App.state.nvg
* Layout|size|width
* Layout|size|height
* App.state.ratio

if: Layout|size|width = 10
--

if: getNext: | = Var
--

if: x = y
--

if: x <= y
--

if: x > y
--

App.state.nvg.nvgBeginFrame: Layout."size"."width", Layout."size"."height", ...

nvgBeginFrame: App.state.nvg, Layout.size.width, Layout.size.height, ...

List.3.key: Tree.1.2

List."key".key: Tree."name"."field"

List|key.key: Tree|name|field, 3

    I like: Dict|key for dynamic dicts, Array.num for arrays.
    It's confusing to have two syntaxes, but it looks much nicer than ."key", and it'll let me chain: Dict|Sub.key:|Sub-sub.key:|last-sub (would be ambiguous in dot notation, whether array or dict: Dict.(Sub.key:) and Dict.(Sub.pos:)...).
    
    Or if I know the type, which I will unless returned by a function...

`nvgBeginFrame(App.state.nvg, get(get(Layout,"size"),"width"), get(get(Layout...`
`nvgBeginFrame(App.state.nvg, get(Layout,"size","width",NULL), get(Layout,"size"...`

X [int]   = 3
Y [float] = 123.4

Z [double]

nvgBeginFrame:
-------------
* context = App.state.nvg
* width   = Window-width
* height  = Window-height

= VG-pointer [void*]


nvgBeginFrame:
-------------
* App.state.nvg
* Window-width
* Window-height


if X = 1:
--------

do-something:
- - - - - - 
a | My-var1
b | My-var2

= My-var3 [int]

do-another: My-var3 | = My-var4 [double]

doOne: | doTwo: Extra-var | doThree: | = And-now--store [dict]

`dict And_now__store = doThree(doTwo(doOne(), Extra_var))`

this-f: takes-function-pointers, do-one

what-about: { x [int] -> y [int] = x + 1 }  _Ad-hoc functions (lamdas)!_

App.doThis: X  _is equivalent to_  `doThis( App, X );`
App|doThis: X  _is equivalent to_  `app_doThis( App, X );`

Head.append: Tail
Head.realloc: 32
Head.memcpy: Tail, 32

Layout|merge: 3

F: Var.modify:, Var.next, Var.do:, Var|do:, Dict|field|sub


else:
----

----


Parts
-----

* SimpleDict
  Records of data.

* SimpleDoc
  Graphical layouts, rich text and text editing.

* SimpleTask
  Concurrency and queues.

* SimpleApp
  Data + layouts -> events.


Tasks
-----

* Tables -> layout
* Dict -> populate layout
* Update dict or layout -> update UI
* Markdown -> C


Refinements
-----------

Yes, Markdown is super-nice, but make it even more simple, both in data implementation and design.

* Forget multi-threading.
* Remove some safety.
* Only doubles.
* Simple hierarchy.


Layout definitions
------------------

EmailApp
+------------------------------+--------------------------+
| MailList                     | ActiveMessage            |
| +--------------------------+ | +------------+---------+ |
| | * Messages               | | | "From:"    | From    | |
| | +--------+------+------+ | | | "To:"      | To      | |
| | | Unread | From | Date | | | | "Date:"    | Date    | |
| | |        +------+------+ | | | "Subject:" | Subject | |
| | |        | Subject     | | | +------------+---------+ |
| | +--------+-------------+ | | | Body                 | |
| +--------------------------+ | +----------------------+ |
+------------------------------+--------------------------+

+ Very human-readable.
+ Syntax colouring (per box) would be very cool.
- A pain to type out.
- A pain to parse and save (probably).

    EmailApp
      MailList.col
        Messages.list
          Unread.col
          Header.col
            Top.row
              From.col
              Date.col
            Subject.row
      ActiveMessage.col
        Header.row
          FromRow.row
            "From:"
            From
          ToRow.row
            "To:"
            To
          DateRow.row
            "Date:"
            Date
          SubjectRow.row
            "Subject:"
            Subject
        Body.row

+ More of an HTML alternative.
- Boring!


Style definitions
-----------------

From, Date @ EmailApp.LeftPane.Messages,
Body       @ EmailApp.CenterPane
----------------------------------------
bold
size: 7.5mm

* No classes (though styles still cascade).


Actions
-------

What if: nothing fancy for actions at all - just handle them in the main loop (in C)? If updating is as simple as modifying two dicts - layout and data - just do it directly with switches, etc.

One event per loop iteration, alongside one draw update. Nice. Callbacks are so... opaque.

(On the other hand, switch chains in C are ugly. _C_ is ugly.)

    app_getEvent ( &app ); // struct app.state; Dict app.layout, app.data;
    
    switch ( app.state.currentEvent.type ) {
      // NO LONGER THE FOLLOWING:
    switch ( deepInt( &app, "state", "event", "type", end ) ) {

      case NOEVENT:
        // ...
        break;
      case CLICK:
        // ...
        break;
      case KEYPRESS:
        // ...
        break;
      case DRAG:
        // ...
        break;
      // ...
    }
    
    app_updateUI ( &app );

Code syntax
-----------

d.setLen: 1           | ==> setLen(1, d);
d::XMapWindow: window | ==> XMapWindow( d, window );

+ Nice.
- Makes it harder to interface with C.
- Introduces complications and errors.
- Will be redundant with a visual editor.
