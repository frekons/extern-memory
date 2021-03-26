# extern_memory
using WINAPIs to interact with other processes's memories

```
	const char* app_name = "brave.exe", *module_name = "brave.exe";

	extern_memory memory(app_name);

	if (!memory.is_attached())
	{
		std::cout << "failed to attach! error: 0x" << std::hex << GetLastError() << std::dec << std::endl;
		return 0;
	}

	std::cout << "attached to \"" << app_name << "\" successfully, MZ test: \"";

	auto mz_text = memory(module_name, 0, (short)0);

	std::cout << ((char*)(&mz_text))[0] << ((char*)(&mz_text))[1] << "\"" << std::endl;
```
