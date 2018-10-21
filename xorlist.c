//
// xorlist.c
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

#include "xorlist.h"

PXLIST_ENTRY InsertTailXList(PXLIST_HEADER List, PXLIST_ENTRY Entry) {
	if (List->Head == NULL) {
		Entry->Pointer = NULL;
		List->Head = Entry;
	}
	else {
		Entry->Pointer = _xor_(NULL, List->Tail);
		List->Tail->Pointer = _xor_(Entry, _xor_(List->Tail->Pointer, NULL));
	}
	List->Tail = Entry;
	return Entry;
}

PXLIST_ENTRY InsertHeadXorList(PXLIST_HEADER List, PXLIST_ENTRY Entry) {
	if (List->Head == NULL) {
		Entry->Pointer = NULL;
		List->Tail = Entry;
	}
	else {
		Entry->Pointer = _xor_(List->Head, NULL);
		List->Head->Pointer = _xor_(Entry, _xor_(NULL, List->Head->Pointer));
	}
	List->Head = Entry;
	return Entry;
}

PXLIST_ENTRY RemoveHeadXorList(PXLIST_HEADER List) {
	PXLIST_ENTRY Entry = List->Head;
	if (Entry != NULL) {
		PXLIST_ENTRY Next = _xor_(NULL, Entry->Pointer);
		if (Next == NULL) {
			List->Tail = NULL;
		}
		else {
			Next->Pointer = _xor_(Entry, _xor_(NULL, Next->Pointer));
		}
		List->Head = Next;
	}
	return Entry;
}

PXLIST_ENTRY RemoveTailXorList(PXLIST_HEADER List) {
	PXLIST_ENTRY Entry = List->Tail;
	if (Entry != NULL) {
		PXLIST_ENTRY Prev = _xor_(Entry->Pointer, NULL);
		if (Prev == NULL) {
			List->Head = NULL;
		}
		else {
			Prev->Pointer = _xor_(Entry, _xor_(Prev->Pointer, NULL));
		}
		List->Tail = Prev;
	}
	return Entry;
}

PXLIST_ENTRY InterlockedInsertTailXorList(PXLIST_HEADER List,
	PXLIST_ENTRY Entry, PKSPIN_LOCK Lock) {
	KIRQL Irql;
	KeAcquireSpinLock(Lock, &Irql);
	InsertTailXorList(List, Entry);
	KeReleaseSpinLock(Lock, Irql);
	return Entry;
}

PXLIST_ENTRY InterlockedInsertHeadXorList(PXLIST_HEADER List,
	PXLIST_ENTRY Entry, PKSPIN_LOCK Lock) {
	KIRQL Irql;
	KeAcquireSpinLock(Lock, &Irql);
	InsertHeadXorList(List, Entry);
	KeReleaseSpinLock(Lock, Irql);
	return Entry;
}

PXLIST_ENTRY InterlockedRemoveHeadXorList(PXLIST_HEADER List,
	PKSPIN_LOCK Lock) {
	KIRQL Irql;
	KeAcquireSpinLock(Lock, &Irql);
	PXLIST_ENTRY Entry = RemoveHeadXorList(List);
	KeReleaseSpinLock(Lock, Irql);
	return Entry;
}

PXLIST_ENTRY InterlockedRemoveTailXorList(PXLIST_HEADER List,
	PKSPIN_LOCK Lock) {
	KIRQL Irql;
	KeAcquireSpinLock(Lock, &Irql);
	PXLIST_ENTRY Entry = RemoveTailXorList(List);
	KeReleaseSpinLock(Lock, Irql);
	return Entry;
}
