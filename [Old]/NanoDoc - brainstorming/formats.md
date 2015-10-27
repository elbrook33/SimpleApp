# High-level (Markdown) format

## Layout

EmailApp
========
|	|	|
| [LeftPane]	| [CenterPane]	|
|	|	|

LeftPane
========
* <Messages>

Messages
========
| <Unread>	| <From> | <Date>	|
|	| <Subject>	|

CenterPane
==========
<ActiveMessage>

ActiveMessage
=============
| To:	| <To>	|
| From:	| <From>	|
| Subject:	| <Subject>	|
| Date:	| <Date>	|
| <Body>		|

## Styles

EmailApp
========
* Font: Oxygen
* Size: 4.5mm

LeftPane
========
* Width: 72mm
* Background: gray.10%

Messages
========
* B1.Style: bold
* C1.Align: right

ActiveMessage.A1:A4
=============
* Color: gray

## Actions

Messages
========
* OnSelect:	ActivateMessage
* OnKey.del:	DeleteMessage

# Mid-level (Yaml) format

``
doc.EmailApp:
	layer:
		block.Toolbar:
			col:
				span:
		
		block:
			col.LeftPane:
				span:
			
			col.CenterPane:
				span:
		
		block.Footer:
			col:
				span:
``

# Low-level (C macros) format

``
DOC("EmailApp",
	LAYER("",
		BLOCK("Toolbar",
			COL("",
				SPAN("")
			)
		),
		BLOCK("",
			COL("LeftPane",
				SPAN("")
			),
			COL("CenterPane",
				SPAN("")
			)
		),
		BLOCK("Footer",
			COL("",
				SPAN("")
			)
		)
	)
)
``