#version 410

uniform sampler2D sTexture;

layout(location = 6) in vec2 TexCoord;
layout(location = 7) in vec4 vertexColor;

out vec4 fragmentColor;

void main()
{
    vec4 baseTex  = texture(sTexture, TexCoord) * vertexColor;
      
    fragmentColor = baseTex; // make the output more vivid
}
