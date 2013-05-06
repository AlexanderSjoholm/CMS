#version 150

in vec3 exPosition;
in vec2 exTexCoord;

uniform sampler2D Tex0;
uniform float time;

out vec4 out_Color;

vec4 vit = vec4(1, 1, 1, 1);
vec4 gul = vec4(1, 1, 0, 1);

void main(void)
{	
	// Texture
	vec4 texel0 = texture(Tex0, (exTexCoord + 0.01*sin(time)));
	
	out_Color = texel0 * vec4(1.0, 1.0, 1.0, 1.0);
	//out_Color =  ( (1 + sin(exTexCoord[0]*2*3.14 + time))/2  * vit + (1 - sin(exTexCoord[0]*2*3.14 + time))/2 * gul);
}
