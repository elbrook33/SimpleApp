C
=

The good
--------

It's fun controlling every last detail - how to search through text, how functions are duplicated for different types.

It's a basic building block: if I were just making a programme, I wouldn't use it; but as a target for simpler, readable code, there are few intricacies, surprises and tricks to support.

C is also the most compatible with external libraries.

The bad
-------

There are many chores: memory management, setting and cleaning up data, and generalising common tasks.

I'm actually quite enjoying using macros for duplicating functions, but almost everything I'm doing has already been solved by C++.

The code is a mess of symbols, and despite the joy, the mess will only grow with time. It's unsustainable.

Where I'm leaning
-----------------

I like the process, but the result is gnarly.

C++ is the great middle ground: on the one hand, I don't really feel like using an actually good language like Nim, and having to build rickety bridges to use all the libraries I want; on the other, C is a great joy to explore, but offers no help on anything - there'll be so many traps to clear.

C++ can call C libraries, but unlike C neatly compartmentalises setting up and cleaning up variables. Destructors called when variables go out of scope are particularly useful.

Templates are ugly and virtual functions are iffy, but I don't have to use them.

Swift may be the other option: can call C and is relatively simple.