@echo off

REM **********************************************************************
REM * Create all the directories for the installed files

pushd %COINDIR%

if exist bin\*.* goto binexists
echo mkdir %COINDIR%\bin
mkdir bin
:binexists
if exist include\*.* goto includeexists
echo mkdir %COINDIR%\include
mkdir include
:includeexists
if exist lib\*.* goto libexists
echo mkdir %COINDIR%\lib
mkdir lib
:libexists

popd

