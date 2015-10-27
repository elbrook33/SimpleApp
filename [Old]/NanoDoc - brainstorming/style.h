#ifndef STYLE_H
#define STYLE_H

#include "lists.h"
#include "simplestring.h"
#include <stdbool.h>
#include <stdlib.h>

// Style data

typedef struct style {
  string name, font;
  float size, line,
        top, bottom, left, right,
        width, height;
  bool  bold, italic;
  vec4  bg, color;
  string before;
} style;

LIST(style)

// Style helpers

static style DefaultStyle = {
  {"default", 7, 8}, {"Serif", 5, 6}, 11, 15,
  0, 0, 0, 0, 0, 0,
  0, 0,
  {1.0, 1.0, 1.0, 1.0}, {0.0, 0.0, 0.0, 1.0},
  {"", 0, 1}
};

style MakeStyle (string Name) {
  style Style = DefaultStyle;
  Style.name = Name;
  return Style;
}

#define STYLES 14
typedef enum {FONT, SIZE, LINE, TOP, BOTTOM, LEFT, RIGHT, WIDTH, HEIGHT, BOLD, ITALIC, BG, COLOR, BEFORE} styleField;
struct styleMap {styleField field; const char* label;};
struct styleMap StyleMap[STYLES] = {
  {FONT, "font"}, {SIZE, "size"}, {LINE, "line"},
  {TOP, "top"}, {BOTTOM, "bottom"}, {LEFT, "left"}, {RIGHT, "right"},
  {WIDTH, "width"}, {HEIGHT, "height"},
  {BOLD, "bold"}, {ITALIC, "italic"}, {BG, "bg"}, {COLOR, "color"},
  {BEFORE, "before"}
};
styleField StyleField (const char* Label) {
  int i;
  styleField Field;
  for (i = 0; i < STYLES; i++) {
    if (strcmp (Label, StyleMap[i].label) == 0) {
      Field = StyleMap[i].field;
      break;
    }
  }
  return Field;
}

float str2f (const char* Text) {
  return atof (Text);
}

void AddStyleProperty (string Field, string Value, style* Style)
{
  char *F = Copy (Field),
       *V = Copy (Value);
  switch(StyleField(F)) {
    case FONT:   Style->font   = Value; break;
    case SIZE:   Style->size   = atof (V); break;
    case LINE:   Style->line   = atof (V); break;
    case TOP:    Style->top    = atof (V); break;
    case BOTTOM: Style->bottom = atof (V); break;
    case LEFT:   Style->left   = atof (V); break;
    case RIGHT:  Style->right  = atof (V); break;
    case WIDTH:  Style->width  = atof (V); break;
    case HEIGHT: Style->height = atof (V); break;
    case BOLD:   Style->bold   = strcmp ("false", V) != 0; break;
    case ITALIC: Style->italic = strcmp ("false", V) != 0; break;
    case BG:
      string* List = Split (",", _(V));
      Style->bg = Map4f (str2f, List);
      free (List);
      break;
    case COLOR:
      string* List = Split (",", _(V));
      Style->color = Map4f (str2f, List);
      free (List);
      break;
    case BEFORE: Style->before = Value; break;
  }
  free (F);
  free (V);
}

#endif