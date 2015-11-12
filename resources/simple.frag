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
smooth in vec4 unTransPos;
in vec3 camPosition;

out vec4 fragColor;

void main()
{
	vec3 newPos = unTransPos.xyz;
	vec3 dis = newPos - camPosition;
	float scale = clamp(inversesqrt(dot(dis, dis)), 0, 1);
    fragColor = scale * texture(texSampler, texMapping);
}
