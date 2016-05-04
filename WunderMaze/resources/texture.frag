#version 330

uniform vec2 resolution;          //view resolution in pixels
uniform float animationTime;
uniform sampler2D texId;
uniform int overlayId;
uniform sampler2D texOverlayId;
uniform sampler2D texVictoryOverlayId;
//uniform sampler2DRect texId;

//smooth in vec4 smoothColor;

//out vec4 fragColor;
out vec4 fragColor;

vec2 res = vec2(resolution);
vec2 fragCoord = gl_FragCoord.xy;
vec2 texCoord = fragCoord/res;

vec4 getProperTexture(vec2 desiredCoord) {
	if(overlayId == 1) {
		if(texture(texOverlayId, desiredCoord).x != 0) {
			return texture(texOverlayId, desiredCoord);
		} else {
			return texture(texId, desiredCoord);
		}
	} else if(overlayId == 2) {
		if(texture(texVictoryOverlayId, desiredCoord).x != 0) {
			return texture(texVictoryOverlayId, desiredCoord);
		} else {
			return texture(texId, desiredCoord);
		}
	} else {
		return texture(texId, desiredCoord);
	}
}

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
	float range = res.x / 2;
	vec2 centerPos = res / 2;
	float modifier = 1 - (abs(30 - animationTime) / 30);

	vec2 toFrag = fragCoord - centerPos;
	float dis = length(toFrag);
	if(dis>range * modifier)
		return getProperTexture(texCoord);
	float scale = ( pow(1-dis/(range*modifier),3))*5;

	mat2 rotZ = mat2(cos(scale), sin(scale), -sin(scale), cos(scale));
	vec2 swlCoord = rotZ * toFrag;
	return getProperTexture((centerPos+swlCoord)/res);
}


void main()
{
	texCoord = fragCoord/res;
//	fragColor = smoothColor;
	fragColor = vec4(0, 0, 0, 1);
//	fragColor = vec4(texCoord, 1, 1);

	if(animationTime > 0) {
		if(overlayId != 2) {
			fragColor = swirl();
		} else {
			fragColor = edgeDetect();
		}
	}
	else {
		fragColor = getProperTexture(texCoord);
	}
//	fragColor = swirl();
	
//	fragColor = edgeDetect();
}