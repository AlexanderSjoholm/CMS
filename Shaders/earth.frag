#version 150

in vec3 exPosition;
in vec3 exNormal;
in vec2 exTexCoord;

uniform float ambientCoeff;
uniform float diffuseCoeff;
uniform float specularCoeff;
uniform uint specularExponent;

uniform vec3 cameraPosition;

uniform sampler2D Tex0;
uniform sampler2D Tex1;
uniform sampler2D Tex2;

out vec4 out_Color;

void main(void)
{
	vec3 lightPosition = vec3(0, 0, -10);
	vec3 lightColor = vec3(1, 1, 1);
	
	//float ambientCoeff = 0.1;
	//float diffuseCoeff = 0.3;
	//float specularCoeff = 0.6;
	//int specularExponent = 10;
	
	// Texture
	vec4 texel0 = texture(Tex0, exTexCoord);
	vec4 texel1 = texture(Tex1, exTexCoord);
	vec4 texel2 = texture(Tex2, exTexCoord);
	float specularityMap = length(vec3(texture(Tex2, exTexCoord)));
	
	// Phong
	vec3 normal = normalize(exNormal);
	
	vec3 lightDirection = normalize(lightPosition - exPosition);
	
	vec3 reflection = normalize(2 * normal * dot(lightDirection, normal) - lightDirection);
    vec3 cameraDirection = normalize(cameraPosition - exPosition);

    float diffuseComponent = max(dot(normal, lightDirection), 0);
    float specularComponent = pow(max(dot(reflection, cameraDirection), 0), specularExponent);
	
	vec3 totalLight = (ambientCoeff + diffuseCoeff*diffuseComponent + specularityMap*specularCoeff*specularComponent)*lightColor;
	
	
	
	//out_Color = vec4(abs(ex_Position), 1.0);
	//out_Color = vec4(ex_Color, 1.0);
	//out_Color = vec4(1, 1, 1, 1);
	//out_Color = vec4(totalLight, 1);
	//out_Color = texel0 * vec4(1.0, 1.0, 1.0, 1.0);
	//out_Color = texel0 * vec4(totalLight, 1);
	out_Color = (texel0 * diffuseComponent) * vec4(totalLight, 1) + texel1 * (1 - diffuseComponent);
	//out_Color = vec4(totalLight, 1);
	
}
