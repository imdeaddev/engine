#pragma once

#ifdef _WIN32
#ifdef engine_EXPORTS
#define ENGINEAPI __declspec(dllexport)
#else
#define ENGINEAPI __declspec(dllimport)
#endif
#else
#define ENGINEAPI
#endif
