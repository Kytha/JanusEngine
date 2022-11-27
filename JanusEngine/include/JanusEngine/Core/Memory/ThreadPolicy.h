#pragma once

namespace JanusEngine
{
    template <class SynchronizationPrimitive>
    class MultiThreadPolicy
    {
    public:
    inline void Enter(void)
    {
        m_primitive.Enter();
    }
    
    inline void Leave(void)
    {
        m_primitive.Leave();
    }
    
    private:
    SynchronizationPrimitive m_primitive;
    };

    class SingleThreadPolicy
    {
    public:
    inline void Enter(void)
    {
    }
    
    inline void Leave(void)
    {
    }
    };
}    
