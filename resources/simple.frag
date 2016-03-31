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

in vec2 texMapping;
smooth in vec4 unTransPos;
in vec3 camPosition;

smooth in vec3 smoothPos;
smooth in vec3 smoothNorm;

out vec4 fragColor;

vec4 defaultShade(in vec3 pos, in vec3 norm);

void main()
{
	vec3 newPos = unTransPos.xyz;
	vec3 dis = newPos - camPosition;
	float scale = clamp(pow(inversesqrt(dot(dis, dis)), 2), 0.05, 1);
	if(immune == 1)
	{
		scale = 1.0f;
	}
    //fragColor = scale * texture(texSampler, texMapping);
	fragColor = texture(texSampler, texMapping);// + defaultShade(smoothPos, smoothNorm);
}

vec4 defaultShade(in vec3 pos, in vec3 norm)
{
	//TODO PASS THIS IN AS UNIFORM
	//float alpha = specAlpha;
	float alpha = 0.1;
	vec4 p = vec4(pos, 1);
	vec4 lp = lightPos;
	vec4 n = normalize(vec4(norm,0));
	vec4 c = vec4(0);
	vec4 Li = vec4(1);
	vec4 ka = vec4(0.05);
	vec4 kd = vec4(0.2, 0.2, 0.8, 1);
	vec4 ks = vec4(1);
	//vec4 ka = Ka;//vec4(1);
	//vec4 kd = Kd;
	//vec4 ks = Ks;
	//TODO PASS THESE IN ABOUT UNIFORMS

	lp = C*L*lp;

	//TODO pass in M as a uniform maybe?
	// M = C * mR * mT
	//mat4 M = C * mR * mT;
	p = M*p;
	n = vec4(N*n.xyz, 0);

	// diffuse coefficent
	vec4 l = normalize(lp - p);
	float d = clamp(dot(l,n), 0, 1);
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
	//vec4 temp = vec4(1,1,1,1);
	//return temp;
}