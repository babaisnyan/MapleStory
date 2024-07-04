import argparse
import jinja2
import ProtoParser


def main():
    arg_parser = argparse.ArgumentParser(description="Packet Generator")
    arg_parser.add_argument(
        "--path",
        type=str,
        default="O:/Visual Studio Projects/Server/Common/Protocol.proto",
        help="Path to the proto file",
    )
    arg_parser.add_argument(
        "--output", type=str, default="TestPacketHandler", help="Class name"
    )
    arg_parser.add_argument("--recv", type=str, default="C_", help="Receive prefix")
    arg_parser.add_argument("--send", type=str, default="S_", help="Send prefix")
    arg_parser.add_argument(
        "--packetstart", type=int, default=1000, help="Packet start"
    )
    args = arg_parser.parse_args()

    parser = ProtoParser.ProtoParser(args.packetstart, args.recv, args.send)
    parser.parse_proto(args.path)

    file_loader = jinja2.FileSystemLoader("Templates")
    env = jinja2.Environment(loader=file_loader)

    tmeplate = env.get_template("PacketHandler.h.jinja")
    tmeplate2 = env.get_template("PacketHandler2.h.jinja")
    output = tmeplate.render(parser=parser, output=args.output)
    output2 = tmeplate2.render(parser=parser, output=args.output)

    with open(f"{args.output}.h", "w+") as f:
        f.write(output)

    with open(f"{args.output}2.h", "w+") as f:
        f.write(output2)

    return


if __name__ == "__main__":
    main()
