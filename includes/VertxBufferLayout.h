#pragma once
#include <vector>
#include <glad/glad.h>

struct VertexBufferElement {
	unsigned int type;
	unsigned int count;
	bool normalized;
};


struct VertexBufferLayout {
	std::vector<VertexBufferElement> elements;
	unsigned int stride;

	VertexBufferLayout() :stride(0) {};

	template<typename T>
	void push(unsigned int count) {
		// static_assert(false);
	}

	inline const std::vector<VertexBufferElement>& getElements() const {
		return elements;
	}


	inline unsigned int getStride() const{
		return stride;
	}
};

template <>
inline void VertexBufferLayout::push<float>(unsigned int count)
{
    elements.push_back({GL_FLOAT, count, false});
    stride += sizeof(float) * count;
}

template <>
inline void VertexBufferLayout::push<unsigned int>(unsigned int count)
{
    elements.push_back({GL_UNSIGNED_INT, count, false});
    stride += sizeof(unsigned int) * count;
}

template <>
inline void VertexBufferLayout::push<unsigned char>(unsigned int count)
{
    elements.push_back({GL_UNSIGNED_BYTE, count, true});
    stride += sizeof(unsigned char) * count;
}