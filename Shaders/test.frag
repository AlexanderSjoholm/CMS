#version 150

in vec3 exPosition;
in vec3 exNormal;
in vec2 exTexCoord;
//in vec3 exColor;

uniform sampler2D Tex0;
uniform sampler2D Tex1;

out vec4 out_Color;

void main(void)
{
	vec3 lightPosition = vec3(0, 5, 10);
	vec3 lightColor = vec3(1, 1, 1);
	float ambientCoeff = 0.3;
	float diffuseCoeff = 0.7;
	float specularCoeff = 0.4;
	
	vec3 normal = normalize(exNormal);
	
	vec3 lightDirection = normalize(lightPosition - exPosition);
	
	vec3 reflection = normalize(2 * normal * dot(lightDirection, normal) - lightDirection);
    //vec3 cameraDirection = normalize(cameraPosition - exPosition);

    float diffuseComponent = max(dot(normal, lightDirection), 0);
    //float specularComponent = max(pow(dot(reflection, cameraDirection), specularExponent), 0);
	
	vec3 totalLight = (ambientCoeff + diffuseCoeff*diffuseComponent)*lightColor;
	
	// Texture
	vec4 texel0 = texture(Tex0, exTexCoord);
	vec4 texel1 = texture(Tex1, exTexCoord);
	
	//out_Color = vec4(abs(ex_Position), 1.0);
	//out_Color = vec4(ex_Color, 1.0);
	//out_Color = vec4(1, 1, 1, 1);
	//out_Color = vec4(totalLight, 1);
	//out_Color = texel0 * vec4(1.0, 1.0, 1.0, 1.0);
	//out_Color = texel0 * vec4(totalLight, 1);
	out_Color = (texel0 * diffuseComponent + texel1 * (1 - diffuseComponent)); // * vec4(totalLight, 1);
}
