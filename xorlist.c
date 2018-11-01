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

#include <ntddk.h>
#include "xorlist.h"

PXLIST_ENTRY InsertTailXList(PXLIST_HEADER List, PXLIST_ENTRY Entry) {
	if (List->Head == NULL) {
		Entry->Links = NULL;
		List->Head = Entry;
	}
	else {
		Entry->Links = _xor_(NULL, List->Tail);
		List->Tail->Links = _xor_(Entry, _xor_(List->Tail->Links, NULL));
	}
	PXLIST_ENTRY PreviousTail = List->Tail;
	List->Tail = Entry;
	return PreviousTail;
}

PXLIST_ENTRY InsertHeadXList(PXLIST_HEADER List, PXLIST_ENTRY Entry) {
	if (List->Head == NULL) {
		Entry->Links = NULL;
		List->Tail = Entry;
	}
	else {
		Entry->Links = _xor_(List->Head, NULL);
		List->Head->Links = _xor_(Entry, _xor_(NULL, List->Head->Links));
	}
	PXLIST_ENTRY PreviousHead = List->Head;
	List->Head = Entry;
	return PreviousHead;
}

PXLIST_ENTRY RemoveHeadXList(PXLIST_HEADER List) {
	PXLIST_ENTRY Entry = List->Head;
	if (Entry != NULL) {
		PXLIST_ENTRY Next = _xor_(NULL, Entry->Links);
		if (Next == NULL) {
			List->Tail = NULL;
		}
		else {
			Next->Links = _xor_(Entry, _xor_(NULL, Next->Links));
		}
		List->Head = Next;
	}
	return Entry;
}

PXLIST_ENTRY RemoveTailXList(PXLIST_HEADER List) {
	PXLIST_ENTRY Entry = List->Tail;
	if (Entry != NULL) {
		PXLIST_ENTRY Prev = _xor_(Entry->Links, NULL);
		if (Prev == NULL) {
			List->Head = NULL;
		}
		else {
			Prev->Links = _xor_(Entry, _xor_(Prev->Links, NULL));
		}
		List->Tail = Prev;
	}
	return Entry;
}

PXLIST_ENTRY RemoveXList(PXLIST_HEADER List, PXLIST_ENTRY Entry) {
	PXLIST_ENTRY Current = List->Head;
	for (PXLIST_ENTRY Previous = NULL, Next = NULL;
		Current != NULL;
		Next = _xor_(Previous, Current->Links), Previous = Current, Current = Next) {
		if (Current == Entry) {
			if (Previous == NULL) {
				List->Head = Next;
			}
			else {
				Previous->Links = _xor_(Previous->Links, _xor_(Current, Next));
			}
			if (Next == NULL) {
				List->Tail = Previous;
			}
			else {
				Next->Links = _xor_(Next->Links, _xor_(Current, Previous));
			}
			break;
		}
	}
	return Current;
}

PXLIST_ENTRY InterlockedInsertTailXList(PXLIST_HEADER List,
	PXLIST_ENTRY Entry, PKSPIN_LOCK Lock) {
	KLOCK_QUEUE_HANDLE LockHandle;
	KeAcquireInStackQueuedSpinLock(Lock, &LockHandle);
	InsertTailXList(List, Entry);
	KeReleaseInStackQueuedSpinLock(&LockHandle);
	return Entry;
}

PXLIST_ENTRY InterlockedInsertHeadXList(PXLIST_HEADER List,
	PXLIST_ENTRY Entry, PKSPIN_LOCK Lock) {
	KLOCK_QUEUE_HANDLE LockHandle;
	KeAcquireInStackQueuedSpinLock(Lock, &LockHandle);
	InsertHeadXList(List, Entry);
	KeReleaseInStackQueuedSpinLock(&LockHandle);
	return Entry;
}

PXLIST_ENTRY InterlockedRemoveHeadXList(PXLIST_HEADER List,
	PKSPIN_LOCK Lock) {
	KLOCK_QUEUE_HANDLE LockHandle;
	KeAcquireInStackQueuedSpinLock(Lock, &LockHandle);
	PXLIST_ENTRY Entry = RemoveHeadXList(List);
	KeReleaseInStackQueuedSpinLock(&LockHandle);
	return Entry;
}

PXLIST_ENTRY InterlockedRemoveTailXList(PXLIST_HEADER List,
	PKSPIN_LOCK Lock) {
	KLOCK_QUEUE_HANDLE LockHandle;
	KeAcquireInStackQueuedSpinLock(Lock, &LockHandle);
	PXLIST_ENTRY Entry = RemoveTailXList(List);
	KeReleaseInStackQueuedSpinLock(&LockHandle);
	return Entry;
}
