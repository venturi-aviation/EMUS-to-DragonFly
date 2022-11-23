#include <SPI.h> //Library for using SPI Communication 
#include <mcp2515.h> //Library for using CAN Communication
//#include <FlexCAN_T4.h>



struct can_frame msg1;  // For receiving the message

struct can_frame msg; // For sending the message 
struct can_frame msg2;
struct can_frame msg3;
struct can_frame msg4;

//CAN_message_t msg1;
MCP2515 mcp2515(10); // SPI CS Pin 10 

// Variable declaration

   bool Operational ;
    bool Errorpresent;
    //bool SAFAovervoltage = (if (msg1.data[1]==1));
    bool Regencharging ;
    bool Chargingmode ;
    bool Balancingmode ;

        //2. SAFA
    bool SAFAovervoltage ;
    bool SAFAundervoltage ;
    bool SAFAovertemp ;
    bool SAFAundertemp ;
    bool SAFAchargecurrent ;
    bool SAFAtasktimeout ;
    bool SAFAwatchdogtimeout ;
        //3. ERRA
    bool ERRovervoltage ;
    bool ERRundervoltage ;
    bool ERRovertemp ;
    bool ERRundertemp ;
    bool ERRovertempbalancing ;
    bool ERRovertemppcb ;
    bool ERRundertemppcb ;
    bool ERRdischargepeakcurrent ;

      //4. ERRB
    bool ERRchargepeakcurrent ;
    bool ERRdischargecontinouscurrent ;
    bool ERRchargecontinuouscurrent ;
    bool ERRdisbalancevoltagebetweencells ;
    bool ERRdisbalancetemp ;
      //bit 5 not used
    bool ERRcommtimeout;
    bool ERRwatchdogtriggered ;
    
    //5. ERRC
    bool ERRtasktimeout ;
    bool ERRincorrectcustomerconfiguration ;
    bool ERRincorrectfactoryconfiguration ;

    //6. ERRD
    bool ERRhvil ;
    bool ERRhvildevice ;
    bool ERRinsulationp ;
    bool ERRinsulationn ;

    //7. COND

    bool ERRnoinput1;                            // Bit 0 – no input 1
    bool ERRnoinitialmodulecomm ;                // Bit 1 – no initial module communication
    bool ERRnoinitialhvilmeasurement ;           // Bit 2 – no initial HVIL measurement (HV BC ONLY)
    bool ERRnoinitialinsulationmeasurement;    

    int data1 ;
    int data2 ;
    int data3 ;
    int data4  ;
    int data01 ;
    int data02 ;
    int dataVmin;
    int dataVmax;
    int SOC ;
    int TMIN;
    int TMAX;



//
int eventEntryNumber = 0 ;

void setup() {
  SPI.begin();   //Begins SPI communication
  Serial.begin(9600); //Begins Serial Communication at 9600 baud rate 
  mcp2515.reset();                          
  mcp2515.setBitrate(CAN_500KBPS,MCP_8MHZ); //Sets CAN at speed 500KBPS and Clock 8MHz 
  mcp2515.setNormalMode();  //Sets CAN at normal mode
}

// Define base id
int base = 437;  // Define the base id of the EMUS BMS, but look if int accepts hexa decimal number

