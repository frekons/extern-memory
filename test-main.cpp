#include "extern_memory.h"

#include <iostream>

int main() {

	const char* app_name = "brave.exe";

	extern_memory manager(app_name);

	if (!manager.is_attached())
	{
		std::cout << "failed to attach! error: 0x" << std::hex << GetLastError() << std::dec << std::endl;
		return 0;
	}

	std::cout << "attached to \"" << app_name << "\" successfully, MZ test: \"";

	auto mz_text = manager(app_name, 0, (short)0);

	std::cout << ((char*)(&mz_text))[0] << ((char*)(&mz_text))[1] << "\"" << std::endl;

	return 0;
}
