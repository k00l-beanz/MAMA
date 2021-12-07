# Maximillian, Austin, Mohammad, Abdullah - MAMA

## General Commands
help : help [COMMAND] <br>
version : version <br>
shutdown : shutdown <br>
clear <br>
alias : alias [ALIAS] [COMMAND] <br>

## R1 - The User Interface
getdate : getdate <br>
gettime : gettime <br>
setdate : setdate [MONTH].[DAY].[YEAR] - Ex: setdate 12.9.21 <br>
settime : settime [HOUR].[MINUTE].[SECOND] - Ex: settime 11.30.00 <br>

## R2 - Process Control Blocks and Queues
suspendpcb : suspendpcb [PCB_NAME] <br>
resumepcb : resumepcb [PCB_NAME] <br>
setprioritypcb : setprioritypcb [PCB_NAME].[NEW_PRIORITY] <br>
showpcb : showpcb [PCB_NAME] <br>
showallpcb : showallpcb <br>
showreadypcb : showreadypcb <br>
showblockedpcb : showblockedpcb <br>
resumeallpcb : resumeallpcb <br>

## R3 + R4 - Dispatching
loadr3 : loadr3 <br>
setalarm : setalarm [HOUR]:[MINUTE],[MESSAGE] - Ex: setalarm 1:00,cs-450_class! <br>
showalarms : showalarms <br>
freealarm : freealarm [HOUR]:[MINUTE] <br>

## R5 - Memory Management
showalloc : showalloc <br>
showfree : showfree <br>
isempty : isempty <br>
