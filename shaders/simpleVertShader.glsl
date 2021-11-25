#version 330 core
layout (location = 0) in vec3 aPos;

uniform float theta;
uniform float xDisp;
uniform float yDisp;

void main() {
     mat3 rotMatrix;
     rotMatrix[0] = vec3(cos(theta), -sin(theta), 0);
     rotMatrix[1] = vec3(sin(theta), cos(theta), 0);
     rotMatrix[2] = vec3(0, 0, 1);
     vec3 newPos = rotMatrix * aPos;
     gl_Position = vec4(newPos.x+xDisp, newPos.y+yDisp, newPos.z, 1.0);
}