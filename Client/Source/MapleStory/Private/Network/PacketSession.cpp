#include "Network/PacketSession.h"

#include "MapleGameInstance.h"
#include "Network/GameServerPacketHandler.h"
#include "Network/LoginServerPacketHandler.h"
#include "Network/NetworkWorker.h"

FPacketSession::FPacketSession(FSocket* Socket, const EServerType Type) : Socket(Socket), ServerType(Type) {
	RecvPacketQueue = MakeUnique<TQueue<TArray<uint8>>>();
	SendPacketQueue = MakeUnique<TQueue<FSendBufferRef>>();
}

void FPacketSession::Run() {
	RecvWorkerThread = MakeShared<FRecvWorker>(Socket, AsShared());
	SendWorkerThread = MakeShared<FSendWorker>(Socket, AsShared());
}

void FPacketSession::Disconnect() {
	bRunning = false;

	HandleRecvPackets();

	if (RecvWorkerThread) {
		RecvWorkerThread->Destroy();
		RecvWorkerThread.Reset();
	}

	if (SendWorkerThread) {
		SendWorkerThread->Destroy();
		SendWorkerThread.Reset();
	}

	if (RecvPacketQueue) {
		RecvPacketQueue.Reset();
		RecvPacketQueue = nullptr;
	}

	if (SendPacketQueue) {
		SendPacketQueue.Reset();
		SendPacketQueue = nullptr;
	}
}

void FPacketSession::HandleRecvPackets() {
	if (!bRunning) return;

	while (RecvPacketQueue && !RecvPacketQueue->IsEmpty()) {
		TArray<uint8> Packet;

		if (!RecvPacketQueue->Dequeue(Packet)) break;

		FPacketSessionRef Session = AsShared();
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Login packet received"));

		switch (ServerType) {
			case EServerType::Login:
				FLoginServerPacketHandler::HandlePacket(Session, Packet.GetData(), Packet.Num());
				break;
			case EServerType::Game:
				FGameServerPacketHandler::HandlePacket(Session, Packet.GetData(), Packet.Num());
				break;
		}
	}
}

void FPacketSession::SendPacket(const FSendBufferRef& SendBuffer) const {
	SendPacketQueue->Enqueue(SendBuffer);
}

void FPacketSession::EnqueueRecvPacket(const TArray<uint8>& Packet) const {
	RecvPacketQueue->Enqueue(Packet);
}
