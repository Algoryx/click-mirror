#pragma once
#if defined(_MSC_VER) && defined(BUILD_SHARED_LIBS)
  #if defined(click_EXPORTS)
#define CLICK_EXPORT __declspec(dllexport)
  #else
#define CLICK_EXPORT __declspec(dllimport)
  #endif
#else
#define CLICK_EXPORT
#endif
