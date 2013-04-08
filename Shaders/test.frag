#version 150

in vec3 ex_Position;
in vec3 ex_Color;

in vec2 texCoord;

out vec4 out_Color;

void main(void)
{
	//out_Color = vec4(abs(ex_Position), 1.0);
	out_Color = vec4(ex_Color, 1.0);
}