void loop(){

  /////////////////////////  For Dragonfly
  msg.can_id = 0x687; //CAN id as 0x036, 0x687
  msg.can_dlc = 8; //CAN data length as 8
  msg2.can_id = 0x687; //CAN id as 0x036, 0x687
  msg2.can_dlc = 8; //CAN data length as 8
  msg3.can_id = 0x687; //CAN id as 0x036, 0x687
  msg3.can_dlc = 8; //CAN data length as 8
  msg4.can_id = 0x687; //CAN id as 0x036, 0x687
  msg4.can_dlc = 8; //CAN data length as 8

  ////////////////////////////
 //if ((mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) && (canMsg.can_id == 0x036))
//if ((mcp2515.readMessage(&msg1) == MCP2515::ERROR_OK) && (msg1.can_id == 0+33))     // 0x19B5 second Master Base Id
//if ((mcp2515.readMessage(&msg1) == MCP2515::ERROR_OK) && (msg1.can_id == 0x01B5))
/*
if ((mcp2515.readMessage(&msg1) == MCP2515::ERROR_OK))

 {
     //int x = canMsg.data[0];
     //int m = bitRead(canMsg.data[1],0);
     Serial.print("ID: ");
     Serial.print(msg1.can_id, HEX);
     Serial.print(", ");
     Serial.print("Data: ");
     for (int i =0; i< 8; i++)
     {
       Serial.print(msg1.data[i]);
       Serial.print(" ");
     } 
     Serial.println(" ");
 }

*/

 //Scanning Temperature and Voltage of each cell in the pack and storing into the Variables,
/*
 Get the length of the Temperature sensor

 int temp_length = ;

 int temp[temp_length];

 if ((mcp2515.readMessage(&msg1) == MCP2515::ERROR_OK) && (msg1.can_id == base+33)) 
{

    for (int i =0; i< temp_length; i++)
    {
      for (int j = 0; j< 8;j++)
      {
        temp[i+j] =msg1.data[j];
      }
      
      
    }

}
 for (int i =0;i)
*/

// Storing the Error Response in the designated Variables.
// Checking the operational status
// All the status here are hard coded just because, it's not one to one co-relation between EMUS BMS and ELEO BMS, they have different architecture.

if ((mcp2515.readMessage(&msg1)== MCP2515::ERROR_OK) && (msg1.can_id == base+133))
{
    Operational = ((msg1.data[1]==1));
    Errorpresent = (((msg1.data[1] ==  2) || (msg1.data[1]== 14)) );  // combining multiple scenario, look into EMUS data sheet
    //bool SAFAovervoltage = (if (msg1.data[1]==1));
    Regencharging = 0;
    Chargingmode = 0;
    Balancingmode =0;

        //2. SAFA
    SAFAovervoltage = 0;
    SAFAundervoltage = 0;
    SAFAovertemp = 0;
    SAFAundertemp = 0;
    SAFAchargecurrent = 0;
    SAFAtasktimeout = 0;
    SAFAwatchdogtimeout = 0;
        //3. ERRA
    ERRovervoltage = 0;
    ERRundervoltage = 0;
    ERRovertemp = 0;
    ERRundertemp = 0;
    ERRovertempbalancing = 0;
    ERRovertemppcb = 0;
    ERRundertemppcb = 0;
    ERRdischargepeakcurrent = 0;

      //4. ERRB
    ERRchargepeakcurrent = 0;
    ERRdischargecontinouscurrent = 0;
    ERRchargecontinuouscurrent = 0;
    ERRdisbalancevoltagebetweencells = 0;
    ERRdisbalancetemp = 0;
      //bit 5 not used
    ERRcommtimeout = 0;
    ERRwatchdogtriggered = 0;
    
    //5. ERRC
    ERRtasktimeout = 0;
    ERRincorrectcustomerconfiguration = 0;
    ERRincorrectfactoryconfiguration = 0;

    //6. ERRD
    ERRhvil = 0;
    ERRhvildevice = 0;
    ERRinsulationp = 0;
    ERRinsulationn = 0;
    // 7.  COND
    ERRnoinput1 = 0;                            // Bit 0 – no input 1
    ERRnoinitialmodulecomm = 0 ;                // Bit 1 – no initial module communication
    ERRnoinitialhvilmeasurement =0 ;           // Bit 2 – no initial HVIL measurement (HV BC ONLY)
    ERRnoinitialinsulationmeasurement = 0;

}


// Vmin and Vmax Tmin and Tmax 
if ((mcp2515.readMessage(&msg1)== MCP2515::ERROR_OK) && (msg1.can_id == (438)))
//if ((mcp2515.readMessage(&msg1)== MCP2515::ERROR_OK) && (msg1.can_id == 0x1B6))
{
    data1 = msg1.data[3];  
    data2 = msg1.data[4];
    data3 = msg1.data[5];
    data4 = msg1.data[6];

double Vmin = msg1.data[0]/100.00 +2.00 ;
    dataVmin = msg1.data[0];
double Vmax = msg1.data[1]/100.00 + 2.00 ;
    dataVmax = msg1.data[1];
//double DCTOT = ((msg1.data[3]<<24) + (msg1.data[4]<<16) + (msg1.data[5]<<8) + (msg1.data[6]))*0.01; 
double DCTOT = ((msg1.data[3]<<8) + (msg1.data[4]))*0.01; 



// Printing to observe the result

Serial.println(" Printing data from the base+1 id: Printing Vmin and Vmax");
Serial.println(Vmin);
Serial.println(Vmax);
Serial.println(DCTOT);
/*
Serial.println("Debugging DC total byte3 byte4 byte5 byte6:");
Serial.println(msg1.data[3]);
Serial.println(msg1.data[4]);
Serial.println(msg1.data[5]);
Serial.println(msg1.data[6]);  */
Serial.println("End of data srings from the base+1 id ");

}
// Temperature measure
if ((mcp2515.readMessage(&msg1)== MCP2515::ERROR_OK) && (msg1.can_id == (439)))  // cell module temperature 
{
int Tmin = msg1.data[0] -100 ;   // Cell Module minimum temperature
int Tmax = msg1.data[1] -100;   // Cell Module Maximum Temperature
TMIN = msg1.data[0] ;
TMAX = msg1.data[1];
Serial.println("Printing data from the base+2 id: Printing Tmin and Tmax");
Serial.println(Tmin);
Serial.println(Tmax);
Serial.println("End of data srings from the base+2 id ");
}

if ((mcp2515.readMessage(&msg1)== MCP2515::ERROR_OK) && (msg1.can_id == (445)))  // cell temperature 
{
int Tmin_c = msg1.data[0] ;   // Minimum Cell Temperature
int Tmax_c = msg1.data[1] ;   // Maximum Cell Temperature
}

// Writing Current 

if ((mcp2515.readMessage(&msg1)== MCP2515::ERROR_OK) && (msg1.can_id == (442)))  // CURRENT and SOC
{
  float CURRENT = ((msg1.data[0]<<8) + msg1.data[1]);   // MSB in 0 and LSB in 1
  int data01 = msg1.data[0];
  int data02 = msg1.data[1];

  float SOC = msg1.data[6];
  Serial.println("Printing data from the base+5 id:  Printing Current and SOC");
  Serial.println(CURRENT);
  Serial.println(SOC);
  Serial.println("End of data srings from the base+5 id ");    
    
} 

// Writing DragonFly CAN message

  msg.data[0]=0x01;
  bitWrite(msg.data[1],0, Operational);
  bitWrite(msg.data[1],2, Errorpresent);
  bitWrite(msg.data[1],3, Regencharging);
  bitWrite(msg.data[1],4, Chargingmode % 2);
  bitWrite(msg.data[1],5, Chargingmode / 2);
  //bitWrite(msg.data[1],5,);
  bitWrite(msg.data[1],6, Balancingmode % 2);
  bitWrite(msg.data[1],7, Balancingmode / 2);


  bitWrite(msg.data[2],0, SAFAovervoltage);
  bitWrite(msg.data[2],1, SAFAundervoltage );
  bitWrite(msg.data[2],2, SAFAovertemp);
  bitWrite(msg.data[2],3, SAFAundertemp);
  bitWrite(msg.data[2],4, SAFAchargecurrent);
  bitWrite(msg.data[2],5, SAFAtasktimeout);
  bitWrite(msg.data[2],6, SAFAwatchdogtimeout); 

  bitWrite(msg.data[3],0, ERRovervoltage);
  bitWrite(msg.data[3],1, ERRundervoltage );
  bitWrite(msg.data[3],2, ERRovertemp);
  bitWrite(msg.data[3],3, ERRundertemp);
  bitWrite(msg.data[3],4, ERRovertempbalancing);
  bitWrite(msg.data[3],5, ERRovertemppcb);
  bitWrite(msg.data[3],6, ERRundertemppcb); 
  bitWrite(msg.data[3],6, ERRdischargepeakcurrent); 


  bitWrite(msg.data[4],0, ERRchargepeakcurrent);
  bitWrite(msg.data[4],1, ERRdischargecontinouscurrent );
  bitWrite(msg.data[4],2, ERRchargecontinuouscurrent);
  bitWrite(msg.data[4],3, ERRdisbalancevoltagebetweencells);
  bitWrite(msg.data[4],4, ERRdisbalancetemp);
  //bitWrite(msg.buf[4],5, ERRovertemppcb);
  bitWrite(msg.data[4],6, ERRcommtimeout); 
  bitWrite(msg.data[3],6, ERRwatchdogtriggered);


  bitWrite(msg.data[5],0, ERRtasktimeout);
  bitWrite(msg.data[5],1, ERRincorrectcustomerconfiguration );
  bitWrite(msg.data[5],2, ERRincorrectfactoryconfiguration);

  bitWrite(msg.data[6],0, ERRhvil);
  bitWrite(msg.data[6],1, ERRhvildevice );
  bitWrite(msg.data[6],2, ERRinsulationp);
  bitWrite(msg.data[6],3, ERRinsulationn);

  bitWrite(msg.data[7],0, ERRnoinput1);
  bitWrite(msg.data[7],1, ERRnoinitialmodulecomm );
  bitWrite(msg.data[7],2, ERRnoinitialhvilmeasurement);
  bitWrite(msg.data[7],3, ERRnoinitialinsulationmeasurement);  

  msg2.data[0]=0x03;
  msg2.data[1]= SOC ;
  msg2.data[2]= 0;    // Setting the MSB to zero as EMUS just publish in single byte compare to Elleo
  msg2.data[3]= dataVmin ;
  msg2.data[4] = 0 ;  // Setting the MSB to zero as EMUS just publish in single byte compare to Elleo
  msg2.data[5] = dataVmax;

  // Encoding Finish
  msg2.data[6]= TMIN;
  msg2.data[7]= TMAX;
  //msg2.data[6]= TMIN;
  //msg2.data[7]= TMAX;

  msg3.data[0]=0x04;
  // Writing the DC total 
  //msg3.data[1] = data1;
  //msg3.data[2] = data2;
  //msg3.data[3] = data3;
  //msg3.data[4] = data4;

  msg3.data[1] = 101;
  msg3.data[2] = 456;
  msg3.data[3] = 32;
  msg3.data[4] = 9;


  msg4.data[0]=0x05;

  msg4.data[1] =0 ;
  msg4.data[2] = 0;
  msg4.data[3] = data01 ;
  msg4.data[4] = data02 ;

  // Writing the Current here

///   Sending all the messgaes

  mcp2515.sendMessage(&msg); //Sends the CAN message
  Serial.println("Message 1 Sent Successfully");
  delay(100);
  mcp2515.sendMessage(&msg2); //Sends the CAN message
   Serial.println("Message 2 Sent Successfully");
  delay(100);
  mcp2515.sendMessage(&msg3); //Sends the CAN message
   Serial.println("Message 3 Sent Successfully");
  delay(100);
  mcp2515.sendMessage(&msg4); //Sends the CAN message
   Serial.println("Message 4 Sent Successfully");
  delay(1000);    

  // Here in the code the problem seems to be that the Code is just sending whatever the data it has recorded and not updating always
  // ides should be to make a function call before sending the message with all new updated data. 
   
}