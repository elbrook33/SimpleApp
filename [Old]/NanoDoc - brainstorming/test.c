#include "file.h"
#include "parsing.h"
#include "style.h"
#include "simplestring.h"
#include <stdio.h>
#include <stdbool.h>

static int TestNum = 1;

void Test (const char* Description, bool Passed) {
  printf ("  %s | Test #%i: %s\n",
          Passed? "Pass" : "FAIL",
          TestNum++,
          Description
         );
}

int main() {
  // String - Count
  int N = Count ("te", _("testes"));
  Test ("string/Count",
        N == 2
       );
  
  // String - FwdTo
  string T = _("  Hello");
  SkipSpaces (&T);
  string Get = FwdTo ("el", &T);
  Test ("string/FwdTo",
        strcmp (Copy(Get), "H") == 0 &&
        strcmp (Copy(T),  "lo") == 0
       );
  
  // Style - MakeStyle, AddStyleProperty
  style S = MakeStyle (_("default"));
  AddStyleProperty (_("color"), _("0.1, 0.2, 0.3, 1.0"), &S);
  Test ("style/Make/Add",
        strcmp (Copy(S.name), "default") == 0 &&
        S.color.b == 0.3f
       );
  
  // Styles, ParseStyles
  const char* Text = "\nStyle:\n  color: 0.9, 0.8, 0.7, 0.6\n"
                     "\nStyle \"h1\":\n  font: Arial\n  line: 20\n";
  list_style Styles = ParseStyles (Text);
  Test ("styles/ParseStyles",
        strcmp (Copy(Styles._[1].name), "h1") == 0 &&
        strcmp (Copy(Styles._[1].font), "Arial") == 0 &&
        Styles._[1].line == 20.f
       );

  // ReadFile
  const char* File = ReadFile ("test.txt");
  Test ("file/ReadFile",
        strcmp (File, "Hello!") == 0
       );
  
  // Parse file
  const char* Yaml = ReadFile ("style-format.yml");
  list_style LongStyles = ParseStyles (Yaml);
  Test ("styles/Parse file",
        strcmp (Copy(LongStyles._[2].name), "i") == 0 &&
        strcmp (Copy(LongStyles._[2].font), "Serif") == 0 &&
        LongStyles._[2].italic == true &&
        strcmp (Copy(LongStyles._[5].before), "•") == 0
       );
  
  return 0;
}