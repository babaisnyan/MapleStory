import os
from pydub import AudioSegment


def convert_to_wav(source_folder):
    for root, dirs, files in os.walk(source_folder):
        for file in files:
            if file.endswith(".mp3"):
                file_path = os.path.join(root, file)
                file_name = os.path.splitext(file)[0]
                new_file_path = os.path.join(root, file_name + ".wav")

                try:
                    audio = AudioSegment.from_file(file_path)
                    audio.export(
                        new_file_path,
                        format="wav",
                        parameters=["-ac", "2", "-ar", "44100", "-acodec", "pcm_s16le"],
                    )
                    os.remove(file_path)
                except:
                    print(f"Error converting {file_path}")


source_folder = "O:\\Unreal Projects\\MapleStory\\Tools\\SoundExporter\\bin\\Release\\net8.0\\Exported"
convert_to_wav(source_folder)
