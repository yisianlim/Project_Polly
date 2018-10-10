#version 150 core

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;

in vec3 vertPosition;
in vec3 vertNormal;
uniform vec3 uColor;

flat out vec3 fragPosition;
flat out vec3 fragNormal;
flat out vec3 fragmentColor;

void main() {
    gl_Position = projectionMat * viewMat * modelMat * vec4(vertPosition, 1.0);

    vec4 pos = viewMat * modelMat * vec4(vertPosition, 1.0);
    fragPosition = vec3(pos) / pos.w;

    mat3 normalMat = transpose(inverse(mat3(viewMat * modelMat)));
    fragNormal = normalMat * vertNormal;
    fragmentColor = uColor;
}
