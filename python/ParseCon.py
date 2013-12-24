'''
Created on 2013-12-23

@author: ZJ
'''

MAX_MIN = 60
MAX_HOUR = 24
MAX_DOM = 31
MAX_MONTH = 12
MAX_DOW = 7

ANYTIME = 0xff

# low means the lower bound of some attributes
# such as the minimum value of minute and hour and day of week is 0
# while the minimum value of day fo month and month is 1
def printTime(msg,attri,low,high):
    print msg
    for i in range(0,high):
        if attri[i] == 1:
            print i + low,
    print

def parseLine(line):
    if cmp(line[0] , "#") == 0:
        return
    splitedStr = line.split(" ", 5)
    minStr = splitedStr[0]
    hourStr = splitedStr[1]
    dateStr = splitedStr[2]
    monthStr = splitedStr[3]
    dowStr = splitedStr[4]
    cmd = splitedStr[5]
    
    task = Task()

    parseTime(task.min, minStr, 0, MAX_MIN - 1)
    parseTime(task.hour, hourStr, 0, MAX_HOUR - 1)
    parseTime(task.date, dateStr, 1, MAX_DOM)
    parseTime(task.month, monthStr, 1, MAX_MONTH)
    parseTime(task.dow, dowStr, 0, MAX_DOW - 1)
    task.cmd = cmd
    
    return task
    
# @param attr: 
#     the attribute such as Task.min, Task.hour etc
# @param str:
#     The time to be parsed, such as 23-7/2, 1-3, *, */3 etc
# @param low:
#    lower bound the the attribute, such as, dow's low is 1 while min's low is 0
# @param high:
#    upper bound of the attribute
def parseTime(attr, time, low, high):
    ranges = time.split(",")
    for range_ in ranges:
        # handle each condition splited by commar
        repeat = -1
        frm = -1
        to = -1
        slashIndex = range_.find("/")
        hyphenIndex = range_.find("-")
        if slashIndex != -1:
            # get repeat value 2 in 23-7/2
            repeat = int(range_[slashIndex + 1:])
        if hyphenIndex != -1:
            # get the from value 23 in 23-7/2
            frm = int(range_[:hyphenIndex])
            if "/" in range_:
                # get the to value 7 in 23-7/2
                to = int(range_[hyphenIndex + 1:slashIndex])
            else:
                # get the to value 8 in 23-8 when no slash
                to = int(range_[hyphenIndex + 1:])
        else:
            # no hyphen condition such as * or just number
            if cmp(range_[0], "*") == 0:
                frm = to = ANYTIME;
            else:
                frm = to = int(range_)
        # print "repeat = ", repeat , " from = ", frm, " to = ", to
        setAttriValue(attr, frm, to, low, high, repeat)
    # for END 

def setAttriValue(attr, frm, to, low, high, repeat):
    if frm == ANYTIME and to == ANYTIME:
        frm = low
        to = high
    if repeat == -1:
        repeat = 1;

    fromGreaterThanTo = frm > to; # frm is greater than to
    if fromGreaterThanTo:
        # such as 21-7/2
        for i in range(frm - low, high - low + 1, repeat):
            #fill 21, 23
            attr[i] = 1;
        for i in range(i - (high - low + 1), to - low + 1, repeat):
            #fill 1, 3, 5, 7
            attr[i] = 1;
    else:
        # such as 7-21/2
        for i in range(frm - low, to - low + 1, repeat):
            #fill 7,9,11,13 ... 19,21
            attr[i] = 1;

class Task:
    def __init__(self):
        self.min = [0] * MAX_MIN
        self.hour = [0] * MAX_HOUR
        self.date = [0] * MAX_DOM #day of month
        self.month = [0] * MAX_MONTH
        self.dow = [0] * MAX_DOW #day of week
        self.cmd = ""
        
    def debug(self):
        printTime("minute:",self.min,0,MAX_MIN)
        printTime("hour:",self.hour,0,MAX_HOUR)
        printTime("date:",self.date,1,MAX_DOM)
        printTime("month:",self.month,1,MAX_MONTH)
        printTime("day of week:",self.dow,0,MAX_DOW)
        print "cmd is " , self.cmd
        print

# Main function #
f = open('cron.cfg', 'r')
for line in f.readlines():
    line = line.rstrip()
    print "Line is ",line
    task = parseLine(line)
    if not task == None:
        task.debug()
f.close()
