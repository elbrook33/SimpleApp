#ifndef LISTS_H
#define LISTS_H

#define LIST(type)                                        \
typedef struct list_##type {                              \
  type * _;                                               \
  int length;                                             \
  int size;                                               \
} list_##type;                                            \
                                                          \
void List_##type (                                        \
  list_##type * wrapper, type * array, int n ) {          \
  wrapper->_ = array;                                     \
  wrapper->length = 0;                                    \
  wrapper->size = n;                                      \
}                                                         \
                                                          \
bool Add_##type (                                         \
  type Add, list_##type * List ) {                        \
  if (List->length >= List->size) {                       \
    return false;                                         \
  }                                                       \
  List->_[List->length] = Add;                            \
  List->length++;                                         \
  return true;                                            \
}                                                         \
                                                          \
bool Insert_##type (                                      \
  int Index, type Insert, list_##type * List ) {          \
}                                                         \
                                                          \
void Remove_##type (                                      \
  int Index, list_##type * List ) {                       \
}                                                         \
                                                          \
bool Append_##type (                                      \
  list_##type Add, list_##type * List ) {                 \
}

#endif