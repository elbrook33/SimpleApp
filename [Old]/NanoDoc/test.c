#include "NanoDoc.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define DEBUG 1

int TestNum = 1;

void assert (bool Check, const char* Description) {
	printf ("%s (Test %i: %s)\n", Check? "Pass" : "FAIL", TestNum++, Description);
}

int main () {

	doc Doc = ndcMake ();

	// Test: docMake/ndcAdd/ndcToText
	
	pos Pos = {0,OFF,OFF,OFF};
	ndcAdd (Pos, Doc);
	
	Pos.block = 1;
	ndcAdd (Pos, Doc);
	
	Pos.block = 0;
	Pos.col = 0;
	ndcAdd (Pos, Doc);
	ndcAdd (Pos, Doc);
	
	Pos.span = 0;
	ndcAdd (Pos, Doc);
	
	Pos.pos = 0;
	ndcAdd (Pos, Doc);
	
#if DEBUG
	printf ("%s\n", ndcToText(Doc));
#endif
	assert	(strcmp(ndcToText(Doc), "Block.p:\n  Col.p:\n    Span.p: \n  Col.p:\nBlock.p:\n") == 0,
		"docMake/ndcAdd/ndcToText");

	ndcClear (Doc);
	return 0;

}