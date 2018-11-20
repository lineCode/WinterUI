#pragma once

#include "definitions.hh"

#include <vector>

struct ConnectionBase
{
	virtual ~ConnectionBase() = default;
};

struct Observer
{
	std::vector<UP<ConnectionBase>> connections;
};
