# USING MESSAGE QUEUE to SAVE INFORMATION STUDENT
- Create a struct message (information student)
- Show MENU in terminal:
    01-Enter information from keyboard
    02-Show information through ID
    03-Show full studen
    04-Delete information through ID
    05-Exit
- 4 step to use Message queue:
  + open: using mq_open()
  + using: mq_send() and mq_receive()
  + close: mq_close() when not use anymore
  + free: mq_unlink() to remove it out /dev/mqueue/
