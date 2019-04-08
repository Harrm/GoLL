#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoord;
uniform mat4 world;

out vec2 currentTextureCoord;

void main() {
	gl_Position = world*vec4(position, 1.0);
	currentTextureCoord = textureCoord;
}
