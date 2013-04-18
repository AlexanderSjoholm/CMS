#version 150

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;
//in vec3 inColor;

out vec3 exNormal;
out vec3 exPosition;
out vec2 exTexCoord;
//out vec3 exColor

uniform mat4 scaleTrans;
uniform mat4 rotX;
uniform mat4 rotY;
uniform mat4 rotZ;
uniform mat4 projectionMatrix;
uniform mat4 lookAtMatrix;

void main(void)
{
	gl_Position = projectionMatrix * lookAtMatrix * scaleTrans * rotY * rotZ * rotX * vec4(inPosition, 1.0);
	exPosition = vec3(scaleTrans * rotY * rotZ * rotX * vec4(inPosition, 1.0));
	exNormal = mat3(scaleTrans * rotY * rotZ * rotX) * inNormal;
	exTexCoord = inTexCoord;
	//ex_Color = in_Color;
}
