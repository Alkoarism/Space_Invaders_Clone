#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STBI_FAILURE_USERMSG
#define STBI_ONLYJPEG

#include <iostream>
#include <utility>

#include "texture.h"

Texture::Texture(GLenum target, const char* location) : m_Target(target) {
	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	glTexParameteri(m_Target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(m_Target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(m_Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(m_Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	int imgWidth, imgHeight, nrChannels;
	unsigned char* data
		= stbi_load(location, &imgWidth, &imgHeight, &nrChannels, 0);
	if (data) {
		glTexImage2D(m_Target, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(m_Target);
	}
	else {
		const char* failLog = stbi_failure_reason();
		std::cout << "FAILED_TO_LOAD_TEXTURE\n" << failLog << std::endl;
	}
	stbi_image_free(data);
}

Texture::Texture(Texture&& other) noexcept
	: m_TextureID(other.m_TextureID) {
	other.m_TextureID = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept {
	if (this != &other) {
		Release();
		std::swap(m_TextureID, other.m_TextureID);
	}
	return *this;
}

void Texture::Bind() const {
	glBindTexture(m_Target, m_TextureID);
}

void Texture::Unbind() const {
	glBindTexture(m_Target, 0);
}