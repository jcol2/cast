@echo off

@REM todo get output from build.bat to determine build failure, then disable test.exe accordingly
call build.bat
call dst\test.exe