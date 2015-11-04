    ==========
    SimpleText
    ==========
    A parser from Markdown-like text to plain C.
    I think it's nice. (It's an experiment.)
    (But in the end it's still code, and hurts eyes.)
    
    Unusual features:
    ----------------
    * Function calling:
      Param1.function: Param2, Param3
      or even
      Param1,Param2.function: Param3
      (All becomes `function(Param1, Param2, Param3)`)
    * Piping results:
      function: Param | function: _ | _.function:
      or (shorter)
      Param.function: | function: | function:
      other uses include
      function: Param | functionAgain: 2, _
      function: Param | Handle [void*] = _
      (alternatively)
      function: Param | = Handle [void*]
      plus
      Handle | Param.function: _
    * Boolean lists:
      + And this term
      * Or this term
      - Not this term
    * Kebab-case (translated to underscore: Kebab_case).
    * Accessing array indices and dict fields:
      Array.2
      Array.(array.len:)
      Dict|sub|field
      Dict|(Item.key:)
    * Inline C:
      `void* Pointer = malloc(10*sizeof(double));`
      or a block:
      ```
      typedef enum { Alphabet, Number, Space, UpperCase, LowerCase } letter-type;
      ```
    * Output puts function declarations at top, definitions below - no need to worry about order.
    * Function pointers:
      Function-pointer [int, char -> int]
    * Implicit functions:
      Radii.map: { R [double] -> Area [double] = PI*R*R } | = Areas [vec]
    * Implicit tuples:
      getCurrentChapter: -> Chapter [num, text, bool]
      Num [num], Text [text], Read [bool] = getCurrentChapter:
    
    To do:
    -----
    * Each block goes into a struct, not text.
    * Parse comment block as a block, not individual lines.
    * C outputter.
    
    Conventions:
    -----------
    * Functions are camelCase, types kebab-case, vars Kebab-case, and consts PascalCase.


include: "SimpleDict.h"
include: "SnipText.h"


parseSimpleText: Text [c-string]
===============
Code [dict] = newDict:
&Text.parseBlocks: &Code
&Code.outputC:
Code.freeFat:


parseBlocks: Text [c-string*], Code [dict*]
===========

do:
--
Text.popEmptyLines: | Code.addSnippet: "blank", _

while:
-----
* Text.parseIncludeLine: Code
* Text.parseCommentLine: Code
* Text.parseCommentBlock: Code
* Text.parseCBlock: Code
* Text.parseFunction: Code
* Text.parseStruct: Code


parseIncludeLine: Text [c-string*], Code [dict*] -> Success [Yes/No]
================

if:
--
+ Text.startsWith: "include: "

then:
----
return:
+ Text.popLine: ==> Code.addSnippet: "include", _


parseCommentLine: Text [c-string*], Code [dict*] -> Success [bool]
================

if:
--
+ Text.startsWith: "_"
+ Text.lineEndsWith: "_"
+ Text, 2.nthLetterInLineHasType: NonSpace
+ Text,-2.nthLetterInLineHasType: NonSpace

then:
----
return:
+ Text.popLine: ==> snipEndsBy: 1, 1 ==> Code.addSnippet: "comment", _


parseCommentBlock: Text [c-string*], Code [dict*] -> Success [bool]
=================

if:
--
+ Text.startsWithAnyOf: "  ", "\t", .

then:
----
Comment [text] = Text.popLine:
Comment.check:
Comment | Code.addText: "comment-block", _ | check:
Comment.freeFat:
return: true

else:
----
return: false


parseCBlock: Text [c-string*], Code [dict*] -> Success [bool]
===========

if:
--
+ Text.lineIs: "```"

then:
----
C-block [text] = Text.popToLine: "```"
C-block.check:
C-block | Code.addText: "c-block", _ | check:
C-block.freeFat:
Text.skipLine:
return: true

else:
----
return: false


parseFunction: Text [c-string*], Code [dict*] -> Success [yes or no]
=============

if:
--
+ Text.startsWithType: LowerCase
+ Text,1.nthWordEndsWith: ":"
+ Text,2.nthLineHasOnly: "="

then:
----
return:
+ Text.parseFunctionDeclaration: Code
+ Text.parseFunctionBody: Code

else:
----
return: false


parseFunctionLine: Text [c-string*], Code, Scope [dict*] -> Success [ok?]
=================

_Scope might be used later for context awareness - e.g. abbreviating function prefixes._
_Also for macros/generics, e.g. parse{TypeTitle}: Parameter [text] -> Result [{Type}]_

return:
* Text.parseIfBlock: Code
* Text.parseWhileBlock: Code
* Text.parseDoWhileBlock: Code
* Text.parseNestedBlock: Code
* Text.parseReturnBlock: Code
* Text.parseFunctionCall: Code
* Text.parseAssignment: Code


parseIfBlock: Text [c-string*], Code, Scope [dict*] -> Success [bool]
============

if:
--
+ Text,1.nthWordIs: "if:"
+ Text,2.nthLineHasOnly: "-"

then:
----
return:
+ Text.skipNLines: 2
+ Text.parseIfConditions: Code

else:
----
return: false


parseWhileBlock: Text [c-string*], Code, Scope [dict*] -> Success [bool]
===============

if:
--
+ Text,1.nthWordIs: "while:"
+ Text,2.nthLineHasOnly: "-"

then:
----
return: Text.parseWhileBlock: Code


parseDoWhileBlock: Text [c-string*], Code, Scope [dict*] -> Success [bool]
=================

if:
--
+ Text,1.nthWordIs: "do:"

then:
----
return: Text.parseDoWhileBlock: Code


parseNestedBlock: Text [c-string*], Code, Scope [dict*] -> Success [bool]
================

if:
--
+ Text,1.nthWordEndsWith: ":"
+ Text,2.nthLineHasOnly:  "-"

then:
----
return: Text.parseNestedBlock: Code


parseReturnBlock: Text [c-string*], Code, Scope [dict*] -> Success [bool]
================

if:
--
+ Text,1.nthWordIs: "return:"

then:
----
return: Text.parseReturnBlock: Code


parseFunctionCall: Text [c-string*], Code, Scope [dict*] -> Success [bool]
=================

if:
--
* Text,1.nthWordEndsWith: ":"
* Text,2.nthWordIs: "|"

then:
----
return: Text.parseFunctionCall: Code


parseAssignment: Text [c-string*], Code, Scope [dict*] -> Success [bool]
===============

if:
--
* Text,2.nthWordIs: "="
* Text,3.nthWordIs: "=" and Text,2.nthWordIsWrappedWith: "[", "]"

then:
----
return: Text.parseAssignment: Code

else:
----
return: false


parseStruct: Text [c-string*], Code [dict*] -> Success [bool]
===========

if:
--
+ Text.startsWith: "["
+ Text.lineEndsWith: "]"
+ Text,2.nthLineHasOnly: "="

then:
----
return:
+ Text.parseStructDeclaration: Code
+ Text.parseStructBody: Code

else:
----
return: false
