#include "Network/TCPClientComponent.h"
#include "IPAddressAsyncResolve.h"
#include "SocketSubsystem.h"
#include "Sockets.h"
#include "TCPWrapperUtility.h"
#include "Async/Async.h"
#include "Network/PacketHeader.h"

TFuture<void> RunLambdaOnBackGroundThread(TFunction<void()> InFunction) {
	return Async(EAsyncExecution::Thread, InFunction);
}

UTCPClientComponent::UTCPClientComponent(const FObjectInitializer& init) : UActorComponent(init) {
	bShouldAutoConnectOnBeginPlay = true;
	bReceiveDataOnGameThread = true;
	bWantsInitializeComponent = true;
	bAutoActivate = true;
	bAutoDisconnectOnSendFailure = true;
	bAutoReconnectOnSendFailure = true;
	ConnectionIP = FString(TEXT("127.0.0.1"));
	ConnectionPort = 3000;
	ClientSocketName = FString(TEXT("unreal-tcp-client"));
	ClientSocket = nullptr;

	BufferMaxSize = 2 * 1024 * 1024; //default roughly 2mb
}

void UTCPClientComponent::ConnectToSocketAsClient(const FString& InIP /*= TEXT("127.0.0.1")*/, const int32 InPort /*= 3000*/) {
	//Already connected? attempt reconnect
	if (IsConnected()) {
		CloseSocket();
		ConnectToSocketAsClient(InIP, InPort);
	}

	ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);

	if (SocketSubsystem == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("TCPClientComponent: SocketSubsystem is nullptr"));
		return;
	}

	auto ResolveInfo = SocketSubsystem->GetHostByName(TCHAR_TO_ANSI(*InIP));
	while (!ResolveInfo->IsComplete());

	auto error = ResolveInfo->GetErrorCode();

	if (error != 0) {
		UE_LOG(LogTemp, Error, TEXT("TCPClientComponent: DNS resolve error code %d"), error);
		return;
	}

	RemoteAdress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	RemoteAdress->SetRawIp(ResolveInfo->GetResolvedAddress().GetRawIp()); // todo: somewhat wasteful, we could probably use the same address object?
	RemoteAdress->SetPort(InPort);

	ClientSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, ClientSocketName, false);

	//Set Send Buffer Size
	ClientSocket->SetSendBufferSize(BufferMaxSize, BufferMaxSize);
	ClientSocket->SetReceiveBufferSize(BufferMaxSize, BufferMaxSize);

	//Listen for data on our end
	ClientConnectionFinishedFuture = FTCPWrapperUtility::RunLambdaOnBackGroundThread([&] {
		double LastConnectionCheck = FPlatformTime::Seconds();

		uint32 BufferSize = 0;
		TArray<uint8> ReceiveBuffer;
		bShouldAttemptConnection = true;

		while (bShouldAttemptConnection) {
			if (ClientSocket->Connect(*RemoteAdress)) {
				FTCPWrapperUtility::RunLambdaOnGameThread([&] {
					OnConnected.Broadcast();
				});
				bShouldAttemptConnection = false;
				continue;
			}

			//reconnect attempt every 3 sec
			FPlatformProcess::Sleep(3.f);
		}

		bShouldReceiveData = true;

		while (bShouldReceiveData) {
			TArray<uint8> HeaderBuffer;
			constexpr int32 HeaderSize = sizeof(FPacketHeader);
			HeaderBuffer.AddZeroed(HeaderSize);

			if (!ReceiveDesiredBytes(HeaderBuffer.GetData(), HeaderSize)) {
				continue;
			}

			FPacketHeader Header;
			{
				FMemoryReader Reader(HeaderBuffer);
				Reader << Header;
			}

			const int32 PayloadSize = Header.PacketSize - HeaderSize;
			if (PayloadSize == 0) {
				TArray<uint8> EmptyPayload;
				EmptyPayload.Append(HeaderBuffer);

				AsyncTask(ENamedThreads::GameThread, [&, EmptyPayload] {
					OnReceivedBytes.Broadcast(EmptyPayload);
				});
				continue;
			}
			
			TArray<uint8> PayloadBuffer;
			PayloadBuffer.Append(HeaderBuffer);
			PayloadBuffer.AddZeroed(PayloadSize);

			if (!ReceiveDesiredBytes(PayloadBuffer.GetData() + HeaderSize, PayloadSize)) {
				continue;
			}
			
			AsyncTask(ENamedThreads::GameThread, [&, PayloadBuffer] {
				OnReceivedBytes.Broadcast(PayloadBuffer);
			});

			ClientSocket->Wait(ESocketWaitConditions::WaitForReadOrWrite, FTimespan(10));
		}
	});
}

void UTCPClientComponent::CloseSocket() {
	if (ClientSocket) {
		bShouldReceiveData = false;
		ClientConnectionFinishedFuture.Get();

		ClientSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ClientSocket);
		ClientSocket = nullptr;

		OnDisconnected.Broadcast();
	}
}

bool UTCPClientComponent::Emit(const TArray<uint8>& Bytes) {
	if (IsConnected()) {
		int32 BytesSent = 0;
		bool bDidSend = ClientSocket->Send(Bytes.GetData(), Bytes.Num(), BytesSent);


		//If we're supposedly connected but failed to send
		if (IsConnected() && !bDidSend) {
			UE_LOG(LogTemp, Warning, TEXT("Sending Failure detected"));

			if (bAutoDisconnectOnSendFailure) {
				UE_LOG(LogTemp, Warning, TEXT("disconnecting socket."));
				CloseSocket();
			}

			if (bAutoReconnectOnSendFailure) {
				UE_LOG(LogTemp, Warning, TEXT("reconnecting..."));
				ConnectToSocketAsClient(ConnectionIP, ConnectionPort);
			}
		}
		return bDidSend;
	}
	return false;
}

bool UTCPClientComponent::IsConnected() {
	return (ClientSocket && (ClientSocket->GetConnectionState() == ESocketConnectionState::SCS_Connected));
}

void UTCPClientComponent::InitializeComponent() {
	Super::InitializeComponent();
}

void UTCPClientComponent::UninitializeComponent() {
	Super::UninitializeComponent();
}

void UTCPClientComponent::BeginPlay() {
	Super::BeginPlay();

	if (bShouldAutoConnectOnBeginPlay) {
		ConnectToSocketAsClient(ConnectionIP, ConnectionPort);
	}
}

void UTCPClientComponent::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	CloseSocket();

	Super::EndPlay(EndPlayReason);
}

bool UTCPClientComponent::ReceiveDesiredBytes(uint8* Results, int32 Size) const {
	uint32 PendingDataSize;
	if (ClientSocket->HasPendingData(PendingDataSize) == false || PendingDataSize <= 0)
		return false;

	int32 Offset = 0;

	while (Size > 0) {
		int32 NumRead = 0;
		ClientSocket->Recv(Results + Offset, Size, NumRead);
		check(NumRead <= Size);

		if (NumRead <= 0)
			return false;

		Offset += NumRead;
		Size -= NumRead;
	}

	return true;
}
