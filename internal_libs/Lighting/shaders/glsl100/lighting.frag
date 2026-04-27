#version 100
precision mediump float;


#define MAX_LIGHTS 20
// Light "Enums" with define
#define LIGHT_DIRECTIONAL 0
#define LIGHT_POINT 1
#define LIGHT_SPOT 2

varying vec3 fragPosition;
varying vec3 fragNormal;
varying vec2 fragTexCoord;

varying vec3 viewDir; // Main Camera's direction to frag
varying vec4 fragPosLightSpace;

struct Light
{
    int enabled;
    int type;
    float intensity;
    vec3 position;
    vec3 direction;
    vec3 color;
};
uniform Light lights[MAX_LIGHTS];

uniform vec3 ambientColor; // Ambient light color shows when there is no light, so this base light for everything.
uniform float ambientStrength; // Ambient strength, it could changed in some areas?
uniform vec3 viewPos;

varying vec4 fragColor;
uniform float shininess;

uniform vec4 colDiffuse;
uniform sampler2D texture0;
uniform sampler2D shadowMap; 

float calculateShadow(vec4 posLightSpace, vec3 normal, vec3 lightDir)
{
    vec3 projCoords = posLightSpace.xyz / posLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.x < 0.0 || projCoords.x > 1.0 ||
        projCoords.y < 0.0 || projCoords.y > 1.0 ||
        projCoords.z > 1.0)
    {
        return 0.0;
    }

    float shadow = 0.0;
    vec2 texelSize = vec2(1.0 / 2048.0);
    float currentDepth = projCoords.z;
    float bias = 0.003;

    for(int x = -2; x <= 2; ++x)
    {
        for(int y = -2; y <= 2; ++y)
        {
            float pcfDepth = texture2D(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 32.0;
    return shadow;
}

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
    for(int i = 0; i < MAX_LIGHTS; i++)
    {
        if(lights[i].enabled == 1) {
            vec3 light = vec3(0.0);
            float shadow = 0.0;

            if (lights[i].type == LIGHT_DIRECTIONAL) {
                light = normalize(-lights[i].direction);
                shadow = calculateShadow(fragPosLightSpace, normal, light);
            }
            if (lights[i].type == LIGHT_POINT) {
                light = normalize(lights[i].position - fragPosition);
            }
            // Specular lighting

            float NdotL = max(dot(normal, light), 0.0) * lights[i].intensity;
            lightDot += lights[i].color.rgb * NdotL * (1.0 - shadow);

            float specCo = 0.0;
            if (NdotL > 0.0 && shadow < 0.5)
                specCo = pow(max(0.0, dot(viewDirection, reflect(-(light), normal))), 16.0);
            specular += specCo * (lights[i].intensity * 0.1);
        }
    }
    vec4 result = (texelColor*((tint + vec4(specular, 1.0))*vec4(lightDot, 1.0)));
    result += texelColor*(ambient/10.0);

    // Gamma correction
    gl_FragColor = pow(result, vec4(1.0/2.2));

}
