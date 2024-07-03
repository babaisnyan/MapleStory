pushd %~dp0
pyinstaller --onefile --noconsole ProcedureGenerator.py
MOVE .\dist\ProcedureGenerator.exe .\GenProcs.exe
@RD /S /Q .\build
@RD /S /Q .\dist
DEL /S /F /Q .\ProcedureGenerator.spec
PAUSE