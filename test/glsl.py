import subprocess
import os
import sys

testBin = "../testutil/t2-output/win64-msvc-debug-default/glsltex.exe"
clBin = "cl.exe"
vertexPath = "glsl/vertex.glsl"
glslHeader = "glsl/frag_header.glsl"
intermediatePath = "glsl/intermediate/"
includes = ["glsl"]

def PreProcess(src, dest):
    filePre, fileExt = os.path.splitext(os.path.basename(src))
    cppArgs = [clBin, "/EP", "/P", "/C"]
    for include in includes:
        cppArgs.extend(["/I", include])
    cppArgs.append(src)
    subprocess.call(cppArgs, stdout=sys.stdout)
    if os.path.exists(dest):
        os.remove(dest)
    os.rename(filePre + ".i", dest)
    

def RunTest(shaderPath):
    filePre, fileExt = os.path.splitext(os.path.basename(shaderPath))
    glslPath = intermediatePath + filePre + ".glsl"
    if not os.path.exists(intermediatePath):
        os.makedirs(intermediatePath)
    with open(glslPath, "w") as destFile:
        with open(glslHeader, "r") as srcFile:
            destFile.write(srcFile.read())
        with open(shaderPath, "r") as srcFile:
            destFile.write(srcFile.read())
    fragmentPath = intermediatePath + filePre + ".o.glsl"
    PreProcess(glslPath, fragmentPath)
    subprocess.call([testBin, vertexPath, fragmentPath], stdout=sys.stdout)

RunTest("test.shl")
