
#pragma once

#ifdef WIN32
#ifdef ACCU_COMM_IMPORTS
#define ACCU_COMM_API __declspec(dllimport)
#else
#ifdef ACCU_COMM_Static_IMPORTS
#define ACCU_COMM_API
#else

#define ACCU_COMM_API __declspec(dllexport)
#endif //ACCU_COMM_STATIC_EXPORTS
#endif

#else
#define ACCU_COMM_API
#endif // ifdef WIN32
