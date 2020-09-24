@echo off
setlocal EnableDelayedExpansion

for /r .\images %%f in (*.ppm) do (
    set fname=%%f
    set pngname=!fname:~,-3!png
    if not exist !pngname! (
        ffmpeg -i !fname! !pngname!
    )
)