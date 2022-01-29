#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const void* indices, unsigned int size){
	glGenBuffers(1, &rendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);

	count = size;
}

void IndexBuffer::bind() const{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
}

void IndexBuffer::unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//IndexBuffer::~IndexBuffer() {
//	glDeleteBuffers(1, &rendererID);
//}