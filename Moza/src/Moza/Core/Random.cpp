#include "mzpch.h"
#include "Moza/Core/Random.h"

namespace Moza
{
	std::mt19937 Random::s_RandomEngine;
	std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;
}
