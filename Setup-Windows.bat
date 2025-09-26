@echo off

pushd %~dp0
python Build/Scripts/Setup_Project.py vs2022
popd

pause
