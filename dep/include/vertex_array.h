#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include <glad/glad.h>

#include <utility>

class VertexArray {
public:
	VertexArray();

	~VertexArray() { Release(); }

	VertexArray(const VertexArray&) = delete;
	VertexArray& operator=(const VertexArray&) = delete;

	VertexArray(VertexArray&& other) noexcept;
	VertexArray& operator=(VertexArray&& other) noexcept;

	void Bind() const;
	void Unbind() const;

private:
	GLuint m_vertexArrayID = 0;
	void Release() noexcept {
		glDeleteVertexArrays(1, &m_vertexArrayID);
		m_vertexArrayID = 0;
	}
};

#endif