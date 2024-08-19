#ifndef LYRICTABGLOBAL_H
#define LYRICTABGLOBAL_H

#ifdef _MSC_VER
#  define LYRIC_TAB_DECL_EXPORT __declspec(dllexport)
#  define LYRIC_TAB_DECL_IMPORT __declspec(dllimport)
#else
#  define LYRIC_TAB_DECL_EXPORT __attribute__((visibility("default")))
#  define LYRIC_TAB_DECL_IMPORT __attribute__((visibility("default")))
#endif

#ifndef LYRIC_TAB_EXPORT
#  ifdef LYRIC_TAB_STATIC
#    define LYRIC_TAB_EXPORT
#  else
#    ifdef LYRIC_TAB_LIBRARY
#      define LYRIC_TAB_EXPORT LYRIC_TAB_DECL_EXPORT
#    else
#      define LYRIC_TAB_EXPORT LYRIC_TAB_DECL_IMPORT
#    endif
#  endif
#endif

#endif //LYRICTABGLOBAL_H
