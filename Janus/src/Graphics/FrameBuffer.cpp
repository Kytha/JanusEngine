#include "jnpch.h"

#include "Graphics/Framebuffer.h"
#include "Graphics/OpenGLFramebuffer.h"

namespace Janus
{

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification &spec)
	{
		Ref<Framebuffer> result = nullptr;
		result = Ref<OpenGLFramebuffer>::Create(spec);
		FramebufferPool::GetGlobal()->Add(result);
		return result;
	}

	FramebufferPool *FramebufferPool::s_Instance = new FramebufferPool;

	FramebufferPool::FramebufferPool(uint32_t maxFBs /* = 32 */)
	{
	}

	FramebufferPool::~FramebufferPool()
	{
	}

	std::weak_ptr<Framebuffer> FramebufferPool::AllocateBuffer()
	{
		// m_Pool.push_back();
		return std::weak_ptr<Framebuffer>();
	}

	void FramebufferPool::Add(const Ref<Framebuffer> &framebuffer)
	{
		m_Pool.push_back(framebuffer);
	}

}