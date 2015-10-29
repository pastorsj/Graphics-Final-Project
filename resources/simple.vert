#version 330

uniform mat4 P;
uniform mat4 C;
uniform mat4 mT;
uniform mat4 mR;
uniform mat4 trans;
uniform mat4 M;
uniform mat4 N;
uniform mat4 L;
uniform vec4 lightPos;
uniform vec4 camPos; 
uniform int shadingMode;

in vec3 pos;
in vec3 colorIn;

smooth out vec4 smoothColor;
smooth out vec4 surfaceNormal;
smooth out vec4 lightVector;
smooth out vec4 viewVector;

vec4 justColor()
{
    return vec4(colorIn, 1);
}

vec4 gouraud()
{
    vec4 normal = (surfaceNormal + 1.0f) * 0.5f;
    vec4 ambientIntensity = 0.1 * vec4(1,1,1,1);
    
    //Reversing the scaling operation
    float dot1 = max(dot(lightVector, surfaceNormal), 0.0);
    vec4 diffuseIntensity = normal * dot1 * vec4(1,1,1,1);
    
    vec4 reflectionVector = normalize(reflect(-lightVector, surfaceNormal));
    float dot2 = max(dot(viewVector, clamp(reflectionVector, 0.0, 1.0)), 0.0);
    vec4 specularIntensity =  vec4(1,1,1,0) * pow(dot2, 10.0);
    
    return ambientIntensity + diffuseIntensity + specularIntensity;
}

vec4 phong()
{
    return justColor();
}

void main()
{
    //TODO add gouraud and phong shading support
    
	vec4 pos = vec4(pos, 1);
	smoothColor = justColor();
    
    //surfaceNormal = C*mR*vec4(colorIn * 2 - vec3(1,1,1), 0);
    //lightVector = normalize(L*lightPos - mR*pos);
    //viewVector = normalize(camPos- mR*pos);
	gl_Position = P*M*trans*pos;
    /*
    if(shadingMode == 0)
        smoothColor = justColor();
    else if (shadingMode == 1)
        smoothColor = gouraud();
    else
        smoothColor = phong();
	*/
}
