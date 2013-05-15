#version 150

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

out vec3 exNormal;
out vec3 exPosition;
out vec2 exTexCoord;
out mat3 normalTransform;

uniform mat4 scaleTrans;
uniform mat4 rotX;
uniform mat4 rotY;
uniform mat4 rotZ;
uniform mat4 projectionMatrix;
uniform mat4 lookAtMatrix;

void main(void)
{
	gl_Position = projectionMatrix * lookAtMatrix * scaleTrans * rotX * rotZ * rotY * vec4(inPosition, 1.0);
	exPosition = vec3(scaleTrans * rotX * rotZ * rotY * vec4(inPosition, 1.0));
	exNormal = inNormal;
	normalTransform = mat3(scaleTrans * rotX * rotZ * rotY);
	exTexCoord = inTexCoord;
}