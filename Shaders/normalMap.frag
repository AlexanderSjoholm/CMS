#version 150
// Phong Shader without texture

in vec3 exPosition;
in vec3 exNormal;
in vec2 exTexCoord;
in mat3 normalTransform;

uniform float ambientCoeff;
uniform float diffuseCoeff;
uniform float specularCoeff;
uniform uint specularExponent;

//uniform vec3 noise;

uniform vec3 positions[20];

uniform vec3 cameraPosition;
//uniform vec3 planetPosition;
//uniform float planetRadius;
uniform vec3 radius[20];
//uniform vec3 color;

uniform sampler2D Tex3; 	//Normalmap

out vec4 out_Color;

void main(void)
{
	// Det råkade blo speglat
	vec2 texFix = exTexCoord;
	texFix[0] = 1-texFix[0];
	// -------------- Shadows --------------
	
	vec3 lightPosition = vec3(0, 0, 0);
	vec3 lightColor = vec3(1, 1, 1);
	float shadow = 1;
	
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
	
	mat3 A = transpose(mat3(Ltilde)) * mat3(Ltilde);
	float LNorm = sqrt(2) / sqrt( A[0][0] + A[1][1] + A[2][2] );
	Ltilde = Ltilde * LNorm;
	
	// Normalize Coordnates and point
	
	//float pNorm = sqrt(pow(planetPosition[0], 2) + pow(planetPosition[1], 2) + pow(planetPosition[2], 2));
	//vec3 noise = vec3((texture(Tex3, exTexCoord*20)), (texture(Tex3, exTexCoord*21)), length(texture(Tex3, exTexCoord*22)));

	vec3 planetPosition;
	vec3 radiusVector;
	float planetRadius;
	vec4 point;
	vec4 dist;
	float lineDistance;
	
	vec3  pixelLightVector = (exPosition - lightPosition);
	vec3  pixelLightVectorNorm = normalize(pixelLightVector);
	float pixelLightVectorLength = length(exPosition - lightPosition);
	
	for (int i = 0; i < 10; i++)
	{
		planetPosition = positions[i];
		radiusVector = radius[i];
		planetRadius = radiusVector[0];
		// Side makes sure the pixel, planet and light source are all in right place produce shadows
		float side = dot((planetPosition - lightPosition), pixelLightVectorNorm) / pixelLightVectorLength;
		
		if ( (side > 0) && (side < 1) )
		{
			point = vec4(planetPosition, 1);
			
			dist = Ltilde * point;
			
			lineDistance = length(vec3(dist));
			
			if ( lineDistance < planetRadius  )
			{
				shadow = 0;
			}
		}
	}
	
	// Normal Map
	
	//vec4 normalMap = texture(Tex3, exTexCoord);
	vec3 normalMap = texture(Tex3, texFix).bgr;
	normalMap = normalMap * 2 - 1; 
	//vec4 texel0 = texture(Tex0, exTexCoord);
	
	
	// Phong
	//vec3 normal = normalize(normalTransform * exNormal);
	vec3 normal = normalize(vec3(normalMap));
	
	vec3 lightDirection = normalize(lightPosition - exPosition);
	
	vec3 reflection = normalize(2 * normal * dot(lightDirection, normal) - lightDirection);
    vec3 cameraDirection = normalize(cameraPosition - exPosition);

    //float diffuseComponent = max(dot(normal, lightDirection), 0) * shadow;
    //float specularComponent = pow(max(dot(reflection, cameraDirection), 0), specularExponent) * shadow;
	
	float diffuseComponent = max(dot(normal, lightDirection), 0);
	float specularComponent = pow(max(dot(reflection, cameraDirection), 0), specularExponent);
	
	vec3 totalLight = (ambientCoeff + shadow * (diffuseCoeff*diffuseComponent + specularCoeff*specularComponent) )*lightColor;
	
	
	//vec3 color = vec3(1, 1, 1);
	
	out_Color = vec4(totalLight, 1);
	
	//out_Color = vec4(1, 1, 1, 1);
}
