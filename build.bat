@echo off

pushd %~dp0\dst

cl /WX /W3 /Zi /Od /nologo /DSTANDALONE ..\cast.c /link /INCREMENTAL:NO
cl /WX /W3 /Zi /Od /nologo ..\test.c /link /INCREMENTAL:NO

popd