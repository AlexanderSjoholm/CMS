#version 150

in vec3 exPosition;
in vec2 exTexCoord;

uniform sampler2D Tex0;

out vec4 out_Color;

void main(void)
{	
	// Texture
	vec4 texel0 = texture(Tex0, exTexCoord);
	
	out_Color = texel0 * vec4(1.0, 1.0, 1.0, 1.0);
}
