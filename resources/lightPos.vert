#version 330

uniform mat4 P;
uniform mat4 C;
uniform mat4 mT;
uniform mat4 mR;
uniform mat4 L;
uniform vec4 lightPos;
uniform vec4 camPos;
uniform int shadingMode;

in vec3 pos;
in vec3 colorIn;

smooth out vec4 smoothColor;

void main()
{    
    //hacky easy way to draw the light position
    gl_Position = P*C*L*lightPos;
    smoothColor = vec4(1,1,1,1);
}
