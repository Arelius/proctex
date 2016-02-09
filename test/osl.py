import subprocess
import os
import sys

shadeBin = "../../osl/bin/testshade.exe"
cmplBin = "../../osl/bin/oslc.exe"
oslHeader = "osl/header.osl"
intermediatePath = "osl/intermediate/"
includes = ["osl"]
viewerPath = "C:/Program Files (x86)/IrfanView/i_view32.exe"

def RunTest(shaderPath):
    filePre, fileExt = os.path.splitext(os.path.basename(shaderPath))
    oslPath = intermediatePath + filePre + ".osl"
    if not os.path.exists(intermediatePath):
        os.makedirs(intermediatePath)
    with open(oslPath, "w") as destFile:
        with open(oslHeader, "r") as srcFile:
            destFile.write(srcFile.read())
        with open(shaderPath, "r") as srcFile:
            destFile.write(srcFile.read())
    subprocess.call([cmplBin, oslPath])
    osoShader, osoExt = os.path.splitext(oslPath)
    outputPath = intermediatePath + "testOutput.exr";
    subprocess.call([shadeBin, "-g", "1024", "1024", "-o", "TestColor", outputPath, filePre])
    subprocess.call([viewerPath, os.path.abspath(outputPath)])

RunTest("test.shl")
