static unsigned int timings[67];
unsigned int receivedBit, receivedBitlength, receivedDelay;
unsigned int bitAvailible;
#define tolerance 1000
#define multiplier 5
int deserialize(int *i)
{
    int i2 = *i; 
    int checksum = i2 & 0b1111;
    i2 >>= 4;
    int i3 = i2;
    int ones = 0;
    while(i3 > 0)
    {
        if(i3 & 1) ones++;
        i3 >>= 1;
    }
    *i = i2;
    return ones == checksum ? 1 : 0;
}
char* convert_bin(long i)
{
  int cnt = 0;
  long icopy = i;
  while (icopy != 0) 
  {
    cnt++;
    icopy >>= 1;
  }
  char *c = (char*)malloc(cnt+1);
  int ind = cnt-1;
  while (i != 0)
  {
    c[ind--] = i & 1 ? '1' : '0';
    i >>= 1;
  }
  c[cnt] = 0x00;
  return c;
}
bool receiveProtocol1(unsigned int changeCount) {
    unsigned long code = 0;
    unsigned long delay = timings[0] / 31;
    unsigned long delayTolerance = delay * tolerance * 0.01;    
    int last = 1, lastb = 0;
    for (int i = 1; i<changeCount ; i=i+2) {
          if (timings[i] < timings[i+1] && abs(timings[i]-timings[i+1]) > 300*multiplier) {
            code = code << 1;
            last = 1;
          } else if (timings[i] > timings[i+1] && abs(timings[i]-timings[i+1]) > 300*multiplier) {
            code+=1;
            code = code << 1;
            last = 0;
          } else if(abs(timings[i]-timings[i+1]) < 300*multiplier) {
            code+=last;
            code = code << 1;
          }
      }
      code = code >> 1;
      if (changeCount > 6) {
        receivedBit = code;
        receivedBitlength = changeCount / 2;
        receivedDelay = delay;
        bitAvailible = 1;
    }

    if (code == 0){
      return false;
    }else if (code != 0){
      return true;
    }
}

void receiveRF()
{
    static unsigned int duration;
    static unsigned int changeCount;
    static unsigned long lastTime;
    static unsigned int repeatCount;

    long time = SysTickCounter;
    duration = time - lastTime;
    //printf("%d-", duration);
    //printf("%d\n", timings[0]);
    if (duration > 5000*multiplier && duration > timings[0] - 200*multiplier && duration < timings[0] + 200*multiplier) {    
      repeatCount++;
      changeCount--;

      if (repeatCount == 2) {
        receiveProtocol1(changeCount);
        repeatCount = 0;
      }
      changeCount = 0;
    } else if (duration > 5000*multiplier) {
      changeCount = 0;
    }

    if (changeCount >= 67) {
      changeCount = 0;
      repeatCount = 0;
    }
    timings[changeCount++] = duration;
    lastTime = time;
}