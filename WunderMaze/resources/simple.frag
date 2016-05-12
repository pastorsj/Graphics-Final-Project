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
uniform int immune;

uniform mat4 M;
uniform mat3 N;

uniform vec4 Ka;
uniform vec4 Kd;
uniform vec4 Ks;
uniform float specAlpha;

in vec2 texMapping;
smooth in vec4 unTransPos;
in vec3 camPosition;

smooth in vec3 smoothPos;
smooth in vec3 smoothNorm;

out vec4 fragColor;

vec4 defaultShade(in vec3 pos, in vec3 norm, float scale);

void main()
{
	vec3 newPos = unTransPos.xyz;
	vec3 dis = newPos - camPosition;
	float scale = clamp(pow(inversesqrt(dot(dis, dis)), 2), 0.05, 1);
	if(immune == 1)
	{
		//scale = 1.0f;
		fragColor = texture(texSampler, texMapping);
	}
	else
	{
		fragColor = ((texture(texSampler, texMapping) + defaultShade(unTransPos.xyz, smoothNorm, scale) * scale)) * scale * 2;
	}
}

vec4 defaultShade(in vec3 pos, in vec3 norm, float scale)
{
	float alpha = 1;
	vec4 p = vec4(pos, 1);
	//vec4 lp = lightPos;
	vec4 lp = vec4(camPosition, 0);
	//vec4 n = vec4(1, 1, 1, 0) * normalize(vec4(norm,0));
	vec4 n = vec4(norm, 0);
	vec4 c = vec4(0);
	vec4 Li = vec4(1);
	vec4 ka = vec4(0.005);
	vec4 kd = vec4(0.2, 0.2, 0.8, 0);
	vec4 ks = vec4(1);
	//vec4 ka = Ka;//vec4(1);
	//vec4 kd = Kd;
	//vec4 ks = Ks;


	//lp = C*L*lp;
	//p = M*p;
	p = C * mT * p;
	//n = vec4(N*n.xyz,0);
	n = C * mR *  mT * n;

	// diffuse coefficent
	vec4 l = normalize(lp - p);
	float d = clamp(dot(n,l), 0, 1);
	//d = toon_refl(d);

	// specular coefficient
	vec4 v = normalize(c-p);
	vec4 r = normalize(reflect(-l,n));
	float s = clamp(dot(v,r), 0, 1);
	s = pow(s, alpha);
	//s = toon_refl(s);

	//outline
	float t = dot(n, v);
	//if(t < outline_edge)
		//return vec4(0);
	
	return ka*Li + kd*d*Li + ks*s*Li;
}