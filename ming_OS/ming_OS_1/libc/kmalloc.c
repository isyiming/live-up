// /*
//  * kmalloc.c -- Heap memory allocation routines
//  * From JamesM's kernel development tutorials.
//  */
// #include "null.h"
// // #include "common.h"
// // #include "paging.h"
// // #include "heap.h"
// #include "kmalloc.h"
//
// // end is defined in the linker script.
// extern uint32_t __end;
// uint32_t placement_address = (uint32_t)&__end;
//
// /* in heap.c */
// extern struct vm_heap *kernel_heap;
// /* in paging.c */
// extern struct vm_page_directory *kernel_directory;
//
// /* internal allocation routine */
// static void *	_kmalloc(size_t len, uint32_t *phys, uint32_t flags);
//
// /* bitmask flags for _kmalloc() */
// #define M_ALIGNED	0x1
// #define M_ZERO		0x2
//
//
// void *
// kmalloc(size_t len)
// {
// 	return (_kmalloc(len, NULL,  0));
// }
//
// void *
// kmalloc0(size_t len)
// {
// 	return (_kmalloc(len, NULL,  M_ZERO));
// }
//
// void *
// kmalloc_a(size_t len)
// {
//
// 	return (_kmalloc(len, NULL, M_ALIGNED));
// }
//
// void *
// kmalloc0_a(size_t len)
// {
//
// 	return (_kmalloc(len, NULL, (M_ALIGNED | M_ZERO)));
// }
//
// void *
// kmalloc_p(size_t len, uint32_t *phys)
// {
//
// 	return (_kmalloc(len, phys, 0));
// }
//
// void *
// kmalloc0_p(size_t len, uint32_t *phys)
// {
//
// 	return (_kmalloc(len, phys, M_ZERO));
// }
//
// void *
// kmalloc_ap(size_t len, uint32_t *phys)
// {
//
// 	return (_kmalloc(len, phys, M_ALIGNED));
// }
//
// void *
// kmalloc0_ap(size_t len, uint32_t *phys)
// {
//
// 	return (_kmalloc(len, phys, M_ALIGNED | M_ZERO));
// }
//
//
// static void *
// _kmalloc(size_t len, uint32_t *phys, uint32_t flags)
// //len开辟内存空间长度
// {
// 	void *addr = NULL;
//
// 	if (kernel_heap == NULL) {
// 		if ((flags & M_ALIGNED) && (placement_address & 0x00000FFF)) {
// 			/* the address is not already aligned */
// 			placement_address = (placement_address & 0xFFFFF000) + 0x1000;
// 		}
// 		if (phys != NULL)
// 			*phys = placement_address;
// 		placement_address += len;
// 		addr = (void *)(placement_address - len);
// 	}
// //	else {
// //		addr = alloc(len, (flags & M_ALIGNED), kernel_heap);
// //		if (phys != NULL) {
// //			struct vm_page *page = get_page((uint32_t)addr, 0, kernel_directory);
// //			*phys = page->p_frame * 0x1000 + ((uint32_t)addr & 0xFFF);
// //		}
// //	}
// 	if (flags & M_ZERO)
// 		bzero(addr, len);
// 	return (addr);
// }
//
//
// void
// kfree(void *ptr)
// {
//
// 	free(ptr, kernel_heap);
// }
