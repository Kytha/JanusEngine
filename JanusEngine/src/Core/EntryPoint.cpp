#include "Core/Core.h"
#include "Core/EntryPoint.h"
#include "Core/Assertion.h"
namespace JanusEngine
{
    int entryPoint(int argc, const char **argv)
    {
        Log::LogGroupProps coreLogGroupProps = {"CORE", "%^[%T] %n: %v%$"};
        Log::registerLogGroup(coreLogGroupProps);
        JN_ERROR("CORE", "Hello");
        int a = 10;
        int b = 20;
        JN_ASSERT("CORE", a > b, "a ({0}) is not greater then b ({1})", a, b);
        Log::deregisterLogGroup("CORE");
        return 0;
    }
}
