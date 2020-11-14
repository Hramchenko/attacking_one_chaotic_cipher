from commands import *
import sys

if len(sys.argv) != 5:
  print "Usage: " + sys.argv[0] + " <original_gamma> <prefix> <first> <log_file>" 
  sys.exit(0)
cipher = open(sys.argv[1], "r")
log = open(sys.argv[4], "w")

def log_and_print(print_log_str):
  log.write(print_log_str + "\n")
  print (print_log_str)

index = 0
prefix = sys.argv[2]
first = int(sys.argv[3])

if first == 0:
    process_cmd = "./Release/chaos_nvidia -t ./" + prefix + "0" 
    log_and_print(process_cmd)
    log_and_print (getoutput(process_cmd))
  

for line in cipher.readlines():
  if index >= first + 3:
    #print(line)
    components = line.split(" ")
    r = ""
    clear_components = []
    for com in components:
      com = com.strip()
      if len(com):
        clear_components.append(com) 
        #print "|" + com + "|"
    log_and_print( "---------------------------------------")
    #print clear_components[1] + " " + clear_components[3] + " " + clear_components[4]
    process_cmd = "./Release/chaos_nvidia -n " + clear_components[3] + " -p " + clear_components[4] + " " + prefix + str(index - 3) + " " + prefix + str(index - 2) 
    log_and_print(process_cmd)
    log_and_print (getoutput(process_cmd))

    check_cmd = "./Release/chaos_nvidia -e " + clear_components[1] + " " +  prefix + str(index - 2) 
    log_and_print(check_cmd)
    log_and_print(getoutput(check_cmd))
  
    #keys_cmd = "./Release/chaos_nvidia -n " + clear_components[3] + " -y " + clear_components[4] + " " + prefix + str(index - 3) + " " + prefix + str(index - 3) + "_keys"  
    #log_and_print(keys_cmd)
    #log_and_print(getoutput(keys_cmd))

  index += 1

cipher.close()
log.close()

