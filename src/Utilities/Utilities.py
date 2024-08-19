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
    def __init__(self, nodeList, grid, i, j, filename, gain=1.0):
        self.nodeList = nodeList
        self.grid = grid
        self.i = i
        self.j = j
        self.filename = filename
        self.gain = gain
        with open(filename, 'wb') as f:  # Create the file in binary write mode
            self.write_wav_header(f)  # Write the header information

    def __call__(self, cycle, time, dt):
        phi = self.nodeList.getFieldDouble("phi")
        value = self.gain*phi[self.grid.index(self.i, self.j, 0)]
        if abs(value) > 1:
            self.gain = 1.0/abs(value)
            value = self.gain*phi[self.grid.index(self.i, self.j, 0)]
        # Normalize the value from the range -1 to 1 to the range 0 to 255
        normalized_value = int(((value + 1) / 2) * 255)

        with open(self.filename, 'ab') as f:  # Append to the file in binary write mode
            data = normalized_value.to_bytes(1, 'big')  # Convert value to 8-bit PCM
            f.write(data)  # Write the audio data

    def write_wav_header(self, f):
        # WAV header format: https://www.sonypictures.com/mgm/tech/wavefile.html
        chunk_id = b'RIFF'
        chunk_size = 36 + (44 * len(self.grid))
        fmt_chunk_id = b'WAVE'
        fmt_chunk_size = 16
        audio_format = 1  # PCM
        num_channels = 1  # Mono
        sample_rate = 44100  # 44 kHz
        byte_rate = sample_rate * num_channels
        block_align = num_channels
        bits_per_sample = 8

        f.write(chunk_id)
        f.write(int.to_bytes(chunk_size, 4, 'big'))
        f.write(fmt_chunk_id)
        f.write(int.to_bytes(fmt_chunk_size, 4, 'big'))
        f.write(int.to_bytes(audio_format, 2, 'big'))
        f.write(int.to_bytes(num_channels, 2, 'big'))
        f.write(int.to_bytes(sample_rate, 4, 'big'))
        f.write(int.to_bytes(byte_rate, 4, 'big'))
        f.write(int.to_bytes(block_align, 2, 'big'))
        f.write(int.to_bytes(bits_per_sample, 2, 'big'))


class SiloDump:
    def __init__(self,baseName,nodeList,fieldNames,dumpCycle=10):
        self.meshWriter = SiloMeshWriter2d(baseName="waveBox",nodeList=nodeList,fieldNames=["phi","xi"])
        self.cycle = dumpCycle
    def __call__(self,cycle,time,dt):
        self.meshWriter.write("-cycle=%03d.silo"%(cycle))