# FileRequester
FileRequester for Windows command line.

![bench](https://raw.githubusercontent.com/pedromagician/CMD_FileRequester/main/pic/screenshot.png)

echo|set /p="@set mytmp=">1.bat\
filerequester -open>>1.bat\
call 1.bat\
echo %mytmp%
