// by nikos papadopoulos, 4rknova / 2013
// Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.

#version 330

#define MAX_BALLS 5000
#define MAX_BALLS1 4

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform float iTimeDelta;            // render time (in seconds)
uniform vec2 ballsPos[MAX_BALLS1];
uniform float ballsRadius[MAX_BALLS1];
uniform int ballsHowMany;

uniform float renderWidth;
uniform float renderHeight;
uniform vec2 center;

// Output fragment color
out vec4 finalColor;

// #define AA 4.

#define CI vec3(.3,.5,.6)
#define CO vec3(0.0745, 0.0862, 0.1058)
#define CM vec3(.0)
#define CE vec3(.8,.7,.5)

float metaball(vec2 p, float r)
{
	return r / dot(p, p);
}

vec3 samplef(in vec2 uv)
{
	float r = 0;

	for (int i = 0; i < ballsHowMany; i++) {
		r += metaball(uv + ballsPos[i], ballsRadius[i]);
	}

	vec3 c = (r > .4 && r < .7)
			  ? (vec3(step(.1, r*r*r)) * CE)
			  : (r < .9 ? (r < .7 ? CO: CM) : CI);

	return c;
}

void main()
{
	vec2 uv = fragTexCoord.xy;
	//vec2 uv = (fragCoord.xy / iResolution.xy * 2. - 1.)
	//		* vec2(iResolution.x / iResolution.y, 1) * 1.25;

    vec3 col = vec3(0);
	col += samplef(uv);
    
    finalColor = vec4(1, 0, 0, 1);//vec4(clamp(col, 0., 1.), 1);
}