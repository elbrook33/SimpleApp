/*
 * Do:
 * - Styles
 * - Draw
 *
 * Later:
 * - Option to nest docs in cols
 *   union node = span/doc, bool nested and #define ELEM_EXTRA
 * - Actions
 * - Yaml
 * - Display compact lists
 */

#ifndef NANODOC_H
#define NANODOC_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
typedef int NVGcontext;

/*
 * Part 1:
 * Primitives
 */

// Geometries

typedef struct point {
	int x, y;
} point;

typedef struct rect {
	int x, y, w, h;
} rect;

const rect rectDefault = {0,0,0,0};

typedef struct pos {
	int block, col, span, pos;
} pos;

// Array bounds

int Inbounds (int i, int l) {
	if (i <  0) i += l;
	if (i >= l) i  = l - 1;
	if (i <  0) i  = 0;
	return i;
}

// substr (for parsing)

typedef struct substr {
	const char* _;
	int length;
} substr;

// File

const char* fileRead (const char* Filename) {
}

bool fileWrite (const char* Filename, const char* Text) {
}

/*
 * Part 2:
 * Styling
 */

// Styling

typedef struct rgba {
	float r, g, b, a;
} rgba;

typedef struct style {
	const char	*name, *font, *before;
	float size, line, top, bottom, left, right, w, h;
	bool bold, italic, editable;
	int align;
	rgba color, bg, border;
} style;

#define LEFT -1
#define CENTER 0
#define RIGHT 1

const style styleDefault = {
	"p", "Serif", "",
	11, 15, 0, 0, 0, 0, 0, 0,
	false, false, true,
	LEFT,
	{0,0,0,1}, {1,1,1,1}, {0,0,0,0}
};

/*
 * Part 3:
 * Doc data structures
 */

// LIST generic type

#define CHUNK 64

#define LIST(TYPE) 	\
	\
typedef struct list_##TYPE { 	\
  TYPE* array;	\
  int length, size;	\
} list_##TYPE;	\
	\
list_##TYPE list_##TYPE##_Make () {	\
  list_##TYPE List;	\
  List.array  = malloc (CHUNK * sizeof(TYPE));	\
  List.length = 0;	\
  List.size   = CHUNK;	\
  return List;	\
}	\
	\
void list_##TYPE##_Expand (list_##TYPE* List) {	\
  List->array = realloc (List->array, (List->size + CHUNK) * sizeof(TYPE));	\
  List->size += CHUNK;	\
}	\
	\
void list_##TYPE##_Paste (list_##TYPE Source, int At, list_##TYPE* List) {	\
  while (At + Source.length > List->size) {	\
    list_##TYPE##_Expand (List);	\
  }	\
  for (int i = 0; i < Source.length; i++) {	\
    List->array[At+i] = Source.array[i];	\
  }	\
}	\
	\
void list_##TYPE##_Shift (int From, int By, list_##TYPE* List) {	\
  list_##TYPE Tail;	\
  Tail.array  = List->array + From;	\
  Tail.length = List->length - From;	\
  Tail.size   = List->size - From;	\
  list_##TYPE##_Paste (Tail, From + By, List);	\
  List->length += By;	\
}	\
	\
void list_##TYPE##_Add (TYPE Add, int Index, list_##TYPE* List) {	\
  int i = Inbounds (Index, List->length + 1);	\
  list_##TYPE##_Shift (i, 1, List);	\
  List->array[i] = Add;	\
}	\
	\
void list_##TYPE##_Append (list_##TYPE Add, list_##TYPE* To) {	\
  list_##TYPE##_Paste (Add, To->length, To);	\
  To->length += Add.length;	\
}	\
	\
void list_##TYPE##_Clear (list_##TYPE* List) {	\
  free (List->array);	\
}

// ELEM generic type

#define ELEM(TYPE, SUBTYPE)	\
	\
typedef struct TYPE {	\
  list_##SUBTYPE* SUBTYPE##s;	\
  style* style;	\
  rect* bounds;	\
} TYPE;	\
	\
TYPE TYPE##Make (const char* Style, style Parent) {	\
  TYPE Box;	\
  Box.SUBTYPE##s = malloc (sizeof (list_##SUBTYPE));	\
  Box.style      = malloc (sizeof (style));	\
  Box.bounds     = malloc (sizeof (rect));	\
 *Box.SUBTYPE##s = list_##SUBTYPE##_Make ();	\
 *Box.style      = Parent;	\
 *Box.bounds     = rectDefault;	\
  return Box;	\
}	\
	\
void TYPE##Clear (TYPE Elem) {	\
  list_##SUBTYPE##_Clear (Elem.SUBTYPE##s);	\
  free (Elem.SUBTYPE##s);	\
  free (Elem.style);	\
  free (Elem.bounds);	\
}

