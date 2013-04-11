#version 150

in vec3 in_Position;
//in vec3 in_Color;
//in vec3 in_Normal;
//in vec2 inTexCoord;

out vec2 texCoord;

uniform mat4 scaleTrans;
uniform mat4 rotX;
uniform mat4 rotY;
uniform mat4 rotZ;
uniform mat4 projectionMatrix;
uniform mat4 lookAtMatrix;

out vec3 ex_Position;
out vec3 ex_Color;

void main(void)
{
	gl_Position = projectionMatrix * lookAtMatrix * scaleTrans * rotY * rotZ * rotX * vec4(in_Position, 1.0);
	ex_Position = in_Position;
	//texCoord = inTexCoord;
	//ex_Color = in_Color;
}
