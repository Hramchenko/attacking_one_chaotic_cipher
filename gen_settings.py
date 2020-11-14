import sys
from math import floor

class SettingsFile:
  
  def __init__(self, filename):
    self.filename = filename
    self.file = open(filename, "w")
    self.lines = []
  
  def line(self, line):
    self.lines.append(line)
    
  def size(self):
    return len(self.lines)
  
  def save(self):
    count = len(self.lines)
    self.file.write(str(IC) + " "  + str(count) + " " + str(count)+ "\n")
    for line in self.lines:
      self.file.write(line)
    self.file.close()

if len(sys.argv) != 3:
  print "Usage: " + sys.argv[0] + " <secret_key_file> <settings_files_prefix>" 
  sys.exit(0)
key_file = open(sys.argv[1], "r")
settings_files = []
for i in range (0, 4):
 settings_files.append(SettingsFile(sys.argv[2] + str(i)))
 
key = []
IC = 0
for line in key_file.readlines():
  line = line.strip()
  if len(line):
    skey = int(line, 16)
    IC += float(skey)/256
    key.append(skey)
key_file.close()

IC -= floor(IC)

Y = int(IC*100.0)

print "Key#, filename, lines"

def out_vals(Y):
#  print Y
  B = Y
  N = Y%4
  SK = key[B]
  key_idx = ""
  for i in range(0, B):
    settings_files[N].line("%.2x %d" % (SK, N) + "\n")
  idx = settings_files[N].size() - B
  print "| %.2d | %s | %d - %d |" % (B, settings_files[N].filename, idx, idx + B - 1) 
    
    

for i in range(0, 16):
  Y = (5*Y + 1)%16
  out_vals(Y)


for sf in settings_files:
  sf.save()
 
