echo Setting up Dota2Cheat files...

mkdir %USERPROFILE%\Documents\Dota2Cheat
mkdir %USERPROFILE%\Documents\Dota2Cheat\config
copy .\Data\signatures.json %USERPROFILE%\Documents\Dota2Cheat\signatures.json
copy .\Data\vmt.json %USERPROFILE%\Documents\Dota2Cheat\vmt.json 
xcopy .\Data\assets\ %USERPROFILE%\Documents\Dota2Cheat\assets\dota\ /E /I /Y
echo Done!