//
// xorlist.h
//
// Windows kernel implementation of an xor chain list.
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

// Example to enumerate the list from head to tail.
//
// EnumerateFromHeadXList(PXLIST_HEADER List) {
//		PXLIST_ENTRY Current = List->Head;
//		PXLIST_ENTRY Previous = NULL;
//		PXLIST_ENTRY Next = NULL;
//		while (Current != NULL) {
//			KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL,
//				"Item pointer: %p\n", Current));
//			Next = _xor_(Previous, Current->Pointer);
//			Previous = Current;
//			Current = Next;
//		}
//	}

// Example to enumerate the list from tail to head.
//
// EnumerateFromTailXList(PXLIST_HEADER List) {
//		PXLIST_ENTRY Current = List->Tail;
//		PXLIST_ENTRY Previous = NULL;
//		PXLIST_ENTRY Next = NULL;
//		while (Current != NULL) {
//			KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL,
//				"Item pointer: %p\n", Current));
//			Previous = _xor_(Next, Current->Pointer);
//			Next = Current;
//			Current = Previous;
//		}
//	}

#if !defined (_XLIST_H)
#define _XLIST_H

#include <ntddk.h>

typedef struct _XLIST_ENTRY {
	struct _XLIST_ENTRY *Pointer;
} XLIST_ENTRY, *PXLIST_ENTRY;

typedef struct _XLIST_HEADER {
	PXLIST_ENTRY Head;
	PXLIST_ENTRY Tail;
} XLIST_HEADER, *PXLIST_HEADER;

FORCEINLINE PXLIST_ENTRY _xor_(PXLIST_ENTRY Back,
	PXLIST_ENTRY Forward) {
	return (PXLIST_ENTRY)((ULONG_PTR)Back ^ (ULONG_PTR)Forward);
}

FORCEINLINE VOID InitializeXListHeader(PXLIST_HEADER List) {
	List->Head = List->Tail = NULL;
	return;
}

PXLIST_ENTRY InsertTailXorList(PXLIST_HEADER List, PXLIST_ENTRY Entry);
PXLIST_ENTRY InsertHeadXorList(PXLIST_HEADER List, PXLIST_ENTRY Entry);
PXLIST_ENTRY RemoveHeadXorList(PXLIST_HEADER List);
PXLIST_ENTRY RemoveTailXorList(PXLIST_HEADER List);

PXLIST_ENTRY InterlockedInsertTailXorList(PXLIST_HEADER List,
	PXLIST_ENTRY Entry, PKSPIN_LOCK Lock);

PXLIST_ENTRY InterlockedInsertHeadXorList(PXLIST_HEADER List,
	PXLIST_ENTRY Entry, PKSPIN_LOCK Lock);

PXLIST_ENTRY InterlockedRemoveHeadXorList(PXLIST_HEADER List,
	PKSPIN_LOCK Lock);

PXLIST_ENTRY InterlockedRemoveTailXorList(PXLIST_HEADER List,
	PKSPIN_LOCK Lock);

#endif