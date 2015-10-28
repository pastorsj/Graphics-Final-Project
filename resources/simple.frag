#version 330

uniform mat4 P;
uniform mat4 C;
uniform mat4 mT;
uniform mat4 mR;
uniform mat4 L;
uniform vec4 lightPos;
uniform vec4 camPos;
uniform int shadingMode;

smooth in vec4 smoothColor;
smooth in vec4 surfaceNormal;
smooth in vec4 lightVector;
smooth in vec4 viewVector;

out vec4 fragColor;

void main()
{
    fragColor = smoothColor;
    
    //TODO add gouraud and phong shading support
    if(false) {

        vec4 normal = (surfaceNormal + 1.0f) * 0.5f;
        vec4 ambientIntensity = 0.1 * vec4(1,1,1,1);
        
        //Reversing the scaling operation
        float dot1 = max(dot(lightVector, surfaceNormal), 0.0);
        vec4 diffuseIntensity = normal * dot1 * vec4(1,1,1,1);

        vec4 reflectionVector = normalize(reflect(-lightVector, surfaceNormal));
        float dot2 = max(dot(viewVector, clamp(reflectionVector, 0.0, 1.0)), 0.0);
        vec4 specularIntensity =  vec4(1,1,1,0) * pow(dot2, 10.0);
        
        
 
        fragColor = ambientIntensity + diffuseIntensity + specularIntensity;
    }
}
