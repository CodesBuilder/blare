#ifndef __BLARE_BASE_H__
#define __BLARE_BASE_H__

#if defined(DEBUG) || defined(_DEBUG) || defined(DBG)
#ifdef _WIN32

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

// Memory leak detection
#define malloc(x) _malloc_dbg(x, _NORMAL_BLOCK, __FILE__, __LINE__)
#define free(x) _free_dbg(x, _NORMAL_BLOCK)
#define __DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#define new __DBG_NEW

#endif
#endif

namespace Blare {
	using TokenID = int;
}

#endif
