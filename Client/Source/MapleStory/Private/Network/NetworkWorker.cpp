// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/NetworkWorker.h"

#include "MapleGameInstance.h"
#include "Sockets.h"
#include "Kismet/GameplayStatics.h"
#include "Network/PacketHeader.h"
#include "Network/PacketSession.h"

FRecvWorker::FRecvWorker(FSocket* Socket, TSharedPtr<FPacketSession> Session) : Socket(Socket), SessionRef(MoveTemp(Session)) {
	Thread = FRunnableThread::Create(this, TEXT("RecvWorkerThread"));
}

bool FRecvWorker::Init() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("RecvWorkerThread started"));
	return true;
}

uint32 FRecvWorker::Run() {
	while (bRunning) {
		TArray<uint8> Packet;

		if (!ReceivePacket(Packet)) continue;

		if (const TSharedPtr<FPacketSession> Session = SessionRef.Pin()) {
			Session->EnqueueRecvPacket(Packet);
		}
	}
	
	return 0;
}

void FRecvWorker::Destroy() {
	bRunning = false;
}

bool FRecvWorker::ReceivePacket(TArray<uint8>& OutPacket) {
	constexpr int32 HeaderSize = sizeof(FPacketHeader);
	TArray<uint8> HeaderBuffer;
	HeaderBuffer.AddZeroed(HeaderSize);

	if (!ReceiveDesiredBytes(HeaderBuffer.GetData(), HeaderSize)) {
		return false;
	}

	FPacketHeader Header;
	FMemoryReader Reader(HeaderBuffer);
	Reader << Header;

	OutPacket = HeaderBuffer;

	const int32 PacketSize = Header.PacketSize - HeaderSize;

	if (PacketSize == 0) {
		return true;
	}

	OutPacket.AddZeroed(PacketSize);

	if (ReceiveDesiredBytes(&OutPacket[HeaderSize], PacketSize)) {
		return true;
	}

	return false;
}

bool FRecvWorker::ReceiveDesiredBytes(uint8* Buffer, int32 Size) {
	uint32 PendingDataSize = 0;

	if (!Socket->HasPendingData(PendingDataSize) || PendingDataSize <= 0) {
		return false;
	}

	int32 Offset = 0;

	while (Size > 0) {
		int32 ReadSize = 0;

		if (!Socket->Recv(Buffer + Offset, Size, ReadSize)) {
			if (auto Session = SessionRef.Pin()) {
				Session->Disconnect();
			}

			bRunning = false;
			return false;
		}

		check(ReadSize <= Size);

		if (ReadSize <= 0) {
			return false;
		}

		Offset += ReadSize;
		Size -= ReadSize;
	}

	return true;
}

FSendWorker::FSendWorker(FSocket* Socket, TSharedPtr<FPacketSession> Session) : Socket(Socket), SessionRef(MoveTemp(Session)) {
	Thread = FRunnableThread::Create(this, TEXT("SendWorkerThread"));
}

bool FSendWorker::Init() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("SendWorkerThread started"));
	return true;
}

uint32 FSendWorker::Run() {
	while (bRunning) {
		if (const TSharedPtr<FPacketSession> Session = SessionRef.Pin()) {
			FSendBufferRef SendBuffer;

			if (!Session->DequeueSendPacket(SendBuffer)) continue;

			if (!SendPacket(SendBuffer)) {
				UE_LOG(LogTemp, Warning, TEXT("Failed to send packet"));
			}
		}
	}

	return 0;
}

bool FSendWorker::SendPacket(const FSendBufferRef& SendBuffer) const {
	if (!SendDesiredBytes(SendBuffer->GetBuffer(), SendBuffer->GetWriteSize())) {
		return false;
	}

	return true;
}

void FSendWorker::Destroy() {
	bRunning = false;
}

bool FSendWorker::SendDesiredBytes(const uint8* Buffer, int32 Size) const {
	while (Size > 0) {
		int32 BytesSent = 0;

		if (!Socket->Send(Buffer, Size, BytesSent)) {
			return false;
		}

		Size -= BytesSent;
		Buffer += BytesSent;
	}

	return true;
}
