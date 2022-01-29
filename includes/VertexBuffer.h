#pragma once
#include <glad/glad.h>


struct VertexBuffer {
	unsigned int rendererID;

	VertexBuffer(const void* data, unsigned int size);

	~VertexBuffer();

	void bind() const;

	void Unbind() const;
};