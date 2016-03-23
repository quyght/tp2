#ifndef PROTOCOL_H
#define PROTOCOL_H

typedef enum PROTOCOL_MESSAGE p_msg;
enum PROTOCOL_MESSAGE
{
  BEGIN,  //1
  CLOSE,//2
  ACK,//3
  REQ,//4
  WAIT,//5
  END,//6
  REFUSE,//7
  INIT//8
};

#endif
