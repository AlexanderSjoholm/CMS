#version 150

in vec3 exPosition;
in vec3 exNormal;
in vec2 exTexCoord;
in mat3 normalTransform;

uniform float ambientCoeff;
uniform float diffuseCoeff;
uniform float specularCoeff;
uniform uint specularExponent;

uniform vec3 cameraPosition;
uniform vec3 noiseVector;

uniform sampler2D Tex0;		// Texture 1
uniform sampler2D Tex3;		// Normal Map
uniform sampler2D Tex4;		// Noise Texture

out vec4 out_Color;

void main(void)
{
	vec3 lightPosition = vec3(0, 0, 0);
	vec3 lightColor = vec3(1, 1, 1);
	
	// Texture
	vec4 texel0 = texture(Tex0, exTexCoord);
	vec3 normalMap = texture(Tex3, exTexCoord).rgb;
	// * 2 - 1, to set the normal range from - 1 to 1;
	normalMap = normalMap * 2 - 1; 
	
	// Tangent Basis
	// I assume the top of the sphere is vec3(0, 1, 0)
	// and that the exNormal equals the point it is normal for due to unit sphere
	vec3 helpVector = normalize(cross(exNormal, (vec3(0, 1.1, 0) - exNormal)));
	vec3 tangentVector = normalize(cross(helpVector, exNormal));		
	vec3 biTangentVector = normalize(cross(tangentVector, exNormal));	
	
	// NormalMapping
	vec3 normalAdjustment = - normalMap.r*tangentVector + normalMap.g*biTangentVector + normalMap.b*exNormal;
	
	// Phong	
	//vec3 normal = normalize(normalTransform * normalAdjustment);
	vec3 normal = normalize(normalTransform * exNormal);
	
	vec3 lightDirection = normalize(lightPosition - exPosition);
	
	vec3 reflection = normalize(2 * normal * dot(lightDirection, normal) - lightDirection);
    vec3 cameraDirection = normalize(cameraPosition - exPosition);

    float diffuseComponent = max(dot(normal, lightDirection), 0);
    float specularComponent = pow(max(dot(reflection, cameraDirection), 0), specularExponent);
	
	vec3 totalLight = (ambientCoeff + diffuseCoeff*diffuseComponent + specularCoeff*specularComponent)*lightColor;

	out_Color = texel0 * vec4(totalLight, 1);
}
