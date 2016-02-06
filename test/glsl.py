import subprocess

testBin = "../testutil/t2-output/win64-msvc-debug-default/glsltex.exe"
vertexPath = "glsl/vertex.glsl"

def RunTest(shaderPath):
    subprocess.call([testBin, vertexPath, shaderPath])

RunTest("glsl/test.glsl")
