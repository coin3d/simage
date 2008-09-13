#ifndef SIMAGE_DEBUG
#error The define SIMAGE_DEBUG needs to be defined to true or false
#endif

#ifndef SIMAGE_INTERNAL
#error this is a private header file
#endif

#if SIMAGE_DEBUG
#include "config-debug.h"
#else /* !SIMAGE_DEBUG */
#include "config-release.h"
#endif /* !SIMAGE_DEBUG */
