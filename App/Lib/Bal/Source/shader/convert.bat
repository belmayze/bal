@echo off

pushd %~dp0

call ..\..\Tool\ShaderConverter\ShaderConverter.exe -i balShader.xml -o ..\..\Rom\Contents\Shader\balShader.bsa
pause

popd
