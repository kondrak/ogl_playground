#version 410

uniform mat4  ModelViewProjectionMatrix;

layout(location = 0) in vec3 inVertex;
layout(location = 1) in vec4 inVertexColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) out vec2 TexCoord;
layout(location = 4) out vec4 vertexColor;
layout(location = 5) in vec3 offset;
layout(location = 8) out int instanceID;

void main()
{
    gl_Position = ModelViewProjectionMatrix * vec4(inVertex + offset, 1.0);    
	TexCoord    = inTexCoord; 
    vertexColor = inVertexColor;
    instanceID = gl_InstanceID;
}
 