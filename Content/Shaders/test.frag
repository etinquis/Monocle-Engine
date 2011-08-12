#version 140

out vec4 vColor;

uniform float totalTime;
uniform float deltaTime;

void main(void)
{
    vColor = vec4(sin(totalTime) / cos(deltaTime), cos(totalTime) / tan(deltaTime), tan(totalTime) / sin(deltaTime), 1.0);
}

