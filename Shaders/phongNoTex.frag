#version 150
// Phong Shader without texture

in vec3 exPosition;
in vec3 exNormal;
in vec2 exTexCoord;

uniform float ambientCoeff;
uniform float diffuseCoeff;
uniform float specularCoeff;
uniform uint specularExponent;

//uniform vec3 noise;

uniform vec3 cameraPosition;
uniform vec3 planetPosition;
uniform float planetRadius;
//uniform vec3 color;

uniform sampler2D Tex3;

out vec4 out_Color;

void main(void)
{
	vec3 lightPosition = vec3(0, 0, 1);
	vec3 lightColor = vec3(1, 1, 1);
	float shadow = 1;
	// Shadows
	mat4 L = outerProduct(vec4(lightPosition, 1), vec4(exPosition, 1)) - outerProduct(vec4(exPosition, 1), vec4(lightPosition, 1));
	
	float a = L[1][0];
	float b = L[2][0];
	float c = L[3][0];
	float d = L[2][1];
	float e = L[3][1];
	float f = L[3][2];
	
	// Calculate Dual Plücker coordinates
	mat4 Ltilde;
	Ltilde[0] = vec4(0, -f, e, -d);
	Ltilde[1] = vec4(f, 0, -c, b);
	Ltilde[2] = vec4(-e, c, 0, -a);
	Ltilde[3] = vec4(d, -b, a, 0); 
	
	// Normalize Coordnates and point
	
	//float pNorm = sqrt(pow(planetPosition[0], 2) + pow(planetPosition[1], 2) + pow(planetPosition[2], 2));
	//vec3 noise = vec3((texture(Tex3, exTexCoord*20)), (texture(Tex3, exTexCoord*21)), length(texture(Tex3, exTexCoord*22)));
	float softness = 0.02;
	vec4 point = vec4(planetPosition + softness, 1);
	
	mat3 A = transpose(mat3(Ltilde)) * mat3(Ltilde);
	float LNorm = sqrt(2) / sqrt( A[0][0] + A[1][1] + A[2][2] );
	Ltilde = Ltilde * LNorm;
	
	vec4 dist = Ltilde * point;
	
	float lineDistance = length(vec3(dist));
	
	if ( lineDistance < planetRadius  )
	{
		shadow = 0.8;
	}
	
	// Phong
	vec3 normal = normalize(exNormal);
	
	vec3 lightDirection = normalize(lightPosition - exPosition);
	
	vec3 reflection = normalize(2 * normal * dot(lightDirection, normal) - lightDirection);
    vec3 cameraDirection = normalize(cameraPosition - exPosition);

    float diffuseComponent = max(dot(normal, lightDirection), 0) * shadow;
    float specularComponent = pow(max(dot(reflection, cameraDirection), 0), specularExponent) * shadow;
	
	vec3 totalLight = (ambientCoeff + diffuseCoeff*diffuseComponent + specularCoeff*specularComponent )*lightColor;
	

	vec3 color = vec3(1, 1, 1);
	
	out_Color = vec4(totalLight, 1);
	//out_Color = vec4(1, 1, 1, 1);
}
