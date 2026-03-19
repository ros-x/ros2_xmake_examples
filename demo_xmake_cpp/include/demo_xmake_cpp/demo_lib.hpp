#pragma once

#ifdef _WIN32
#  ifdef DEMO_XMAKE_CPP_BUILDING_DLL
#    define DEMO_XMAKE_CPP_PUBLIC __declspec(dllexport)
#  else
#    define DEMO_XMAKE_CPP_PUBLIC __declspec(dllimport)
#  endif
#else
#  define DEMO_XMAKE_CPP_PUBLIC __attribute__((visibility("default")))
#endif

DEMO_XMAKE_CPP_PUBLIC int add_two(int value);
