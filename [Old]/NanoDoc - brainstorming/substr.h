#ifndef SUBSTR_H
#define SUBSTR_H

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct substr {
  const char *start;
  int length;
} string;

substr _ (const char *Text) {
  string Str;
  Str.start = Text;
  Str.length = strlen (Text);
  return Str;
}

char* Copy (substr Str) {
  char* T = malloc (Str.length * sizeof(char) + 1);
  int i;
  for (i = 0; i < Str.length; i++) {
    T[i] = Str.text[i];
  }
  T[i] = '\0';
  return T;
}

void SetBounds (int* Start, int* End, int Length) {
  if (*End < 0) *End += Length;
  if (*End >= Length) *End = Length - 1;
  if (*Start < 0) *Start += Length;
  if (*Start >= Length || *Start > *End) {
    *Start = Length;
    *End = Length - 1;
  }
}

string MakeSlice (int Start, int End, string Text) {
  SetBounds (&Start, &End, Text.length);
  string Str;
  Str.text = Text.text + Start;
  Str.length = End - Start + 1;
  Str.size = Text.length - Start + 1;
  return Str;
}

void Slice (int Start, int End, string* Str) {
  SetBounds (&Start, &End, Str->length);
  Str->text += Start;
  Str->length = End - Start + 1;
  Str->size -= Start;
}

void Fwd (int Shift, string* Text) {
  Slice (Shift, -1, Text);
}

bool StartsWith (string Find, string Text) {
  if (Find.length > Text.length) return false;
  int i; for (i = 0; i < Find.length; i++) {
    if (Find.text[i] != Text.text[i]) break;
  }
  return i == Find.length;
}

int IndexOf (string Find, string Text) {
  int N = Text.length, i;
  for (i = 0; Text.length >= Find.length; i++) {
    if (StartsWith (Find, Text)) return i;
    Fwd(1, &Text);
  }
  return N;
}

inline int min (int a, int b) { return a < b? a : b; }
inline int max (int a, int b) { return a > b? a : b; }
inline bool between (int low, int hi, int n) { return n >= low && n <= hi; }

string FwdTo (const char* Delim, string* Text) {
  string D = _(Delim);
  int i = max(0, IndexOf(D, *Text)-1);
  string Str = MakeSlice (0, i, *Text);
  Fwd (i + 1 + D.length, Text);
  return Str;
}

int Count (const char* Find, string Text) {
  int N = -1, l;
  do {
    N++;
    l = Text.length;
  } while (between(1, l-1, FwdTo(Find, &Text).length));
  return N;
}

void SkipSpaces (string* Text) {
  while (Text->length > 0) {
    if (Text->text[0] != ' ' && Text->text[0] != '\n') break;
    Slice (1, -1, Text);
  }
}

string* Split (const char* Delim, string Text) {
  int N = Count (Delim, Text) + 1, i;
  string* List = malloc (N * sizeof(string));
  for (i = 0; i < N; i++) {
    List[i] = FwdTo (Delim, &Text);
  }
  return List;
}

typedef struct vec4 {
  float r, g, b, a;
} vec4;

void Set4f (int i, float Value, vec4* Vec) {
  switch (i) {
    case 0: Vec->r = Value; break;
    case 1: Vec->g = Value; break;
    case 2: Vec->b = Value; break;
    case 3: Vec->a = Value; break;
  }
}

vec4 Map4f (float (*F) (const char*), string* List) {
  vec4 V; int i;
  for (i = 0; i < 4; i++) {
    Set4f (i, F(List[i].text), &V);
  }
  return V;
}

#endif