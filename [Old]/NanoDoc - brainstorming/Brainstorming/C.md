
Text.json [json-tree]
=====================
Parameters:
* Text [text]
* Options [list]

Case: Text.length = 0
---------------------
Fail: "Input text is empty"

Case: Text.length > 0
---------------------

Match: Text        | Return
------------------ | ------------------------------------
(Label): [(Array)  | (Label.json-item, Array.json-array)
(Label): {(Object) | (Label.json-item, Array.json-object)
[(Array)           | Array.json-array
{(Object)          | Object.json-object


Text.json-array [json-tree, text]
=================================
Parameters:
* Text [text]
* Options [list]

Match: Text     | Return
--------------- | -----------------------------------
[(Nest)         | json-nested-array: Nest.json-array
{(Nest)         | json-nested-array: Nest.json-object
(Entry)] (Tail) | Entry.json-item, Tail
(Entry), (Tail) | Entry.json-item, Tail.json-array


json-nested-array [json-tree]
=============================
Parameters:
* Nest [json-tree]
* Remainder [text]

Match: Remainder | Return
---------------- | ---------------------
, (Tail)         | Nest, Tail.json-array
] (Tail)         | Nest, Tail


Text.json-item [json-tree]
==========================
Parameters: Text [text]

Match: Text | Return
----------- | ------
"(String)"  | String
(Number)    | Number
