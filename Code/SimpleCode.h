
void parse ( const char* Text )
{
  dict H_file = newDict ();
  dict C_file = newDict ();

  parsePreamble ( &Text, &H_file, &C_file );
  parseBlocks   ( &Text, &H_file, &C_file );
}

void parsePreamble ( const char** Text, dict* H_file, dict* C_file )
{
  skipEmptyLines ( Text );
  if ( matchLine ( Text, "```" ) )
  {
    text Preamble = skipToLine ( "```" ); check(Preamble);
    addText ( H_file, "Preamble", Preamble );
    freeFat ( Preamble );
  }
}

void parseBlocks ( const char** Text, dict* H_file, dict* C_file )
{
  do {
    skipEmptyLines ( Text );
  } while (
       parseCommentBlock ( Text, H_file, C_file )
    || parseCBlock ( Text, H_file, C_file )
    || parseFunction ( Text, H_file, C_file )
    || parseStruct ( Text, H_file, C_file )
  ) {}
}

bool parseCommentBlock ( const char** Text, dict* H_file, dict* C_file )
{
  if ( startsWith ( Text, "_" ) )
  {
    text Comment = skipTo_Ending ( Text, "_" ); check(Comment);
    addText ( H_file, "Comment", Comment );
    freeFat ( Comment );
    skipEmptyLines ( Text );
    return true;
  }
  return false;
}

bool parseCBlock ( const char** Text, dict* H_file, dict* C_file )
{
  if ( matchLine ( Text, "```" ) )
  {
    text C_block = skipToLine ( "```" ); check(C_block);
    addText ( H_file, "C-block", C_block );
    freeFat ( C_block );
    return true;
  }
  return false;
}

bool parseFunction ( const char** Text, dict* H_file, dict* C_file )
{
  if ( startsWithType ( Text, WordChar )
    && nextLineHasOnly( Text, "=" ) )
  {
    return parseFunctionDeclaration ( Text, H_file, C_file )
        && parseFunctionBody ( Text, H_file, C_file );
  }
  return false;
}

bool parseStruct ( const char** Text, dict* H_file, dict* C_file )
{
  if ( startsWith     ( Text, "[" )
    && endsWith       ( Text, "]" )
    && nextLineHasOnly( Text, "=" ) )
  {
    return parseStructDeclaration ( Text, H_file, C_file )
        && parseStructBody ( Text, H_file, C_file );
  }
  return false;
}

bool parseStructDeclaration ( const char** Text, dict* H_file, dict* C_file )
{
  "typedef struct "
}
