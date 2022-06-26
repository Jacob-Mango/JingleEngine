@echo off

cd /D "%~dp0"

"build/premake5.exe" vs2019
