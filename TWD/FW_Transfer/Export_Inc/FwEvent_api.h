/*
 * FwEvent_api.h
 *
 * Copyright(c) 1998 - 2010 Texas Instruments. All rights reserved.      
 * All rights reserved.      
 * 
 * This program and the accompanying materials are made available under the 
 * terms of the Eclipse Public License v1.0 or BSD License which accompanies
 * this distribution. The Eclipse Public License is available at
 * http://www.eclipse.org/legal/epl-v10.html and the BSD License is as below.                                   
 *                                                                       
 * Redistribution and use in source and binary forms, with or without    
 * modification, are permitted provided that the following conditions    
 * are met:                                                              
 *                                                                       
 *  * Redistributions of source code must retain the above copyright     
 *    notice, this list of conditions and the following disclaimer.      
 *  * Redistributions in binary form must reproduce the above copyright  
 *    notice, this list of conditions and the following disclaimer in    
 *    the documentation and/or other materials provided with the         
 *    distribution.                                                      
 *  * Neither the name Texas Instruments nor the names of its            
 *    contributors may be used to endorse or promote products derived    
 *    from this software without specific prior written permission.      
 *                                                                       
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS   
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT     
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT  
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT      
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT   
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


/** \file FwEvent.h
 *  \brief FwEvent api
 *
 *  \see FwEvent.c
 */


#ifndef _FW_EVENT_API_H
#define _FW_EVENT_API_H

/* Public Function Definitions */

/*
 * \brief	Create the FwEvent module object
 * 
 * \param  hOs  - OS module object handle
 * \return Handle to the created object
 * 
 * \par Description
 * Calling this function creates a FwEvent object
 * 
 * \sa fwEvent_Destroy
 */
TI_HANDLE       fwEvent_Create              (TI_HANDLE hOs);


/*
 * \brief	Destroys the FwEvent object
 * 
 * \param  hFwEvent  - The object to free
 * \return TI_OK
 * 
 * \par Description
 * Calling this function destroys a FwEvent object
 * 
 * \sa fwEvent_Create
 */
TI_STATUS       fwEvent_Destroy             (TI_HANDLE hFwEvent);


/*
 * \brief	Requests the context engine to schedule the driver task
 * 
 * \param  hFwEvent  - FwEvent Driver handle
 * \return void
 * 
 * \par Description
 * Called by the FW-Interrupt ISR.
 * Requests the context engine to schedule the driver task 
 * for handling the FW-Events (FwEvent callback).
 * 
 * \sa
 */
void            fwEvent_InterruptRequest    (TI_HANDLE hFwEvent);


/*
 * \brief	Handle the FW interrupts
 * 
 * \param  hFwEvent  - FwEvent Driver handle
 * \return void
 * 
 * \par Description
 * Called from context module upon receiving FW interrupt
 * The function mask the interrupts and reads the FW status
 * 
 * \sa
 */
void            fwEvent_Handle              (TI_HANDLE hFwEvent);


/*
 * \brief	Handle the Fw Status information 
 * 
 * \param  hFwEvent  - FwEvent Driver handle
 * \return void
 * 
 * \par Description
 * This function is called from fwEvent_Handle on a sync read, or from TwIf as a CB on an async read.
 * It calls fwEvent_CallHandler to handle the triggered interrupts.
 * 
 * \sa fwEvent_Handle
 */
void            fwEvent_ReadCompleteCb      (TI_HANDLE hFwEvent);


/*
 * \brief	Config the FwEvent module object
 * 
 * \param  hFwEvent  - FwEvent Driver handle
 * \param  hTWD  - Handle to TWD module
 * \return TI_OK
 * 
 * \par Description
 * From hTWD we extract : hOs, hReport, hTwIf, hContext,
 *      hHealthMonitor, hEventMbox, hCmdMbox, hRxXfer, 
 *      hTxHwQueue, hTxResult
 * In this function we also register the FwEvent to the context engine
 * 
 * \sa
 */
TI_STATUS       fwEvent_Init                (TI_HANDLE hFwEvent, TI_HANDLE hTWD);


/*
 * \brief	Stop & reset FwEvent (called by the driver stop process)
 * 
 * \param  hFwEvent  - FwEvent Driver handle
 * \return TI_OK
 * 
 * \par Description
 *
 * \sa
 */
TI_STATUS       fwEvent_Stop                (TI_HANDLE hFwEvent);


/*
 * \brief	Translate host to FW time (Usec)
 * 
 * \param  hFwEvent  - FwEvent Driver handle
 * \param  uHostTime - The host time in MS to translate
 *
 * \return FW Time in Usec
 * 
 * \par Description
 * 
 * \sa
 */
TI_UINT32       fwEvent_TranslateToFwTime (TI_HANDLE hFwEvent, TI_UINT32 uHostTime);


/*
 * \brief	Disable the FwEvent client of the context thread handler
 * 
 * \param  hFwEvent  - FwEvent Driver handle
 * \return void
 * 
 * \par Description
 *
 * \sa
 */
void            fwEvent_DisableInterrupts   (TI_HANDLE hFwEvent);


/*
 * \brief	Enable the FwEvent client of the context thread handler
 * 
 * \param  hFwEvent  - FwEvent Driver handle
 * \return void
 * 
 * \par Description
 *
 * \sa
 */
void            fwEvent_EnableInterrupts    (TI_HANDLE hFwEvent);


/*
 * \brief	Unmask all interrupts, set Rx interrupt bit and call FwEvent_Handle
 * 
 * \param  hFwEvent  - FwEvent Driver handle
 * \return void
 * 
 * \par Description
 * Called when driver Start or recovery process is completed.
 * Unmask all interrupts, set Rx interrupt bit and call FwEvent_Handle 
 * (in case we missed an Rx interrupt in a recovery process).
 *
 * \sa
 */
void            fwEvent_EnableExternalEvents(TI_HANDLE hFwEvent);


/*
 * \brief	Unmask only cmd-cmplt and events interrupts (needed for init phase)
 * 
 * \param  hFwEvent  - FwEvent Driver handle
 * \return Event mask
 * 
 * \par Description
 * Unmask only cmd-cmplt and events interrupts (needed for init phase)
 * and return interrupt enabled bit mask.
 *
 * \sa
 */
TI_UINT32       fwEvent_GetInitMask         (TI_HANDLE hFwEvent);


/*
 * \brief	Issue a FW status read (Not in response to any FW interrupt)
 * 
 * \param  hFwEvent  - FwEvent Driver handle
 * \return void
 * 
 * \par Description
 * Issue a FW status read (Not in response to any FW interrupt)
 * Called also from fwEvent_EnableExternalEvents after recovery.
 * 
 * \sa fwEvent_EnableExternalEvents
 */
void            fwEvent_GetFwStatus         (TI_HANDLE hFwEvent);



#ifdef TI_DBG

void            fwEvent_PrintStat           (TI_HANDLE hFwEvent);

#endif  /* TI_DBG */



#endif /* _FW_EVENT_API_H */







