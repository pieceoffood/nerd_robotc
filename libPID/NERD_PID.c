#include "NERD_PID.h"

//initialize PID structure, set parameters
//
//arguments: pointer to PID structure, kP constant, kI constant, kD constant, 
//             Epsilon constant (range of set point in which to stop summing for I component)
void 
pidInit(PID pPID, float fKP, float fKI, float fKD, float fEpsilon){
	pPID->m_fKP = fKP;
	pPID->m_fKI = fKI;
	pPID->m_fKD = fKD;
	pPID->m_fEpsilon = fEpsilon;
	pPID->m_fSigma = 0;
	pPID->m_fLastValue = 0;
	pPID->m_uliLastTime = nPgmTime;
}

//calculate PID output
//
//returns: output value constrained between -127 and 127
//arguments: pointer to PID structure, set point, sensor value or process variable
float 
pidCalculate(PID pPID, float fSetPoint, float fProcessVariable){
	float fDeltaTime = (float)(nPgmTime - m_uliLastTime) / 1000.0;
	pPID->m_uliLastTime = nPgmTime;

	float fDeltaPV = 0;
	if(fDeltaTime > 0)
		fDeltaPV = (fProcessVariable - pPID->m_fLastValue) / fDeltaTime;

	float fError = fSetPoint - fProcessVariable;

	if(abs(fError) > pPID->m_fEpsilon)
		pPID->m_fSigma += fProcessVariable * fDeltaTime;

	float output = fError * pPID->mfKP 
					+ pPID->m_fSigma * pPID->m_fKI 
					+ fDeltaPV * pPID->m_fKD;

	output = abs(output) > 127 ? abs(output)/output * output : output;
}