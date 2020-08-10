@echo off

pushd %~dp0

call ..\..\Tool\ShaderConverter\ShaderConverter.exe -i bal_shader.xml -o ..\..\Rom\Contents\Shader\bal_shader.bsa

popd
