//<LICENSE>

#ifndef FDKBACKPROJ_GLOBAL_H
#define FDKBACKPROJ_GLOBAL_H

#ifndef NO_QT
#include <QtCore/qglobal.h>

#if defined(FDKBACKPROJ_LIBRARY)
#  define FDKBACKPROJSHARED_EXPORT Q_DECL_EXPORT
#else
#  define FDKBACKPROJSHARED_EXPORT Q_DECL_IMPORT
#endif
#else
#  define FDKBACKPROJSHARED_EXPORT
#endif

#ifdef __GNUC__
#define UNUSED(x) UNUSED_ ## x __attribute__((__unused__))
#else
#define UNUSED(x) UNUSED_ ## x
#endif

#endif // FDKBACKPROJ_GLOBAL_H
