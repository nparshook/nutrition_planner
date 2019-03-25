#ifndef NPCORE_GLOBAL_H
#define NPCORE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(NPCORE_LIBRARY)
#  define NPCORESHARED_EXPORT Q_DECL_EXPORT
#else
#  define NPCORESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // NPCORE_GLOBAL_H
