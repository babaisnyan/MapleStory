pushd %~dp0

GenProcs.exe --path=../../GameServer/GameDB.xml --output=gen_procedures.h

XCOPY /Y gen_procedures.h "../../GameServer"
DEL /S /Q /F *.h

PAUSE