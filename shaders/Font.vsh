#version 410

layout(location = 5) in vec3 inVertex;

uniform mat4 ModelViewProjectionMatrix;
uniform mat4 TextureMatrix;

layout(location = 6) out vec2  TexCoord;

void main()
{
    gl_Position = ModelViewProjectionMatrix * vec4(inVertex, 1.0);
    TexCoord  = (TextureMatrix * vec4(inVertex, 1.0)).xy;
}
 