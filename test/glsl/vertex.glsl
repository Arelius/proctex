in vec3 vertex;
in vec2 texCoord;

out vec2 outTexCoord;

void main(void)
{
    outTexCoord = texCoord;
    gl_Position = vec4(vertex, 1.0);
}
