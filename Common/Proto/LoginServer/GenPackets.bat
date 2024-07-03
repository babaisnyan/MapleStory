pushd %~dp0

protoc.exe -I=./ --cpp_out=./Generated/ ./login_struct.proto
protoc.exe -I=./ --cpp_out=./Generated/ ./login_enum.proto
protoc.exe -I=./ --cpp_out=./Generated/ ./login_protocol.proto

GenPackets.exe --path=./login_protocol.proto --output=LoginClientPacketHandler --recv=LoginClient --send=LoginServer
GenPackets.exe --path=./login_protocol.proto --output=LoginServerPacketHandler --recv=LoginServer --send=LoginClient

chdir Generated
@REM MOVE .\LoginEnum.pb.h .\login_enum.pb.h
@REM MOVE .\LoginEnum.pb.cc .\login_enum.pb.cc
@REM MOVE .\LoginStruct.pb.h .\login_struct.pb.h
@REM MOVE .\LoginStruct.pb.cc .\login_struct.pb.cc
@REM MOVE .\LoginProtocol.pb.h .\login_protocol.pb.h
@REM MOVE .\LoginProtocol.pb.cc .\login_protocol.pb.cc

XCOPY /Y login_enum.pb.h "../../../../Server/LoginServer/protocol"
XCOPY /Y login_enum.pb.cc "../../../../Server/LoginServer/protocol"
XCOPY /Y login_struct.pb.h "../../../../Server/LoginServer/protocol"
XCOPY /Y login_struct.pb.cc "../../../../Server/LoginServer/protocol"
XCOPY /Y login_protocol.pb.h "../../../../Server/LoginServer/protocol"
XCOPY /Y login_protocol.pb.cc "../../../../Server/LoginServer/protocol"

DEL /S /Q /F *.pb.h
DEL /S /Q /F *.pb.cc

chdir ..
MOVE .\LoginClientPacketHandler.h .\login_client_packet_handler.h
@REM MOVE .\GameServerPacketHandler.h .\game_server_packet_handler.h
XCOPY /Y login_client_packet_handler.h "../../../Server/LoginServer"
@REM XCOPY /Y server_packet_handler.h "../../DummyClient"
DEL /S /Q /F *.h

PAUSE