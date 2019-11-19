import os
import time

def child():
    for i in range(5):
        print("child # %d %d" %  (os.getpid(), i))
        time.sleep(1)

def parent():
    pid_list = []
    while True:
        newpid = os.fork()
        if newpid == 0:
            child()
        else:
            pid_list.append(newpid)
            print("parent %d child: %d " % (os.getpid(), newpid))

        reply = input("(n) new child, anything else to exit")
        if reply == 'n':
            continue
        else:
            break

parent()

for pid in pid_list:
    print(pid)




