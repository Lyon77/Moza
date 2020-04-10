#pragma once

//Common
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

//Data Structures
#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>

#include "Moza/Core/Log.h"
#include "Moza/Core/KeyCodes.h"
#include "Moza/Core/MouseButtonCodes.h"
#include "Moza/Core/Random.h"

#include "Moza/Debug/Instrumentor.h"


#ifdef MZ_PLATFORM_WINDOWS
	#include <Windows.h>
#endif