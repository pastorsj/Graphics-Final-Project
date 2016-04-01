#version 330

uniform vec2 resolution;          //view resolution in pixels
uniform float animationTime;

in vec3 pos;

smooth out vec4 smoothColor;

void main()
{
	gl_Position = vec4(pos,1);
	smoothColor = vec4(1,1,1,1);
}
