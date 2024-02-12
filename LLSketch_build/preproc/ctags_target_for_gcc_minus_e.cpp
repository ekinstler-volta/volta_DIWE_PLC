# 1 "C:\\Users\\ErikKinstler\\Downloads\\Test_Stand_HMI\\Test_Stand_HMI\\LLSketch\\LLSketch.ino"
# 2 "C:\\Users\\ErikKinstler\\Downloads\\Test_Stand_HMI\\Test_Stand_HMI\\LLSketch\\LLSketch.ino" 2

/* pmc_1.0.3
*/

struct PLCSharedVarsInput_t
{
 float in_EC;
};
PLCSharedVarsInput_t& PLCIn = (PLCSharedVarsInput_t&)m_PLCSharedVarsInputBuf;

struct PLCSharedVarsOutput_t
{
};
PLCSharedVarsOutput_t& PLCOut = (PLCSharedVarsOutput_t&)m_PLCSharedVarsOutputBuf;


AlPlc AxelPLC(1955367127);

# 21 "C:\\Users\\ErikKinstler\\Downloads\\Test_Stand_HMI\\Test_Stand_HMI\\LLSketch\\LLSketch.ino" 2

Ezo_board EC = Ezo_board(100, "EC");

char EC_data[32];
char *EC_str;

unsigned long preMills, delay1, delay2;
int delaySeq;

void step1();
void step2();
void i2c_nodelay_sequencer(unsigned long* preMills, unsigned long delay1, unsigned long delay2, int* delaySeq);

void setup() {
  Wire.begin(); // enable I2C port.
  delay1 = 1000;
  delay2 = 600;
  delaySeq = 0;
  delay(3000);
  preMills = millis();

 AxelPLC.Run();
}

void loop() {
    i2c_nodelay_sequencer(&preMills, delay1, delay2, &delaySeq);
}

void step1() {
    EC.send_cmd("r");
}

void step2() {
    //put the response into the buffer
    EC.receive_cmd(EC_data, 8);
    // Parse string at commas
    EC_str = strtok(EC_data, ",");
    // Send data to PLC
    PLCIn.in_EC = atof(EC_str);
}

void i2c_nodelay_sequencer(unsigned long* preMills, unsigned long delay1, unsigned long delay2, int* delaySeq) {
 unsigned long curMills = millis();
 if(*delaySeq == 0){
     if (curMills - *preMills >= delay1){
         step1();
         *delaySeq += 1;
         *preMills = millis();
     }
 }
 else if(*delaySeq == 1){
     if (curMills - *preMills >= delay2){
         step2();
         *delaySeq = 0;
         *preMills = millis();
     }
 }
 return;
}
