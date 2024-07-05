pushd %~dp0

protoc.exe -I=./ --cpp_out=./Generated/ ./login_struct.proto
protoc.exe -I=./ --cpp_out=./Generated/ ./login_enum.proto
protoc.exe -I=./ --cpp_out=./Generated/ ./login_protocol.proto

GenPackets.exe --path=./login_protocol.proto --output=LoginClientPacketHandler --recv=LoginClient --send=LoginServer
GenPackets.exe --path=./login_protocol.proto --output=LoginServerPacketHandler --recv=LoginServer --send=LoginClient

chdir Generated

XCOPY /Y login_enum.pb.h "../../../../Server/LoginServer/network/protocol"
XCOPY /Y login_enum.pb.cc "../../../../Server/LoginServer/network/protocol"
XCOPY /Y login_struct.pb.h "../../../../Server/LoginServer/network/protocol"
XCOPY /Y login_struct.pb.cc "../../../../Server/LoginServer/network/protocol"
XCOPY /Y login_protocol.pb.h "../../../../Server/LoginServer/network/protocol"
XCOPY /Y login_protocol.pb.cc "../../../../Server/LoginServer/network/protocol"

XCOPY /Y login_enum.pb.h "../../../../Client/Source/MapleStory/Public/Network/Protocols"
XCOPY /Y login_enum.pb.cc "../../../../Client/Source/MapleStory/Private/Network/Protocols"
XCOPY /Y login_struct.pb.h "../../../../Client/Source/MapleStory/Public/Network/Protocols"
XCOPY /Y login_struct.pb.cc "../../../../Client/Source/MapleStory/Private/Network/Protocols"
XCOPY /Y login_protocol.pb.h "../../../../Client/Source/MapleStory/Public/Network/Protocols"
XCOPY /Y login_protocol.pb.cc "../../../../Client/Source/MapleStory/Private/Network/Protocols"

DEL /S /Q /F *.pb.h
DEL /S /Q /F *.pb.cc

chdir ..
MOVE .\LoginClientPacketHandler.h .\login_client_packet_handler.h
MOVE .\LoginServerPacketHandler2.h .\LoginServerPacketHandler.h
XCOPY /Y login_client_packet_handler.h "../../../Server/LoginServer/network"
XCOPY /Y LoginServerPacketHandler.h "../../../Client/Source/MapleStory/Public/Network"
DEL /S /Q /F *.h

PAUSE