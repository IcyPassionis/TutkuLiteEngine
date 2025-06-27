#version 100
precision mediump float;

varying vec3 fragPosition;
varying vec3 fragNormal;
varying vec2 fragTexCoord;

varying vec3 viewDir;

#define LIGHT_DIRECTIONAL 0
#define LIGHT_POINT 1

uniform vec3 lightColor;
uniform float lightIntensity;
uniform int lightType;
uniform vec3 lightPos;
uniform vec3 lightDir;

uniform vec3 ambientColor; // Ambient light color shows when there is no light, so this base light for everything.
uniform float ambientStrength;

varying vec4 fragColor;
uniform float shininess; // Specular shinines(32 default)

uniform vec4 colDiffuse;
uniform sampler2D texture0;

void main()
{
    vec4 texelColor = texture2D(texture0, fragTexCoord);
    vec3 lightDot = vec3(0.0);
    vec3 normal = normalize(fragNormal);
    vec3 viewDirection = normalize(viewDir);
    vec3 specular = vec3(0.0);


    vec4 tint = colDiffuse * fragColor;

    // Ambient Lighting(or just base lighting)
    vec4 ambient = (ambientStrength * vec4(ambientColor, 1.0)) * fragColor;

    // Diffuse Lighting
    // Lights
    vec3 light = vec3(0.0);
    if (lightType == LIGHT_DIRECTIONAL) {
        light = -normalize(lightDir - lightPos);
    }
    if(lightType == LIGHT_POINT){
        light = normalize(lightPos - fragPosition);
    }
    float NdotL = max(dot(normal, light), 0.0) * (lightIntensity * 0.5);
    lightDot += lightColor * NdotL;

    float specCo = 0.0;
    if (NdotL > 0.0) specCo = pow(max(0.0, dot(viewDirection, reflect(-(light), normal))), 16.0); // 16 refers to shine
    specular += specCo * (lightIntensity * 0.1);


    vec4 result = (texelColor*((tint + vec4(specular, 1.0))*vec4(lightDot, 1.0)));
    result += texelColor*(ambient/10.0);
    gl_FragColor =  pow(result, vec4(1.0/2.2));
}