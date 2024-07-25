pushd %~dp0

protoc.exe -I=./ --cpp_out=./Generated/ ./game_struct.proto
protoc.exe -I=./ --cpp_out=./Generated/ ./game_enum.proto
protoc.exe -I=./ --cpp_out=./Generated/ ./game_protocol.proto

GenPackets.exe --path=./game_protocol.proto --output=GameClientPacketHandler --recv=GameClient --send=GameServer --namespace=game --classname=GameClientPacketHandler --packetstart=3000
GenPackets.exe --path=./game_protocol.proto --output=GameServerPacketHandler --recv=GameServer --send=GameClient --namespace=game --classname=GameServerPacketHandler --packetstart=3000

chdir Generated

XCOPY /Y game_enum.pb.h "../../../../Server/GameServer/network/protocol"
XCOPY /Y game_enum.pb.cc "../../../../Server/GameServer/network/protocol"
XCOPY /Y game_struct.pb.h "../../../../Server/GameServer/network/protocol"
XCOPY /Y game_struct.pb.cc "../../../../Server/GameServer/network/protocol"
XCOPY /Y game_protocol.pb.h "../../../../Server/GameServer/network/protocol"
XCOPY /Y game_protocol.pb.cc "../../../../Server/GameServer/network/protocol"

XCOPY /Y game_enum.pb.h "../../../../Client/Source/MapleStory/Public/Network/Protocols"
XCOPY /Y game_enum.pb.cc "../../../../Client/Source/MapleStory/Private/Network/Protocols"
XCOPY /Y game_struct.pb.h "../../../../Client/Source/MapleStory/Public/Network/Protocols"
XCOPY /Y game_struct.pb.cc "../../../../Client/Source/MapleStory/Private/Network/Protocols"
XCOPY /Y game_protocol.pb.h "../../../../Client/Source/MapleStory/Public/Network/Protocols"
XCOPY /Y game_protocol.pb.cc "../../../../Client/Source/MapleStory/Private/Network/Protocols"

DEL /S /Q /F *.pb.h
DEL /S /Q /F *.pb.cc

chdir ..
MOVE .\GameClientPacketHandler.h .\game_client_packet_handler.h
MOVE .\GameServerPacketHandler2.h .\GameServerPacketHandler.h
XCOPY /Y game_client_packet_handler.h "../../../Server/GameServer/network/game"
XCOPY /Y GameServerPacketHandler.h "../../../Client/Source/MapleStory/Public/Network"
DEL /S /Q /F *.h