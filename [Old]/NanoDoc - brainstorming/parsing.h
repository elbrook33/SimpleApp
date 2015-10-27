#ifndef PARSING_H
#define PARSING_H

#include "style.h"
#include <stdio.h>

// Styles

style ParseStyle (string Tags, string Block) {
  string Name;
  if (Count ("\"", Tags) == 2) {
           FwdTo ("\"", &Tags);
    Name = FwdTo ("\"", &Tags);
  } else {
    Name = _("default");
  }
  style Style = MakeStyle (Name);
  string Label, Value;
  while (Block.length > 0) {
    SkipSpaces (&Block);
    Label = FwdTo (":", &Block);
    Value = FwdTo ("\n", &Block);
    SkipSpaces (&Value);
    AddStyleProperty (Label, Value, &Style);
  }
  return Style;
}

list_style ParseStyles (const char* Text) {
  string T = _(Text);
  int N = Count ("\nStyle", T);
  list_style Styles;
  style List[N];
  List_style (&Styles, List, N);
  string Tags, Block;
  FwdTo ("Style", &T);
  while (T.length > 0) {
    Tags  = FwdTo (":", &T);
    Block = FwdTo ("\nStyle", &T);
    Add_style (ParseStyle(Tags, Block), &Styles);
  }
  return Styles;
}

// Elements

int N_span  (string In) { return In.length; }
int N_col   (string In) { return Count ("    Span", In); }
int N_block (string In) { return Count ("  Column", In); }
int N_doc   (string In) { return Count ("\nBlock" , In); }

list_ Label_col

span Parse_span (string In) {
  style Style;
  list_char List;
  char Array[In.length];
  List_char (&List, Array, In.length);
  
  span Span;
  Span.char = List;
  Span.style = Style;
}

col Parse_col (style Parent, list_style Styles, substr Text) {
  // Extract header
  substr Header = FwdTo (":", &Text);
  style Style = Parse_style_classes (Header, Styles);

  // Split spans
  int N = Count (SPAN_DELIM, Text);
  FwdTo (SPAN_DELIM, &Text);
  list_substr Blocks = Split (SPAN_DELIM, Text);
  span* Array = Map_list_substr_to_span (Parse_span, Blocks);

  // Finalise data
  list_span List;
  list_span_attach (&List, Array, N);
  col Column;
  Column.sub   = List;
  Column.style = Style;
  
  return Column;
}

block Parse_block (string Text) {
}

doc Parse_doc (string Text) {
}

#endif