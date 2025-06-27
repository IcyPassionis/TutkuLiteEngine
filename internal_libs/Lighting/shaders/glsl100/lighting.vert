#version 100

attribute vec3 vertexPosition;
attribute vec2 vertexTexCoord;
attribute vec3 vertexNormal;

uniform mat4 mvp;
uniform mat4 matModel;
uniform mat4 matNormal;

uniform vec3 lightPos;
uniform vec3 viewPos; // Camera Position

varying vec3 fragPosition;
varying vec2 fragTexCoord;
varying vec4 fragColor;
varying vec3 fragNormal;

varying vec3 lightDir;
varying vec3 viewDir;

void main()
{
    fragPosition = vec3(matModel * vec4(vertexPosition, 1.0));
    fragNormal = normalize(vec3(matNormal * vec4(vertexNormal, 0.0)));
    fragTexCoord = vertexTexCoord;
    lightDir = normalize(lightPos - fragPosition);
    viewDir = normalize(viewPos - fragPosition);
    gl_Position = mvp * vec4(vertexPosition, 1.0);
}