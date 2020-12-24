#pragma once

#include <Windows.h>

#include <string>

#include "Module.h"

#define throws_exception

class memory_manager
{
protected:
	HANDLE handle = INVALID_HANDLE_VALUE;

	uint32_t process_id;

public:
	memory_manager(uint32_t process_id, DWORD desired_access = PROCESS_ALL_ACCESS);

	memory_manager(std::string process_name, DWORD desired_access = PROCESS_ALL_ACCESS);

	virtual ~memory_manager();

	bool attach(std::string app_name, DWORD desired_access = PROCESS_ALL_ACCESS);

	bool attach(uint32_t process_id, DWORD desired_access = PROCESS_ALL_ACCESS);

	bool is_attached();

	module_t get_module(std::string module_name);

	// functions

	bool read(uintptr_t address, void* buffer, size_t size);

	template <class T> T read(uintptr_t address);

	template <class T> bool write(uintptr_t address, T value);

	// operators

	inline module_t operator()(std::string module_name);

	template<class T> inline T operator()(uintptr_t address);
	
	/*
	 "type" parameter because, i could not find a way to call a operator with template
	 however, if you want to read non-primitive types you should call it like manager.read<T>(address) because in this function there is additional T parameter and
	 it will copy all the bytes of type T to call this function, that's like nothing in primitive types but not in non-primitive types
	*/
	template<class T> T operator()(std::string module_name, uintptr_t address, T type) throws_exception;



	// exceptions

	class InvalidModule : std::exception
	{
	public:
		std::exception::exception;
	};
};



template<class T>
inline T memory_manager::read(uintptr_t address)
{
	T value;

	ReadProcessMemory(this->handle, (LPCVOID)address, &value, sizeof(T), 0);

	return value;
}

template<class T>
inline bool memory_manager::write(uintptr_t address, T value)
{
	size_t numbers_of_bytes_written = 0;

	WriteProcessMemory(this->handle, (LPCVOID)address, &value, sizeof(T), &numbers_of_bytes_written);

	return numbers_of_bytes_written == sizeof(T);
}

template<class T>
inline T memory_manager::operator()(uintptr_t address)
{
	return read<T>(address);
}

template<class T>
inline T memory_manager::operator()(std::string module_name, uintptr_t address, T type)
{
	auto module = get_module(module_name);

	if (!module.is_valid())
		throw InvalidModule();

	return read<T>(module.base_address + address);
}
