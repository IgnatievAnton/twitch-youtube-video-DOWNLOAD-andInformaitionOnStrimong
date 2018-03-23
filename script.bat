@echo off
set /p link=< url.txt
set /p name=< name.txt
copy nul hack.txt
set a= youtube-dl --get-url %link%
%a% >>hack.txt
set /p hc=< hack.txt
%name%
ffmpeg -i %hc% -c copy %name%
pause  
 
