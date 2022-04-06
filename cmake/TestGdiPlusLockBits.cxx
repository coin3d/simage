#include <windows.h>
#if defined(_MSC_VER) && (_MSC_VER == 1200) && !defined(ULONG_PTR)
#define ULONG_PTR ULONG
#endif
#include <gdiplus.h>

int main() 
{
	Gdiplus::Bitmap * bitmap = NULL;
	Gdiplus::BitmapData bitmapData;
	Gdiplus::Rect rect(0, 0, 100, 100);
	Gdiplus::Status result = bitmap->LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bitmapData);
	return 0;
}