
#ifndef DT_EXPORT_H
#define DT_EXPORT_H

#ifdef DT_STATIC_DEFINE
#  define DT_EXPORT
#  define DT_NO_EXPORT
#else
#  ifndef DT_EXPORT
#    ifdef deptrum_EXPORTS
        /* We are building this library */
#      define DT_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define DT_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef DT_NO_EXPORT
#    define DT_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef DT_DEPRECATED
#  define DT_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef DT_DEPRECATED_EXPORT
#  define DT_DEPRECATED_EXPORT DT_EXPORT DT_DEPRECATED
#endif

#ifndef DT_DEPRECATED_NO_EXPORT
#  define DT_DEPRECATED_NO_EXPORT DT_NO_EXPORT DT_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef DT_NO_DEPRECATED
#    define DT_NO_DEPRECATED
#  endif
#endif

#endif /* DT_EXPORT_H */
