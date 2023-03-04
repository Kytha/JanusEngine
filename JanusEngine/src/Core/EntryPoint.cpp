#include "Core/Core.h"
#include "Core/EntryPoint.h"
#include "Core/Assertion.h"
namespace JanusEngine
{
    int entryPoint(int argc, const char **argv)
    {
        Log::registerLogGroup({"CORE", "%^[%T] %n: %v%$"});
        // Engine Loop
        Log::deregisterLogGroup("CORE");
        return 0;
    }
}
