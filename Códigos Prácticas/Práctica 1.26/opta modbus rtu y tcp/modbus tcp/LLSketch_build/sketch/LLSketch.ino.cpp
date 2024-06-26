#include <Arduino.h>
#line 1 "C:\\Users\\PCX\\Desktop\\trst\\LLSketch\\LLSketch.ino"
#include <AlPlc_Opta.h>

/* opta_1.0.3
*/

struct PLCSharedVarsInput_t
{
};
PLCSharedVarsInput_t& PLCIn = (PLCSharedVarsInput_t&)m_PLCSharedVarsInputBuf;

struct PLCSharedVarsOutput_t
{
};
PLCSharedVarsOutput_t& PLCOut = (PLCSharedVarsOutput_t&)m_PLCSharedVarsOutputBuf;


AlPlc AxelPLC(-78720439);

// shared variables can be accessed with PLCIn.varname and PLCOut.varname


// Enable usage of EtherClass, to set static IP address and other
#include <PortentaEthernet.h>
arduino::EthernetClass eth(&m_netInterface);


#line 27 "C:\\Users\\PCX\\Desktop\\trst\\LLSketch\\LLSketch.ino"
void setup();
#line 44 "C:\\Users\\PCX\\Desktop\\trst\\LLSketch\\LLSketch.ino"
void loop();
#line 27 "C:\\Users\\PCX\\Desktop\\trst\\LLSketch\\LLSketch.ino"
void setup()
{

	// Configure static IP address
	IPAddress ip(192, 168, 1, 100);
	IPAddress dns(8, 8, 8, 8);
	IPAddress gateway(192, 168, 1, 1);
	IPAddress subnet(255, 255, 255, 0);
	// If cable is not connected this will block the start of PLC with about 60s of timeout!
	eth.begin(ip, dns, gateway, subnet);


	AxelPLC.InitFileSystem();

	AxelPLC.Run();
}

void loop()
{

}

