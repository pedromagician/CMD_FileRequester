#include "Debug.h"

Debug::Debug()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(...);
#endif
}

Debug::~Debug()
{
	//_CrtDumpMemoryLeaks();
}
