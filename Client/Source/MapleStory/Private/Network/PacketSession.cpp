#include "Network/PacketSession.h"

#include "MapleGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Network/LoginServerPacketHandler.h"
#include "Network/NetworkWorker.h"

FPacketSession::FPacketSession(FSocket* Socket) : Socket(Socket) {}

void FPacketSession::Run() {
	RecvWorkerThread = MakeShared<FRecvWorker>(Socket, AsShared());
	SendWorkerThread = MakeShared<FSendWorker>(Socket, AsShared());
}

void FPacketSession::Disconnect() {
	bRunning = false;

	WaitForEmptyRecvQueue();

	SendPacketQueue.Empty();
	RecvPacketQueue.Empty();

	if (RecvWorkerThread) {
		RecvWorkerThread->Destroy();
		RecvWorkerThread = nullptr;
	}

	if (SendWorkerThread) {
		SendWorkerThread->Destroy();
		SendWorkerThread = nullptr;
	}
}

void FPacketSession::OnServerDisconnected() {
	const UWorld* World = GEngine->GetWorld();

	if (UMapleGameInstance* GameInstance = Cast<UMapleGameInstance>(UGameplayStatics::GetGameInstance(World))) {
		GameInstance->QuitGame();
	}
}

void FPacketSession::HandleRecvPackets() {
	if (!bRunning) return;
	
	while (!RecvPacketQueue.IsEmpty()) {
		TArray<uint8> Packet;

		if (!RecvPacketQueue.Dequeue(Packet)) break;

		FPacketSessionRef Session = AsShared();
		FLoginServerPacketHandler::HandlePacket(Session, Packet.GetData(), Packet.Num());
	}
}

void FPacketSession::SendPacket(const FSendBufferRef& SendBuffer) {
	SendPacketQueue.Enqueue(SendBuffer);
}

void FPacketSession::EnqueueRecvPacket(const TArray<uint8>& Packet) {
	RecvPacketQueue.Enqueue(Packet);
}
