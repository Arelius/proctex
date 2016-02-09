in vec2 outTexCoord;

#define color vec3
#define point vec3

color testMain(point coords);

void main(void)
{
    gl_FragColor = vec4(testMain(vec3(outTexCoord, 0.0)), 1.0);
}
