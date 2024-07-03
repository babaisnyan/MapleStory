class ProtoParser:
    def __init__(self, start_id, recv_prefix, send_prefix):
        self.recv_packet = []
        self.send_packet = []
        self.total_packet = []
        self.start_id = start_id
        self.id = start_id
        self.recv_prefix = recv_prefix
        self.send_prefix = send_prefix

    def parse_proto(self, path):
        with open(path, "r") as f:
            lines = f.readlines()

            for line in lines:
                if not line.startswith("message"):
                    continue

                packet_name = line.split(" ")[1].strip()
                packet_name_upper = packet_name.upper()

                if packet_name.startswith(self.recv_prefix):
                    self.recv_packet.append(
                        Packet(packet_name, packet_name_upper, self.id)
                    )
                elif packet_name.startswith(self.send_prefix):
                    self.send_packet.append(
                        Packet(packet_name, packet_name_upper, self.id)
                    )
                else:
                    continue

                self.total_packet.append(Packet(packet_name, packet_name_upper, self.id))
                self.id += 1


class Packet:
    def __init__(self, name, name_upper, id):
        self.name = name
        self.name_upper = name_upper
        self.id = id
