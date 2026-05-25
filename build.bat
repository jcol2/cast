@echo off

pushd %~dp0\dst

set "wflags=/WX /W4 /wd4057 /wd4100 /wd4189 /wd4201"

cl %wflags% /Zi /Od /nologo /DSTANDALONE ..\cast.c /link /INCREMENTAL:NO
cl %wflags% /Zi /Od /nologo ..\test.c /link /INCREMENTAL:NO

popd