
// Blocks
// ------
#define MATCH(line, testFunc, query, outputter)	\
if (testFunc(query, line)) {	\
outputter	\
} else {	\
}

#define MATCH2(line, testFunc, query, outputter)	\
substr _1 = Line(Text);	\
substr _2 = Line(Text);	\
MATCH(line, testFunc, query, outputter)

#define LINE(line, styleClass)	\
ndcAdd(Pos, Doc);	\
ndcStyle(styleClass, Pos, Doc);	\
ndcInsert(ndcParseLine(line, Doc), Pos, Doc);	\

//	Ln	Test-func		Match		Outputter		Style	
//	--	---------		-----		---------		-----	
MATCH2(	_2,	HasOnly	,	"="	,	LINE( _1	,	h1	)	)
MATCH2(	_2,	HasOnly	,	"-"	,	LINE( _1	,	h2	)	)
MATCH2(	_2,	HasOnly	,	"-| "	,	COLS( _1, " | "	,	th	)	)
MATCH1(	_1,	Has	,	" | "	,	COLS( _1, " | "	,	tr	)	)
MATCH1(	_1,	StartsWith	,	"- "	,	SKIP( _1, 2	,	li	)	)
MATCH3(	_2,	HasOnly	,	"- "	,	LINE( ""	,	hr	)	)
MATCH1(	_1,	Any	,	""	,	LINE( _1	,	p	)	)
/*
match.2:
	2.hasOnly:	=
	line:	1

match.1:
	1.has:	" | "
	cols:	" | ", line.1
*/

// Spans
// -----
QUOTE( "_",	"_",	i,	i	)
QUOTE( "**",	"**",	b,	b	)
QUOTE( "[",	"]",	p,	i	)

KEYWD( "Parameters:",	b	)
KEYWD( "Result:",	b	)
KEYWD( "Case:",	b	)
KEYWD( "Match:",	b	)
KEYWD( "Return",	b	)
/*
quote:
	l: _
	r: _
	q: i
	t: i

quote:
	l: [
	r: ]
	wrap: p
	text: i

keyword:
	"Parameters:": b
*/

// Styles
// ------
#define STYLE(name, additions)	\
style name;	\
{	\
  style styleNew = styleDefault;	\
  additions	\
  name = styleNew;	\
}
#define SET(field, value)	styleNew.field = value;

STYLE( h1, SET(size, 27.5	) SET(bold, true)	)
STYLE( h2, SET(size, 16.5	)	)
STYLE( li, SET(before, "• "	)	)
STYLE( hr, SET(width, 1	)	)
/*
h1:
	size: 27.5
	bold:

h2:
	size: 16.5

li:
	before: "• "

hr:
	width: 1

b:
	bold:

i:
	italic:
*/

// Actions
// -------
#define ACTIONS(name, additions)	\
actions actions_##name;	\
{	\
  actions actionsNew = actionsDefault;	\
  additions	\
  name = actionsNew;	\
}
#define ON(trigger, fn) actionNew.trigger = fn;

ACTIONS( h1, ON(click, functionName) )
/*
h1:
	click: fnName

li:
	click: fnName
*/