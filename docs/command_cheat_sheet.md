# Maximillian, Austin, Mohammad, Abdullah - MAMA

## General Commands
help : help [COMMAND]
version : version
shutdown : shutdown
clear
alias : alias [ALIAS] [COMMAND]

## R1 - The User Interface
getdate : getdate
gettime : gettime
setdate : setdate [MONTH].[DAY].[YEAR] - Ex: setdate 12.9.21
settime : settime [HOUR].[MINUTE].[SECOND] - Ex: settime 11.30.00

## R2 - Process Control Blocks and Queues
suspendpcb : suspendpcb [PCB_NAME]
resumepcb : resumepcb [PCB_NAME]
setprioritypcb : setprioritypcb [PCB_NAME].[NEW_PRIORITY]
showpcb : showpcb [PCB_NAME]
showallpcb : showallpcb
showreadypcb : showreadypcb
showblockedpcb : showblockedpcb
resumeallpcb : resumeallpcb

## R3 + R4 - Dispatching
loadr3 : loadr3
setalarm : setalarm [HOUR]:[MINUTE],[MESSAGE] - Ex: setalarm 1:00,cs-450_class!
showalarms : showalarms
freealarm : freealarm [HOUR]:[MINUTE]

## R5 - Memory Management
showalloc : showalloc
showfree : showfree
isempty : isempty
