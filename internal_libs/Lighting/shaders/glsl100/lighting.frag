#version 100
precision mediump float;

// Light "Enums"
#define MAX_LIGHTS 20
#define LIGHT_DIRECTIONAL 0
#define LIGHT_POINT 1
#define LIGHT_SPOT 2

varying vec3 fragPosition;
varying vec3 fragNormal;
varying vec2 fragTexCoord;

varying vec3 viewDir;

struct Light
{
    int enabled;
    int type;
    float intensity;
    vec3 position;
    vec3 direction;
    vec3 color;
};
uniform vec3 ambientColor; // Ambient light color shows when there is no light, so this base light for everything.
uniform float ambientStrength;

varying vec4 fragColor;
uniform float shininess; // Specular shinines(32 default)

uniform vec4 colDiffuse;
uniform Light lights[MAX_LIGHTS];
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
    for(int i = 0; i < MAX_LIGHTS; i++ )
    {
        if(lights[i].enabled == 1) {
            vec3 light = vec3(0.0);
            if (lights[i].type == LIGHT_DIRECTIONAL) {
                light = normalize(-lights[i].direction);
            }
            if (lights[i].type == LIGHT_POINT){
                light = normalize(lights[i].position - fragPosition);
            }
            float NdotL = max(dot(normal, light), 0.0) * (lights[i].intensity * 0.5);
            lightDot += lights[i].color.rgb * NdotL;

            float specCo = 0.0;
            if (NdotL > 0.0) specCo = pow(max(0.0, dot(viewDirection, reflect(-(light), normal))), 16.0);// 16 refers to shine
            specular += specCo * (lights[i].intensity * 0.1);
        }
    }
    vec4 result = (texelColor*((tint + vec4(specular, 1.0))*vec4(lightDot, 1.0)));
    result += texelColor*(ambient/10.0);
    gl_FragColor =  pow(result, vec4(1.0/2.2));
}