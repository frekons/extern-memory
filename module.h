#pragma once

#include <cstdint>

class module_t
{
//private:
public:
	uintptr_t base_address;

	size_t size;

public:
	module_t() : base_address{ 0 }, size{ 0 } { }

	module_t(uintptr_t base_address, size_t size) : base_address{ base_address }, size{ size } { }

	bool is_valid()
	{
		return base_address != 0 && size != 0;
	}
};