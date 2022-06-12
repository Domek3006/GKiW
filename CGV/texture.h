#ifndef TEXTURE
#define TEXTURE

#include "libraries.h"
#include "stb_image.h"

class Texture {
	private:
		GLuint textureID;
		int width;
		int height;
	public:
		Texture(const char* fileName);
		~Texture();
		void bind();
		void unbind();		
};

#endif
