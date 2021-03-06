#version 330 core
layout(location = 0) in vec3 aPos; // the position variable has attribute position 0
layout(location = 1) in vec2 aTextCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 textCoord;

void main()
{
    textCoord = aTextCoord;
    gl_Position = projection * view * model * vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor
}