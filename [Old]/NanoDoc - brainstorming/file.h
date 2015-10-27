#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdlib.h>

char* ReadFile(char* Filename) {
  char* Buffer = NULL;
  int Size, Read;
  FILE* File = fopen (Filename, "r");
  if (File) {
    fseek (File, 0, SEEK_END);
    Size = ftell (File);
    rewind (File);

    Buffer = malloc ((Size + 1) * sizeof(char));
    Read = fread (Buffer, sizeof(char), Size, File);
    Buffer[Size] = '\0';

    if (Size != Read) {
      free (Buffer);
      Buffer = NULL;
    }
  }
  return Buffer;
}

#endif