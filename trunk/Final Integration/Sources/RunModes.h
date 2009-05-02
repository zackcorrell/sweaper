/*
************************************************************************************
** SWEAPER
** Authors: Zachary Correll, David Sternberg, John Burkett
** EE 403W SP 09
** Pennsylvania State University
** Last modified: 4/26/09 
**
** RunModes.h
************************************************************************************     
*/

#ifndef RunModes_H        /*prevent duplicated includes*/
#define RunModes_H


#include "derivative.h" /* include peripheral declarations */
#include "ICS.h"
#include "target.h"    
#include "sci_drv.h"
#include "MC9S08QE128.h"

void NormalRun(void);
void SleepRun(void);

#endif /* RunModes_H */