#version 330

uniform mat4 P;
uniform mat4 C;
uniform mat4 mT;
uniform mat4 mR;
uniform mat4 preTrans;
uniform mat4 postTrans;
uniform mat4 M;
uniform mat3 N;
uniform mat4 L;
uniform vec4 lightPos;
uniform vec4 camPos; 
uniform int shadingMode;
uniform int immune;

in vec3 pos;
in vec2 texCoord;
in vec3 normal;

out vec2 texMapping;
smooth out vec4 unTransPos;
out vec3 camPosition;
smooth out vec3 smoothNorm;
smooth out vec3 smoothPos;

//smooth out vec3 smoothPos;
//smooth out vec3 smoothNorm;

void main()
{    
	//vec4 pos = vec4(pos, 1);
	//smoothPos = pos;
	//smoothNorm = colorIn*2 - 1 + texCoord.x / 40;

	camPosition = camPos.xyz;
	unTransPos = postTrans*mR*preTrans*vec4(pos, 1);

	texMapping = texCoord;

	/*camPosition = camPosition + normal;
	camPosition = camPosition - normal;*/
	smoothPos = pos;
	smoothNorm = normal;

	gl_Position = P*C*unTransPos;

}