#version 140
 
in vec3 vertex;
uniform float totalTime;
uniform float deltaTime;
 
void main() {
    gl_Position = vec4(vertex, 1.0);
}
