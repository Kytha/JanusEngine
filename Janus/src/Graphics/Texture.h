#pragma once

#include <string>

#include "Core/Buffer.h"
#include "Core/stb_image/stb_image.h"

namespace Janus
{

    enum class TextureFormat
	{
		None = 0,
		RGB = 1,
		RGBA = 2,
		Float16 = 3
	};

	class Texture : public RefCounted
	{
		public:
			virtual ~Texture() {};
			virtual void Bind(uint32_t slot = 0) = 0;
		public:
			static uint32_t Texture::CalculateMipMapCount(uint32_t width, uint32_t height);
	};
    class Texture2D : public Texture
    {
    public:
        Texture2D(const std::string &filePath);
		~Texture2D() override;
        virtual void Bind(uint32_t slot = 0) override;
        bool Loaded() const;
        uint32_t m_Width, m_Height;
        Buffer m_ImageData;
        std::string m_FilePath;
        uint32_t m_RendererID;
        bool m_Loaded = false;
    };

	class TextureCube : public Texture
	{
	public:
		TextureCube(TextureFormat format, uint32_t width, uint32_t height);
		TextureCube(TextureFormat format, uint32_t width, uint32_t height, void* data);
		TextureCube(const std::string& path);
		~TextureCube() override;
		virtual void Bind(uint32_t slot = 0) override;
		TextureFormat GetFormat() const { return m_Format; }
		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }
		// This function currently returns the expected number of mips based on image size,
		// not present mips in data
		uint32_t GetMipLevelCount() const ;

		const std::string& GetPath() const  { return m_FilePath; }

		uint32_t GetRendererID() const  { return m_RendererID; }

		bool operator==(const Texture& other) const 
		{
			return m_RendererID == ((TextureCube&)other).m_RendererID;
		}
		uint32_t m_RendererID;
		
	private:
		TextureFormat m_Format;
		uint32_t m_Width, m_Height;

		unsigned char* m_ImageData;
		Buffer m_LocalStorage;
		std::string m_FilePath;
	};
}