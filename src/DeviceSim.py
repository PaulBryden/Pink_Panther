import json
import random
import sys
import urllib
import urllib2
from thread import start_new_thread


def printUsage():
    print "Usage:: DeviceSim.py [URL] [SITENAME] [NUMOFDEVICES]"
    exit(0)


def createUser(ID, SITE):
    while (True):
        data = {
            'ID': ID,
            'Site': SITE,
            'X': 71,  # random.randint(1,20),
            'Y': 10,  # random.randint(1,20),
            'Z': random.randint(1, 10),
        }

        req = urllib2.Request(sys.argv[1])
        req.add_header('Content-Type', 'application/json')

        response = urllib2.urlopen(req, json.dumps(data))


if (len(sys.argv) < 4 or len(sys.argv) > 4):
    printUsage()

if (urllib.urlopen(sys.argv[1]).getcode() != 200):
    print "Could Not Reach " + sys.argv[1]
    printUsage()

if (not isinstance(int(sys.argv[3]), int)):
    printUsage()

for x in range(0, int(sys.argv[3])):
    start_new_thread(createUser("USER" + str(x), str(sys.argv[2])))

try:
    raw_input("Press Enter to continue...")
except SyntaxError:
    pass
