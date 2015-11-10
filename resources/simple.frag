#version 330

uniform mat4 P;
uniform mat4 C;
uniform mat4 mT;
uniform mat4 mR;
uniform mat4 L;
uniform vec4 lightPos;
uniform vec4 camPos;
uniform int shadingMode;
uniform sampler2D texSampler;

in vec2 texMapping;

out vec4 fragColor;

void main()
{
    fragColor = texture(texSampler, texMapping);
}
