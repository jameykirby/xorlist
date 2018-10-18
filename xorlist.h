//
// xorlist.h
//
// Native NT implementation of an xor chain list.
//
// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non - commercial, and by any
// means.
// 
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
// For more information, please refer to <http://unlicense.org/>
// 

#if !defined (_XOR_LIST_H)
#define _XOR_LIST_H

#include <ntddk.h>

typedef struct _XOR_LIST_ENTRY {
	struct _XOR_LIST_ENTRY *Pointer;
} XOR_LIST_ENTRY, *PXOR_LIST_ENTRY;

typedef struct _XOR_LIST {
	PXOR_LIST_ENTRY Head;
	PXOR_LIST_ENTRY Tail;
} XOR_LIST, *PXOR_LIST;

FORCEINLINE PXOR_LIST_ENTRY _xor_(PXOR_LIST_ENTRY Back,
	PXOR_LIST_ENTRY Forward) {
	return (PXOR_LIST_ENTRY)((ULONG_PTR)Back ^ (ULONG_PTR)Forward);
}

FORCEINLINE VOID InitializeXorListHead(PXOR_LIST List) {
	List->Head = List->Tail = NULL;
	return;
}

PXOR_LIST_ENTRY InsertTailXorList(PXOR_LIST List, PXOR_LIST_ENTRY Entry);
PXOR_LIST_ENTRY RemoveHeadXorList(PXOR_LIST List);

PXOR_LIST_ENTRY ExInterlockedInsertTailXorList(PXOR_LIST List,
	PXOR_LIST_ENTRY Entry, PKSPIN_LOCK Lock);

PXOR_LIST_ENTRY ExInterlockedRemoveHeadXorList(PXOR_LIST List,
	PKSPIN_LOCK Lock);

#endif