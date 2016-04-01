#version 330

uniform vec2 resolution;          //view resolution in pixels
uniform float animationTime;
uniform sampler2D texId;
//uniform sampler2DRect texId;

smooth in vec4 smoothColor;

//out vec4 fragColor;
layout(location = 0) out vec4 fragColor;

vec2 res = vec2(resolution);
vec2 fragCoord = gl_FragCoord.xy;
vec2 texCoord = fragCoord/res;

vec4 edgeDetect()
{
	vec4 sharpColor = vec4(0);
	sharpColor = texture(texId, texCoord) * 4;
	sharpColor -= texture(texId, (fragCoord + vec2(0,1))/res);
	sharpColor -= texture(texId, (fragCoord + vec2(0,-1))/res);
	sharpColor -= texture(texId, (fragCoord + vec2(1,0))/res);
	sharpColor -= texture(texId, (fragCoord + vec2(-1,0))/res);
	return sharpColor;
}

vec4 swirl()
{
	float range = res / 2;
	vec2 centerPos = res / 2;
	float modifier = 1 - (abs(30 - animationTime) / 30);

	vec2 toFrag = fragCoord - centerPos;
	float dis = length(toFrag);
	if(dis>range * modifier)
		return texture(texId, texCoord);
	float scale = ( pow(1-dis/(range*modifier),3))*5;

	mat2 rotZ = mat2(cos(scale), sin(scale), -sin(scale), cos(scale));
	vec2 swlCoord = rotZ * toFrag;
	return texture(texId, (centerPos+swlCoord)/res);
}

void main()
{
	texCoord = fragCoord/res;
	fragColor = smoothColor;
	fragColor = vec4(0, 0, 0, 1);
//	fragColor = vec4(texCoord, 1, 1);

	if(animationTime > 0) {
		fragColor = swirl();
	}
	else {
		fragColor = texture(texId, texCoord);
	}
//	fragColor = swirl();
	
//	fragColor = edgeDetect();
}