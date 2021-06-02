#version 330 core
layout(location = 0) in vec3 aPos; // the position variable has attribute position 0
layout(location = 1) in vec2 aTextCoord;

out vec2 textCoord; 

void main()
{
    gl_Position = vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor
    textCoord = aTextCoord; // set the output variable to a dark-red color
}