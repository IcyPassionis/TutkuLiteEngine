#version 100
precision mediump float;

varying vec3 fragPosition;
varying vec3 fragNormal;
varying vec2 fragTexCoord;

varying vec3 lightDir;
varying vec3 viewDir;

uniform vec3 lightColor;
uniform float lightIntensity;
uniform vec3 ambientColor; // Ambient light color shows when there is no light, so this base light for everything.
uniform float ambientStrength;

uniform vec3 materialColor; // Base color of a object/material
uniform float shininess; // Specular shinines(32 default)

void main()
{
    vec3 normal = normalize(fragNormal);
    vec3 lightDirection = normalize(lightDir);
    vec3 viewDirection = normalize(viewDir);

    // Ambient Lighting(or just base lighting)
    vec3 ambient = ambientStrength * ambientColor * materialColor;

    float diff = max(dot(normal,lightDirection),0.0);
    vec3 diffuse = diff * lightColor * lightIntensity * materialColor;

    vec3 halfwayDir = normalize(lightDirection + viewDirection);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    vec3 specular = spec * lightColor * lightIntensity * 0.3;


    vec3 result = (ambient + diffuse + specular);
    gl_FragColor = vec4(result, 1.0);
}