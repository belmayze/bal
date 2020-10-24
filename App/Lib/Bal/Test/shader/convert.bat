@echo off

pushd %~dp0

call ..\..\Tool\ShaderConverter\ShaderConverter.exe -i testShader.xml -o ..\..\Rom\Contents\Shader\testShader.bsa
pause

popd
