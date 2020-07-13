#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec3 inTangent;
layout(location = 3) in vec3 inNormal;

layout(location = 0) out vec2 outUV;

void main() 
{
    outUV = inUV;
    gl_Position = vec4(inPosition, 1.0);
}