namespace JanusEngine
{
    class NoBoundsChecking
    {
    public:
    static const size_t SIZE_FRONT = 0;
    static const size_t SIZE_BACK = 0;
    
    inline void GuardFront(void*) const {}
    inline void GuardBack(void*) const {}
    
    inline void CheckFront(const void*) const {}
    inline void CheckBack(const void*) const {}
    };
}