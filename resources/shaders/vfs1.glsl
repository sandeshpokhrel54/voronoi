#version 330 core
out vec4 FragColor;


vec3 color = vec3(0.5, 0.8, 0.2);
// uniform vec3 color;


void main()
{
    FragColor = vec4(color,1.0f);

}