LIST(char)
ELEM(span, char)
LIST(span)
ELEM(col, span)
LIST(col)
ELEM(block, col)
LIST(block)
ELEM(doc, block)

/*
 * Part 4:
 * Functions (API)
 */

// Getting

block ndcBlock (pos Pos, doc Doc) {
	int i = Inbounds (Pos.block, Doc.blocks->length);
	return Doc.blocks->array[i];
}

col ndcCol (pos Pos, doc Doc) {
	int i = Inbounds (Pos.col, ndcBlock (Pos, Doc).cols->length);
	return ndcBlock (Pos, Doc).cols->array[i];
}

span ndcSpan (pos Pos, doc Doc) {
	int i = Inbounds (Pos.span, ndcCol (Pos, Doc).spans->length);
	return ndcCol (Pos, Doc).spans->array[i];
}

char ndcChar (pos Pos, doc Doc) {
	int i = Inbounds (Pos.pos, ndcSpan (Pos, Doc).chars->length);
	return ndcSpan (Pos, Doc).chars->array[i];
}

// Making

doc ndcMake () {
	return docMake (styleDefault);
}

void ndcInsert (const char* Add, pos Pos, doc Doc) {
}

void ndcDelete (int NumChars, pos Pos, doc Doc) {
// Pass 0 to NumChars to remove node
}

void ndcStyle (const char* StyleClass, pos Pos, doc Doc) {
}

# define OFF -1000

void ndcAdd (pos Pos, doc Doc) {
// E.g. {block 2, col 1, span OFF, pos OFF} makes a new col.
	if (Pos.col == OFF) {
		list_block_Add (blockMake(styleDefault), Pos.block, Doc.blocks);
		
	} else if (Pos.span == OFF) {
		block Parent = ndcBlock(Pos, Doc);
		list_col_Add (colMake(*Parent.style), Pos.col, Parent.cols);
		
	} else if (Pos.pos == OFF) {
		col Parent = ndcCol(Pos, Doc);
		list_span_Add (spanMake(*Parent.style), Pos.span, Parent.spans);
	}
}

// Parsing

doc ndcParse (const char* Text) {
	doc Doc = docMake(styleDefault);
	return Doc;
}

doc ndcRead (const char* File) {
	return ndcParse (fileRead (File));
}

// Storing

list_char _ls (const char* Text) {
	list_char List	= list_char_Make();
	List.array	= Text;
	List.length	= strlen (Text);
	return List;
}

const char* ndcToText (doc Doc) {
	list_char Text = list_char_Make();
	for (int a = 0; a < Doc.blocks->length; a++) {
		list_char_Append (_ls("Block."), &Text);
		list_char_Append (_ls(Doc.style->name), &Text);
		list_char_Append (_ls(":\n"), &Text);
		block Block = Doc.blocks->array[a];
	for (int b = 0; b < Block.cols->length; b++) {
		list_char_Append (_ls("  Col."), &Text);
		list_char_Append (_ls(Block.style->name), &Text);
		list_char_Append (_ls(":\n"), &Text);
		col Col = Doc.blocks->array[a].cols->array[b];
	for (int c = 0; c < Col.spans->length; c++) {
		list_char_Append (_ls("    Span."), &Text);
		list_char_Append (_ls(Col.style->name), &Text);
		list_char_Append (_ls(": "), &Text);
		list_char_Append (*Doc.blocks->array[a].cols->array[b].spans->array[c].chars, &Text);
		list_char_Append (_ls("\n"), &Text);
	}}}
	return Text.array;
}

bool ndcWrite (const char* File, doc Doc) {
	return fileWrite (File, ndcToText (Doc));
}

// Drawing

rect ndcDrawCol (col Col, rect Bounds, NVGContext* Context) {
}

rect ndcDrawBlock (block Block, rect Bounds, NVGContext* Context) {
	*Block.bounds = {0,0,0,0};
}

rect ndcDrawDoc (doc Doc, rect Bounds, NVcontext* Context) {
	for (int a = 0; a < Blocks->length; a++) {
		block Block = Blocks->array[a];
		ndcDrawBlock (Block, Bounds, Context);
	}
}

void ndcDraw (doc Doc, NVGcontext* Context) {
 *Doc.bounds = ndcDrawDoc(Doc, {0,0,0,0}, Context);
}

// Events

pos ndcPosAt (point XY, doc Doc) {
}

list_char ndcCopy (int NumChars, pos Pos, doc Doc) {
}

// Viewer

int ndcWindow (const char* File) {
}

void ndcClose (int WindowID) {
}

void ndcCloseAll () {
}

// Freeing

void ndcClear (doc Doc) {
	docClear (Doc);
}

#endif
