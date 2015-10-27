NanoDoc specs
=============

* NanoDB / DataStacks for tree.
* NanoVG for drawing straight to GL.
* Markdown-inspired markup format.
* Plain C code (with macros).

Features:
* Lists: automatic populating from data, selections, and drag and drop.
* Rich-text: editing.
* Scrolling.

Questions:
* How to relay actions?
  Function pointers (callbacks): compiled, not scripted.
  (Linking shared libraries - libtool, etc. - is too complicated.)
* How to write styles?
* How to point to actions? A big switch, I guess.
