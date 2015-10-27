#ifndef DOC_H
#define DOC_H

#include "style.h"
#include "lists.h"

#define SUB_BLOCK(parent, child)                          \
typedef struct parent {                                   \
  list_##child child;                                     \
  style style;                                            \
} parent;

LIST(char)
LIST(span)
LIST(col)
LIST(block)
LIST(doc)

SUB_BLOCK(doc, block)
SUB_BLOCK(block, col)
SUB_BLOCK(col, span)
SUB_BLOCK(span, char)

#endif