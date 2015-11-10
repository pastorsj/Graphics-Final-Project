#version 330

uniform mat4 P;
uniform mat4 C;
uniform mat4 mT;
uniform mat4 mR;
uniform mat4 preTrans;
uniform mat4 postTrans;
uniform mat4 M;
uniform mat4 N;
uniform mat4 L;
uniform vec4 lightPos;
uniform vec4 camPos; 
uniform int shadingMode;

in vec3 pos;
in vec2 texCoord;

out vec2 texMapping;

void main()
{    
	vec4 pos = vec4(pos, 1);

	texMapping = texCoord;
	gl_Position = P*C*postTrans*mR*preTrans*pos;

}
