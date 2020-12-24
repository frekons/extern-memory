#include "MemoryManager.h"

#include <tlhelp32.h>

#include <Psapi.h>

memory_manager::memory_manager(uint32_t process_id, DWORD desired_access)
{
	attach(process_id, desired_access);
}

memory_manager::memory_manager(std::string process_name, DWORD desired_access)
{
	attach(process_name, desired_access);
}

memory_manager::~memory_manager()
{
	CloseHandle(this->handle);
}

bool memory_manager::attach(std::string process_name, DWORD desired_access)
{
	uint32_t process_id = -1;

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	PROCESSENTRY32 process_entry;

	ZeroMemory(&process_entry, sizeof(process_entry));

	process_entry.dwSize = sizeof(process_entry);

	if (Process32First(snapshot, &process_entry))
	{
		do
		{
			if (std::string(process_entry.szExeFile) == process_name)
			{
				process_id = process_entry.th32ProcessID;
				break;
			}

		} while (Process32Next(snapshot, &process_entry));
	}

	CloseHandle(snapshot);

	if (process_id != -1) // we have found the process
	{
		this->process_id = process_id;

		this->handle = OpenProcess(desired_access, true, this->process_id);

		return true;
	}

	return false;
}

bool memory_manager::attach(uint32_t process_id, DWORD desired_access)
{
	this->process_id = process_id;

	this->handle = OpenProcess(desired_access, true, this->process_id);

	if (this->handle != INVALID_HANDLE_VALUE)
	{
		return true;
	}

	this->process_id = -1;

	return false;
}

bool memory_manager::is_attached()
{
	return handle != INVALID_HANDLE_VALUE;
}

module_t memory_manager::get_module(std::string module_name)
{
	if (this->handle == INVALID_HANDLE_VALUE)
		return {};

	HANDLE module = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, this->process_id);

	MODULEENTRY32 entry;

	entry.dwSize = sizeof(entry);

	do
	{
		if (strcmp(entry.szModule, (LPSTR)module_name.c_str()) == 0) // we have found the module that we look for
		{
			CloseHandle(module);

			return { (uintptr_t)entry.hModule, entry.modBaseSize };
		}

	} while (Module32Next(module, &entry));

	return {};
}

bool memory_manager::read(uintptr_t address, void* buffer, size_t size)
{
	size_t bytes_read = 0;

	ReadProcessMemory(this->handle, (LPCVOID)address, buffer, size, &bytes_read);

	return bytes_read > 0;
}

inline module_t memory_manager::operator()(std::string module_name) 
{
	return get_module(module_name);
}


