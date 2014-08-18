#ifndef OVERRIDE
#define OVERRIDE override
#endif

#ifndef UNREFERENCED_PARAMETER
#ifdef _MSC_VER
#define UNREFERENCED_PARAMETER(P) (P) /* In windows, make it identical to Winnt.h*/
#else
#define UNREFERENCED_PARAMETER(P) (void)(P) /* In gcc/clang, (P) complains about useless statement. (void)(P) doesnt.*/
#endif
#endif

#ifndef FINAL
#define FINAL final
#endif

#ifndef DEPRECATED 
#define DEPRECATED(msg) __declspec(deprecated(msg))
#endif
