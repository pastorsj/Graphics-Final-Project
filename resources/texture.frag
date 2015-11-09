#version 330

uniform vec2 resolution;          //view resolution in pixels
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

void main()
{
	texCoord = fragCoord/res;
	fragColor = smoothColor;
	fragColor = vec4(0, 0, 0, 1);
//	fragColor = vec4(texCoord, 1, 1);

	fragColor = texture(texId, texCoord);
	
//	fragColor = edgeDetect();
}