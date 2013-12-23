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

class Task:
    def __init__(self):
        self.min = [0] * MAX_MIN
        self.hour = [0] * MAX_HOUR
        self.date = [0] * MAX_DOM #day of month
        self.month = [0] * MAX_MONTH
        self.dow = [0] * MAX_DOW
        self.cmd = ""
        
    def debug(self):
            print self.min
            print self.hour
            print self.date
            print self.month
            print self.dow
            print "cmd is " , self.cmd
        
def parseLine(line):
    if cmp(line[0] , "#") == 0:
        return
    print "The task is: ", line
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
    parseTime(task.date, dateStr, 0, MAX_DOM - 1)
    parseTime(task.month, monthStr, 0, MAX_MONTH - 1)
    parseTime(task.dow, dowStr, 0, MAX_DOW - 1)
    task.cmd = cmd
    
    return task
    
# @param data: 
#     such asTask.min, Task.hour etc
# @param str:
#     The time to be parsed
# @param low:
#    lower bound the the item, such as, dow's low is 1 while min's low is 0
def parseTime(data, time, low, high):
    ranges = time.split(",")
    for range_ in ranges:
        # handle like 23-7/2
        repeat = -1
        frm = -1
        to = -1
        if "/" in range_:
            repeat = int(range_[range_.index("/") + 1:])
        if "-" in range_:
            frm = int(range_[:range_.index("-")])
            if "/" in range_:
                to = int(range_[range_.index("-") + 1:range_.index("/")])
            else:
                to = int(range_[range_.index("-"):])
        else:
            # 10 or *
            if cmp(range_[0], "*") == 0:
                frm = to = ANYTIME;
            else:
                frm = to = int(range_)
        print "repeat = ", repeat , " from = ", frm, " to = ", to
        set_task_bit(data, frm, to, low, high, repeat)
    # for range_ in ranges END 
    
    print
    

def set_task_bit(data, frm, to, low, high, repeat):
    if frm == ANYTIME and to == ANYTIME:
        frm = low
        to = high
    if repeat == -1:
        repeat = 1;
        
    # printf("from=%d to=%d low=%d high=%d repeat=%d\n",from,to,low,high,repeat);
    i = 0
    tmp = 0
    f_g_t = frm > to; # frm is greater than to
    if f_g_t:
        # such as 21 - 7 / 2
        for i in range(frm - low, high - low + 1, repeat):
            #fill 21, 23
            data[i] = 1;
        tmp = i - (high - low + 1)
        for i in range(tmp, to - low + 1, repeat):
            #fill 1, 2, 3, 5, 7
            data[i] = 1;
    else:
        for i in range(frm - low, to - low + 1, repeat):
            data[i] = 1;


f = open('cron.cfg', 'r')
for line in  f.readlines():
    task = parseLine(line.rstrip())
    if not task == None:
        task.debug()
