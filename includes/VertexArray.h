#pragma once
#include <VertexBuffer.h>
#include <VertxBufferLayout.h>

struct VertexArray {
	unsigned int rendererID;

	VertexArray();

	void bind() const;

	~VertexArray();

	void addBuffer(const VertexBuffer& vb, const VertexBufferLayout &layout);
};