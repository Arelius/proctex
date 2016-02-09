in vec2 outTexCoord;

vec4 testMain(vec2 coords);

void main(void)
{
    gl_FragColor = testMain(outTexCoord);
}
