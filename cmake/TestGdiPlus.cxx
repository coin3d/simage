#include <windows.h>
#if defined(_MSC_VER) && (_MSC_VER == 1200) && !defined(ULONG_PTR)
#define ULONG_PTR ULONG
#endif
#include <gdiplus.h>

int main() 
{ 
	return 0; 
}
