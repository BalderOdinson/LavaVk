//
// Created by oshikuru on 21. 01. 2020..
//

#ifndef LAVAVK_GLM_INCLUDER_H
#define LAVAVK_GLM_INCLUDER_H

#if defined(__clang__)
// CLANG ENABLE/DISABLE WARNING DEFINITION
#	define LAVAVK_DISABLE_WARNINGS()                             \
		_Pragma("clang diagnostic push")                        \
		    _Pragma("clang diagnostic ignored \"-Wall\"")       \
		        _Pragma("clang diagnostic ignored \"-Wextra\"") \
		            _Pragma("clang diagnostic ignored \"-Wtautological-compare\"")

#	define LAVAVK_ENABLE_WARNINGS() \
		_Pragma("clang diagnostic pop")
#elif defined(__GNUC__) || defined(__GNUG__)
// GCC ENABLE/DISABLE WARNING DEFINITION
#	define LAVAVK_DISABLE_WARNINGS()                             \
		_Pragma("GCC diagnostic push")                          \
		    _Pragma("GCC diagnostic ignored \"-Wall\"")         \
		        _Pragma("clang diagnostic ignored \"-Wextra\"") \
		            _Pragma("clang diagnostic ignored \"-Wtautological-compare\"")

#	define LAVAVK_ENABLE_WARNINGS() \
		_Pragma("GCC diagnostic pop")
#elif defined(_MSC_VER)
// MSVC ENABLE/DISABLE WARNING DEFINITION
#	define LAVAVK_DISABLE_WARNINGS() \
		__pragma(warning(push, 0))

#	define LAVAVK_ENABLE_WARNINGS() \
		__pragma(warning(pop))
#endif

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#ifndef GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#endif
#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif
#ifndef GLM_GTX_random
#define GLM_GTX_random
#endif

#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/random.hpp>

#endif //LAVAVK_GLM_INCLUDER_H
