pushd %~dp0

protoc.exe -I=./ --cpp_out=./Generated/ ./center_struct.proto
protoc.exe -I=./ --cpp_out=./Generated/ ./center_enum.proto
protoc.exe -I=./ --cpp_out=./Generated/ ./center_protocol.proto

GenPackets.exe --path=./center_protocol.proto --output=CenterClientPacketHandler --recv=CenterClient --send=CenterServer
GenPackets.exe --path=./center_protocol.proto --output=CenterServerPacketHandler --recv=CenterServer --send=CenterClient

chdir Generated

XCOPY /Y center_enum.pb.h "../../../../Server/CenterServer/network/protocol"
XCOPY /Y center_enum.pb.cc "../../../../Server/CenterServer/network/protocol"
XCOPY /Y center_struct.pb.h "../../../../Server/CenterServer/network/protocol"
XCOPY /Y center_struct.pb.cc "../../../../Server/CenterServer/network/protocol"
XCOPY /Y center_protocol.pb.h "../../../../Server/CenterServer/network/protocol"
XCOPY /Y center_protocol.pb.cc "../../../../Server/CenterServer/network/protocol"

XCOPY /Y center_enum.pb.h "../../../../Server/LoginServer/network/protocol"
XCOPY /Y center_enum.pb.cc "../../../../Server/LoginServer/network/protocol"
XCOPY /Y center_struct.pb.h "../../../../Server/LoginServer/network/protocol"
XCOPY /Y center_struct.pb.cc "../../../../Server/LoginServer/network/protocol"
XCOPY /Y center_protocol.pb.h "../../../../Server/LoginServer/network/protocol"
XCOPY /Y center_protocol.pb.cc "../../../../Server/LoginServer/network/protocol"

XCOPY /Y center_enum.pb.h "../../../../Server/GameServer/network/protocol"
XCOPY /Y center_enum.pb.cc "../../../../Server/GameServer/network/protocol"
XCOPY /Y center_struct.pb.h "../../../../Server/GameServer/network/protocol"
XCOPY /Y center_struct.pb.cc "../../../../Server/GameServer/network/protocol"
XCOPY /Y center_protocol.pb.h "../../../../Server/GameServer/network/protocol"
XCOPY /Y center_protocol.pb.cc "../../../../Server/GameServer/network/protocol"

DEL /S /Q /F *.pb.h
DEL /S /Q /F *.pb.cc

chdir ..
MOVE .\CenterClientPacketHandler.h .\center_client_packet_handler.h
MOVE .\CenterServerPacketHandler.h .\center_server_packet_handler.h
XCOPY /Y center_client_packet_handler.h "../../../Server/CenterServer/network"
XCOPY /Y center_server_packet_handler.h "../../../Server/LoginServer/network/center"
XCOPY /Y center_server_packet_handler.h "../../../Server/GameServer/network/center"
DEL /S /Q /F *.h