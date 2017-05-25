#version 100

attribute highp   vec3 inVertex;
attribute mediump vec4 inVertexColor;
//attribute mediump vec3 inNormal;
attribute mediump vec2 inTexCoord;

uniform highp   mat4 ModelViewProjectionMatrix;

void main()
{
    gl_Position = ModelViewProjectionMatrix * vec4(inVertex, 1.0);
}
 