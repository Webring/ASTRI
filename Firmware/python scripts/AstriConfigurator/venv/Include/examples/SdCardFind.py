import os
#diskLetter = "E"
#print(os.popen("fsutil fsinfo drives").read().replace("\\", "").replace(" ", "").split(":")[1:-1]) # получения всех букв дисков
#print(os.popen(f"vol {diskLetter}:").read().split("\n")[0].split()[-1]) # Получение имени диска, по букве
def sdCardCheck():
    for diskLetter in os.popen("fsutil fsinfo drives").read().replace("\\", "").replace(" ", "").split(":")[1:-1]:
        try:
            if os.popen(f"vol {diskLetter}:").read().split("\n")[0].split()[-1] == "ASTRIOS":
                return diskLetter + ":\\"
        except:
            pass
    return 1
if __name__ == '__main__':
    print(sdCardCheck())