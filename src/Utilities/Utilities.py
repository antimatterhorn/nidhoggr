from nidhoggr import SiloMeshWriter2d

class TextMicrophone:
    def __init__(self, nodeList, grid,i,j,filename,workCycle=1):
        self.nodeList = nodeList
        self.cycle = workCycle
        self.grid = grid
        self.i = i
        self.j = j
        self.phi = self.nodeList.getFieldDouble("phi")
        self.filename = filename
        print("Wave Equation microphone initialized at %d,%d"%(i,j))
    def __call__(self,cycle,time,dt):
        phi = self.nodeList.getFieldDouble("phi")
        value = phi[self.grid.index(self.i,self.j,0)]
        with open(self.filename, 'a') as f:
            f.write(f"{time},{value}\n")

class Microphone:
    def __init__(self, nodeList, grid, i, j, filename, gain=1.0,workCycle=1):
        self.nodeList = nodeList
        self.grid = grid
        self.i = i
        self.j = j
        self.filename = filename
        self.gain = gain
        self.cycle = workCycle
        with open(filename, 'wb') as f:  # Create the file in binary write mode
            self.write_wav_header(f)  # Write the header information

    def __call__(self, cycle, time, dt):
        phi = self.nodeList.getFieldDouble("phi")
        value = phi[self.grid.index(self.i, self.j, 0)]
        if self.gain*abs(value) > 1:
            self.gain = 0.99/abs(value)
        value = self.gain*value
        # Normalize the value from the range -1 to 1 to the range 0 to 255
        normalized_value = int(((value + 1) / 2) * 255)
        with open(self.filename, 'ab') as f:  # Append to the file in binary write mode
            data = normalized_value.to_bytes(1, 'big')  # Convert value to 8-bit PCM
            f.write(data)  # Write the audio data

    def write_wav_header(self, f):
        # WAV header format
        f.write(b'RIFF')  # ChunkID
        f.write(b'\x00\x00\x00\x00')  # Placeholder for ChunkSize
        f.write(b'WAVE')  # Format
        f.write(b'fmt ')  # Subchunk1ID
        f.write(int.to_bytes(16, 4, 'little'))  # Subchunk1Size (16 for PCM)
        f.write(int.to_bytes(1, 2, 'little'))  # AudioFormat (1 for PCM)
        f.write(int.to_bytes(1, 2, 'little'))  # NumChannels (1 for mono)
        f.write(int.to_bytes(44100, 4, 'little'))  # SampleRate
        f.write(int.to_bytes(44100 * 1 * 1, 4, 'little'))  # ByteRate (SampleRate * NumChannels * BitsPerSample/8)
        f.write(int.to_bytes(1 * 1, 2, 'little'))  # BlockAlign (NumChannels * BitsPerSample/8)
        f.write(int.to_bytes(8, 2, 'little'))  # BitsPerSample (8 bits)
        f.write(b'data')  # Subchunk2ID
        f.write(b'\x00\x00\x00\x00')  # Placeholder for Subchunk2Size

    # Later update the header with actual sizes
    def update_wav_header(self):
        import os
        file_size = os.path.getsize(self.filename)
        with open(self.filename, 'r+b') as f:
            f.seek(4)
            f.write(int.to_bytes(file_size - 8, 4, 'little'))  # Update ChunkSize
            f.seek(40)
            f.write(int.to_bytes(file_size - 44, 4, 'little'))  # Update Subchunk2Size


class SiloDump:
    def __init__(self,baseName,nodeList,fieldNames,dumpCycle=10):
        self.meshWriter = SiloMeshWriter2d(baseName="waveBox",nodeList=nodeList,fieldNames=["phi","xi"])
        self.cycle = dumpCycle
    def __call__(self,cycle,time,dt):
        self.meshWriter.write("-cycle=%03d.silo"%(cycle))