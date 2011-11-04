#include "../src/AttestationIMVPolicyManager.h"
#include <stdio.h>
#include <vector>

/* log4cxx includes */
#include "log4cxx/logger.h"
using namespace log4cxx;
using namespace std;

/* our logger */
static LoggerPtr
logger(Logger::getLogger("AttestationIMV.Test"));


int main()
{
	LOG4CXX_INFO(logger, "Starting test ...");
	tncfhh::iml::AttestationIMVPolicyManager pm("../etc/tnc/attestationimv.policy");
	tncfhh::iml::PcrEntry pcrEntry(1, std::string("123456789abcdef123456789abcdef123456789a"));
	tncfhh::iml::PcrEntry pcrEntry2;
	pcrEntry2 = pcrEntry;
	tncfhh::iml::PcrEntry pcrEntry3(pcrEntry);



	printf("%p\n", pcrEntry.value);
	printf("%p\n", pcrEntry2.value);
	printf("%p\n", pcrEntry3.value);

	pcrEntry2.value[0] = 2;
	pcrEntry3.value[0] = 5;

	printf("%02x\n", pcrEntry.value[0]);
	printf("%02x\n", pcrEntry2.value[0]);
	printf("%02x\n", pcrEntry3.value[0]);


	return 0;
}
