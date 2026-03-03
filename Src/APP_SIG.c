/**
 * @file        LibRamp.c
 * @brief       Framework Memory Access Control module.
 * @note        TemplateDetailsDescription.\n
 *
 * @author      mba
 * @date        15/09/2024
 * @version     1.0
 */
// ********************************************************************
// *                      Includes
// ********************************************************************
#include "./APP_SIG.h"
#include "./APP_CTRL/APP_SYS/Src/APP_SYS.h"
#include "APP_CFG/ConfigFiles/APPSIG_ConfigPrivate.h"
#include "APP_CTRL/APP_SDM/Src/APP_SDM.h"

#include "Library/Queue/Src/LIBQueue.h"
#include "Library/SafeMem/SafeMem.h"




// ********************************************************************
// *                      Defines
// ********************************************************************

// ********************************************************************
// *                      Types
// ********************************************************************
/* CAUTION : Automatic generated code section for Enum: Start */

/* CAUTION : Automatic generated code section for Enum: End */

//-----------------------------ENUM TYPES-----------------------------//

/* CAUTION : Automatic generated code section for Structure: Start */

/* CAUTION : Automatic generated code section for Structure: End */
//-----------------------------STRUCT TYPES---------------------------//
///@brief Queue information for
typedef struct 
{
    t_uint32 msgId_u32;                             //---- Id of the message ----//
    t_uint8 data_ua8[APPSIG_DATA_PAYLOAD_LEN];       //---- Payload data ----//
    t_eAPPSIG_MsgOrigin origin_e;                   //---- Message Origin ----//
} t_sAPPSIG_msgPayload;

///@brief Message Information 
typedef struct 
{
    t_uint32 lastTimeSend_u32;          //---- last time we sent the msg ---//
    t_uint32 lastTimeRcv_u32;           //---- last time received ----//
} t_sAPPSIG_MsgTimestamp;

/// @brief Message Information 
typedef struct 
{
    t_sAPPSIG_MsgTimestamp timeStamp_s;
    t_bool forceSend_b;
    t_cbAPPSIG_MsgRcvCallback * rcvCallback_pacb[APPSIG_MSG_RCV_SUBSRIBERS_MAX];
} t_sAPPSIG_MsgInfo;

///@brief siangl information 
typedef struct 
{
    t_float32 value_f32;                                                                    //---- current value of the signal -----//
    t_bool isValid_b;                                                                       //---- Flag to know of the value is valid ----//
    t_bool isRcvOnce_b;                                                                     //---- Flag to know of the value has been at least rcv once ----//
    t_cbAPPSIG_SignalRcvCallback  * rcvCallback_pacb[APPSIG_SIG_RCV_SUBSRIBERS_MAX];        //---- array of callback for notice update signals ----//
} t_sAPPSIG_SignalInfo;
//-----------------------------TYPEDEF TYPES---------------------------//

// ********************************************************************
// *                      Variables
// ********************************************************************
///@brief Module State
static t_eCyclicModState g_AppSig_ModState_e = STATE_CYCLIC_CFG;

///@brief Queue Managment 
t_sLIBQUEUE_QueueCore g_RxSoftQueueMngmt_s;
t_sAPPSIG_msgPayload g_msgPayloadBuffer_as[APPSIG_RX_BUFFER_SIZE];

///@brief Container for all signal value 
t_sAPPSIG_SignalInfo g_signalInfo_as[APPSIG_SIGNAL_NB];

///@brief last time send messages
t_sAPPSIG_MsgInfo g_SrlSMsgInfo_as[APPSIG_SRL_MSG_NB];
t_sAPPSIG_MsgInfo g_CANSMsgInfo_as[APPSIG_CAN_MSG_NB];

///@brief varaible to know the direction of the msg depedning on ecu 
t_eAPPSYS_EcuPos g_EcuId_e = APPSYS_ECU_POS_NB;

///@brief msg signal buffer 
t_eAPPSIG_Signal g_signalIDBufferCb_ae[APPSIG_BUFFER_MSG_SIG_CB];
t_float32 g_signalValBufferCb_af32[APPSIG_BUFFER_MSG_SIG_CB];
/**
*
*	@brief      Configure the Wire Serial Line.
*	@note   	 
*
*
*/
typedef t_eReturnCode (t_cbAPPSIG_SendFrameMsg)(t_sAPPSIG_MsgCfg * f_msgInfo_ps);
//********************************************************************************
//                      Local functions - Prototypes
//********************************************************************************
/**
*
*	@brief      Configure the Wire Serial Line.
*	@note   	 
*
*
*/
static t_eReturnCode s_APPSIG_ConfigurationState(void);

/**
*
*	@brief      Configure the Wire Serial Line.
*	@note   	 
*
*
*/
static t_eReturnCode s_APPSIG_OperationalState(void);
/**
*
*	@brief      Configure the Wire Serial Line.
*	@note   	 
*
*
*/
static t_eReturnCode s_APPSIG_Ope_RxSignalMngmt(void);
/**
*
*	@brief      Configure the Wire Serial Line.
*	@note   	 
*
*
*/
static t_eReturnCode s_APPSIG_FastTask_TxSignalMngmt(void);
/**
*
*	@brief      Configure the Wire Serial Line.
*	@note   	 
*
*
*/
static t_eReturnCode s_APPSIG_Ope_RxDiagnosticMngmt(void);
/**
*
*	@brief      Configure the Wire Serial Line.
*	@note   	 
*
*
*/
static t_eReturnCode s_APPSIG_InitializeSrlGate(void);
/**
*
*	@brief      Configure the Wire Serial Line.
*	@note   	 
*
*
*/
static t_eReturnCode s_APPSIG_InitializeCanGate(void);
/**
*
*	@brief      Configure the Wire Serial Line.
*	@note   	 
*
*
*/
static t_eReturnCode s_APPSIG_FindSignalMapping(t_sAPPSIG_msgPayload f_msgPayload_s, 
                                                t_sAPPSIG_MsgCfg ** f_msgCfg_ps,
                                                t_uint16 * f_msgId_pu16,
                                                t_cbAPPSIG_MsgRcvCallback *** f_msgCallback_pacb);

/**
*
*	@brief      Configure the Wire Serial Line.
*	@note   	 
*
*
*/
static t_eReturnCode s_APPSIG_SendTxMsgMngmt(t_eAPPSIG_MsgOrigin f_msgGate_e);
/**
*
*	@brief      Configure the Wire Serial Line.
*	@note   	 
*
*
*/
static t_eReturnCode s_APPSIG_RxTimeoutMngmt(t_eAPPSIG_MsgOrigin f_msgGate_e);
/**
*
*	@brief      Configure the Wire Serial Line.
*	@note   	 
*
*
*/
static t_eReturnCode s_APPSIG_MsgDecoder(   t_uint8 * f_data_pu8, 
                                            const t_sAPPSIG_MsgCfg * msgCfg_ps);
/**
*
*	@brief      Configure the Wire Serial Line.
*	@note   	 
*
*
*/
static t_eReturnCode s_APPSIG_MsgEncoder(   t_uint8 * f_data_pu8, 
                                            const t_sAPPSIG_MsgCfg * f_msgInfo_ps);
/**
*
*	@brief      Configure the Wire Serial Line.
*	@note   	 
*
*
*/
static t_eReturnCode s_APPSIG_SendSrlFrame(t_sAPPSIG_MsgCfg * f_msgInfo_ps);
/**
*
*	@brief      Configure the Wire Serial Line.
*	@note   	 
*
*
*/
static t_eReturnCode s_APPSIG_SendCanFrame(t_sAPPSIG_MsgCfg * f_msgInfo_ps);
/**
*
*	@brief      Configure the Wire Serial Line.
*	@note   	 
*
*
*/
static void s_APPSIG_FastTask(void);
/**
*
*	@brief      Function call by serial / CAN
*	@note   	 
*
*
*/
static void s_APPSIG_SerialRcvCallback( t_uint8 * f_rxData_pu8, 
                                        t_uint16 f_dataSize_u16, 
                                        t_eFMKSRL_RxCallbackInfo f_InfoCb_e);
static void s_APPSIG_CanRcvCallback(t_eFMKFDCAN_NodeList f_Node_e,
                                    t_sFMKFDCAN_RxItemEvent f_RxItem_s, 
                                    t_eFMKFDCAN_NodeStatus f_NodeStatus_e);
/**
*
*	@brief      Configure the Wire Serial Line.
*	@note   	 
*
*
*/
static t_uint32 s_APPSIG_ExtractRawValue(   const t_uint8 *f_data_pu8,
                                            t_uint8 f_startBit_u8,
                                            t_uint8 f_bitLength_u8,
                                            t_eAPPSIG_SigEncode f_encode_e);
/**
*
*	@brief      Configure the Wire Serial Line.
*	@note   	 
*
*
*/
static void s_APPSIG_InsertRawValue( t_uint8 * f_data_pu8,
                                     t_uint8 f_startBit_u8,
                                     t_uint8 f_bitLength_u8,
                                     t_eAPPSIG_SigEncode f_encode_e,
                                     t_uint32 f_rawValue_u32);
/**
*
*	@brief      Configure the Wire Serial Line.
*	@note   	 
*
*
*/
static t_eReturnCode s_APPSIG_BroadcastUpdate(  t_sAPPSIG_MsgCfg  * f_msgCfg_ps, 
                                                t_uint16 f_msgEnmID_u16,
                                                t_cbAPPSIG_MsgRcvCallback  ** f_rcvMsgCb_pacb);
//********************************************************************************
//                      Public functions - Implementation
//********************************************************************************
//********************************
// APPSIG_Init
//********************************
t_eReturnCode APPSIG_Init(void)
{
    t_eReturnCode Ret_e;
    t_uint16 LLI_u16;
    t_uint8 LLI2_u8;
    t_sLIBQUEUE_QueueCfg queueCfg_s;

    //---- set to 0 buffer ----//
    Ret_e = SafeMem_memclear(   &g_msgPayloadBuffer_as, 
                                (t_uint16)(sizeof(t_sAPPSIG_msgPayload) * APPSIG_RX_BUFFER_SIZE));

    //---- Configure Rx Buffer Queue ----//
    queueCfg_s.bufferHead_pv = (void *)&g_msgPayloadBuffer_as;
    queueCfg_s.bufferSize_u8 = APPSIG_RX_BUFFER_SIZE;
    queueCfg_s.elementSize_u8 = sizeof(t_sAPPSIG_msgPayload);
    queueCfg_s.enableOverwrite_b = False;
    Ret_e = LIBQUEUE_Create(&g_RxSoftQueueMngmt_s, queueCfg_s);

    //---- init signals values ----//
    for(LLI_u16 = (t_uint16)0 ; LLI_u16 < (t_uint16)APPSIG_SIGNAL_NB ; LLI_u16++)
    {
        g_signalInfo_as[LLI_u16].value_f32 = (t_float32)-1.0f;
        g_signalInfo_as[LLI_u16].isRcvOnce_b = FALSE;
        g_signalInfo_as[LLI_u16].isValid_b = FALSE;

        for(LLI2_u8 = (t_uint8)0 ; LLI2_u8 < APPSIG_MSG_RCV_SUBSRIBERS_MAX ; LLI2_u8++)
        {
            g_signalInfo_as[LLI_u16].rcvCallback_pacb[LLI2_u8] = NULL_FUNCTION;
        }
    }

    //---- init serial info ----//
    for(LLI_u16 = (t_uint16)0 ; LLI_u16 < (t_uint16)APPSIG_SRL_MSG_NB ; LLI_u16++)
    {
        g_SrlSMsgInfo_as[LLI_u16].forceSend_b = FALSE;
        g_SrlSMsgInfo_as[LLI_u16].timeStamp_s.lastTimeRcv_u32 = (t_uint32)0;
        g_SrlSMsgInfo_as[LLI_u16].timeStamp_s.lastTimeSend_u32 = (t_uint32)0;

    }

    //---- init CAN timestamp ----//
    for(LLI_u16 = (t_uint16)0 ; LLI_u16 < (t_uint16)APPSIG_CAN_MSG_NB ; LLI_u16++)
    {
        g_CANSMsgInfo_as[LLI_u16].forceSend_b = FALSE;
        g_CANSMsgInfo_as[LLI_u16].timeStamp_s.lastTimeRcv_u32 = (t_uint32)0;
        g_CANSMsgInfo_as[LLI_u16].timeStamp_s.lastTimeSend_u32 = (t_uint32)0;
    }

    return Ret_e;
}

//********************************
// APPSIG_Cyclic
//********************************
t_eReturnCode APPSIG_Cyclic(void)
{
    t_eReturnCode Ret_e = RC_OK;

    switch(g_AppSig_ModState_e)
    {
        case STATE_CYCLIC_CFG:
        {
            Ret_e = s_APPSIG_ConfigurationState();

            if(Ret_e == RC_OK)
            {
                g_AppSig_ModState_e = STATE_CYCLIC_PREOPE;
            }
            break;
        }
        case STATE_CYCLIC_PREOPE:
        {
            Ret_e = APPSYS_SetFastTaskState(APPSYS_MODULE_APP_SIG,
                                            APPSYS_FAST_TASK_ENABLE);
            if(Ret_e == RC_OK)
            {
                g_AppSig_ModState_e = STATE_CYCLIC_OPE;
            }
            break;
        }
        case STATE_CYCLIC_OPE:
        {
            Ret_e = s_APPSIG_OperationalState();
            if(Ret_e < RC_OK)
            {
                ASSERT((t_uint16)0);
                g_AppSig_ModState_e = STATE_CYCLIC_ERROR;
            }
        }
        case STATE_CYCLIC_BUSY:
        case STATE_CYCLIC_ERROR:
        default:
        {
            break;
        }
    }

    return Ret_e;
}

/*********************************
 * APPSIG_GetState
 *********************************/
t_eReturnCode APPSIG_GetState(t_eCyclicModState *f_State_pe)
{
    t_eReturnCode Ret_e = RC_OK;

    if(f_State_pe == (t_eCyclicModState *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(Ret_e == RC_OK)
    {
        *f_State_pe = g_AppSig_ModState_e;
    }

    return Ret_e;
}

/*********************************
 * APPSIG_SetState
 *********************************/
t_eReturnCode APPSIG_SetState(t_eCyclicModState f_State_e)
{

    g_AppSig_ModState_e = f_State_e;

    return RC_OK;
}


/*********************************
 * APPSIG_SetSignalValue
 *********************************/
t_eReturnCode APPSIG_SetSignalValue(t_eAPPSIG_Signal f_signal_e, t_float32 f_value_f32)
{
    t_eReturnCode Ret_e;

    if(f_signal_e >= APPSIG_SIGNAL_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)0);
    }
    else 
    {
        Ret_e = RC_OK;
        g_signalInfo_as[f_signal_e].value_f32 = f_value_f32;

        if(g_signalInfo_as[f_signal_e].isValid_b == FALSE)
        {   
            g_signalInfo_as[f_signal_e].isValid_b = TRUE;
        }
    }

    return Ret_e;
}

/*********************************
 * APPSIG_GetSignalValue
 *********************************/
t_eReturnCode APPSIG_GetSignalValue(t_eAPPSIG_Signal f_signal_e, t_float32 * f_value_pf32)
{
    t_eReturnCode Ret_e;

    if(f_signal_e >= APPSIG_SIGNAL_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)0);
    }
    else if (f_value_pf32 == (t_float32 *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
        ASSERT((t_uint16)0);
    }
    else 
    {
        if((g_signalInfo_as[f_signal_e].isValid_b == FALSE)
        || (g_signalInfo_as[f_signal_e].isRcvOnce_b == FALSE))
        {
            *f_value_pf32 = 0.0f;
            Ret_e = RC_WARNING_NO_OPERATION;
        }
        else 
        {
            Ret_e = RC_OK;
            *f_value_pf32 =  (t_float32)g_signalInfo_as[f_signal_e].value_f32;
        }
    }

    return Ret_e;
}

/*********************************
 * APPSIG_AddRcvSigCallback
 *********************************/
t_eReturnCode APPSIG_ForceMsgSend(t_eAPPSIG_MsgOrigin f_origin_e, t_uint16 f_msgID_u16)
{
    t_eReturnCode Ret_e;

    if(f_origin_e >= APPSIG_MSG_ORIGIN_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)0);
    }
    else if(g_AppSig_ModState_e !=  STATE_CYCLIC_OPE)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    else 
    {
        Ret_e = RC_OK;
        
        if(f_origin_e == APPSIG_MSG_ORIGIN_CAN)
        {
            if(f_msgID_u16 >= (t_uint16)APPSIG_CAN_MSG_NB)
            {
                Ret_e = RC_ERROR_PARAM_INVALID;
                ASSERT((t_uint16)0);
            }

            g_CANSMsgInfo_as[f_msgID_u16].forceSend_b = TRUE;
        }
        else 
        {
            if(f_msgID_u16 >= (t_uint16)APPSIG_SRL_MSG_NB)
            {
                Ret_e = RC_ERROR_PARAM_INVALID;
                ASSERT((t_uint16)0);
            }

            g_SrlSMsgInfo_as[f_msgID_u16].forceSend_b = TRUE;
        }
    }

    return Ret_e;
}
/*********************************
 * APPSIG_AddRcvSigCallback
 *********************************/
t_eReturnCode APPSIG_AddRcvSigCallback(t_eAPPSIG_Signal f_signal_e, 
                                          t_cbAPPSIG_SignalRcvCallback * f_SigRcvCallback_pcb)
{
    t_eReturnCode Ret_e;
    t_uint8 idxSubcriber_u8;
    t_bool itemFreeFound_b = FALSE;

    if(f_signal_e >= APPSIG_SIGNAL_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)0);
    }
    else if(f_SigRcvCallback_pcb == NULL_FUNCTION)
    {
        Ret_e = RC_ERROR_PTR_NULL;
        ASSERT((t_uint16)0);
    }
    else 
    {
        Ret_e = RC_OK;
        for(idxSubcriber_u8 = (t_uint8)0 ; 
        (idxSubcriber_u8 < APPSIG_MSG_RCV_SUBSRIBERS_MAX)
        && (itemFreeFound_b == FALSE) ; 
        idxSubcriber_u8++)
        {
            if(g_signalInfo_as[f_signal_e].rcvCallback_pacb[idxSubcriber_u8] == NULL_FUNCTION)
            {
                itemFreeFound_b = TRUE;
                g_signalInfo_as[f_signal_e].rcvCallback_pacb[idxSubcriber_u8] = f_SigRcvCallback_pcb;
            }
        }
        //--- not found any container
        //      consider to rise APPSIG_MSG_RCV_SUBSRIBERS_MAX ----//
        if(itemFreeFound_b == FALSE)
        {
            Ret_e = RC_ERROR_LIMIT_REACHED;
        }
    }

    return Ret_e;
}

/*********************************
 * APPSIG_AddRcvMsgCallback
 *********************************/
t_eReturnCode APPSIG_AddRcvMsgCallback( t_uint16 f_msgID_u16,
                                        t_eAPPSIG_MsgOrigin f_msgOrigin_e,
                                        t_cbAPPSIG_MsgRcvCallback * f_msgRcvCallback_pcb)
{
    t_eReturnCode Ret_e;
    t_uint8 idxSubcriber_u8;
    t_sAPPSIG_MsgInfo * msgInfo_pas = NULL;
    t_bool itemFreeFound_b = FALSE;

    if(f_msgOrigin_e >= APPSIG_MSG_ORIGIN_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)0);
    }
    else if(f_msgRcvCallback_pcb == NULL_FUNCTION)
    {
        Ret_e = RC_ERROR_PTR_NULL;
        ASSERT((t_uint16)0);
    }
    else 
    {
        Ret_e = RC_OK;
        if(f_msgOrigin_e == APPSIG_MSG_ORIGIN_CAN)
        {
            if(f_msgID_u16 >= (t_uint16)APPSIG_CAN_MSG_NB)
            {
                Ret_e = RC_ERROR_PARAM_INVALID;
                ASSERT((t_uint16)f_msgID_u16);
            }

            msgInfo_pas = g_CANSMsgInfo_as;
        }
        else if(f_msgOrigin_e == APPSIG_MSG_ORIGIN_SRL)
        {    
            if(f_msgID_u16 >= (t_uint16)APPSIG_SRL_MSG_NB)
            {
                Ret_e = RC_ERROR_PARAM_INVALID;
                ASSERT((t_uint16)f_msgID_u16);
            }

            msgInfo_pas = g_SrlSMsgInfo_as;
        }
        if(Ret_e == RC_OK)
        {
            for(idxSubcriber_u8 = (t_uint8)0 ; 
            (idxSubcriber_u8 < APPSIG_MSG_RCV_SUBSRIBERS_MAX)
            && (itemFreeFound_b == FALSE) ; 
            idxSubcriber_u8++)
            {
                if(msgInfo_pas[f_msgID_u16].rcvCallback_pacb[idxSubcriber_u8] == NULL_FUNCTION)
                {
                    itemFreeFound_b = TRUE;
                    msgInfo_pas[f_msgID_u16].rcvCallback_pacb[idxSubcriber_u8] = f_msgRcvCallback_pcb;
                }
            }
            //--- not found any container
            //      consider to rise APPSIG_MSG_RCV_SUBSRIBERS_MAX ----//
            if(itemFreeFound_b == FALSE)
            {
                Ret_e = RC_ERROR_LIMIT_REACHED;
            }
        }
    }

    return Ret_e;
}
//********************************************************************************
//                      Local functions - Implementation
//********************************************************************************
/*********************************
 * s_APPSIG_ConfigurationState
 *********************************/
static t_eReturnCode s_APPSIG_ConfigurationState(void)
{
    t_eReturnCode Ret_e = RC_WARNING_NO_OPERATION;

    //---- first get the ecu Id for this software ----//
    Ret_e = APPSYS_GetEcuPosition(&g_EcuId_e);
    if(Ret_e == RC_OK)
    {
        //---- get the port gate configuration and initialize the Port ----//
        if(GETBIT(APPSIG_PORTGATE_CFG, APPSIG_PORTGATE_CAN) == BIT_IS_SET_32B)
        {
            Ret_e = s_APPSIG_InitializeCanGate();
            FMKSRL_LOG("[SIG] : CAN Gate Init\r\n");
        }
        if(GETBIT(APPSIG_PORTGATE_CFG, APPSIG_PORTGATE_SRL) == BIT_IS_SET_32B)
        {
            Ret_e = s_APPSIG_InitializeSrlGate();
            FMKSRL_LOG("[SIG] : CAN Serial Init\r\n");
        }
        if(Ret_e == RC_WARNING_NO_OPERATION)
        {
            //---- the module is inactive but allow to be in ope mode ----//
            Ret_e = RC_OK;
        }
        if(Ret_e == RC_OK)
        {
            APPSYS_AddFastTask(APPSYS_MODULE_APP_SIG, s_APPSIG_FastTask);
        }
    }

    return Ret_e;
}

/*********************************
 * s_APPSIG_OperationalState
 *********************************/
static t_eReturnCode s_APPSIG_OperationalState(void)
{
    t_eReturnCode Ret_e;

    //---- Call Rx Signal Managment ----//
    Ret_e = s_APPSIG_Ope_RxSignalMngmt();
    
    if(Ret_e < RC_OK)
    {
        ASSERT((t_uint16)Ret_e);
        Ret_e = RC_OK;
    }
    if(Ret_e >= RC_OK)
    {
        //---- Call Rx Diagnostic Mngmt ----//
        Ret_e = s_APPSIG_Ope_RxDiagnosticMngmt();
        if(Ret_e < RC_OK)
        {
            ASSERT((t_uint16)Ret_e);
        }
    }

    return Ret_e;
}

/*********************************
 * s_APPSIG_Ope_RxSignalMngmt
 *********************************/
static t_eReturnCode s_APPSIG_Ope_RxSignalMngmt(void)
{
    t_eReturnCode Ret_e;
    t_eReturnCode taskRet_e;
    t_sAPPSIG_msgPayload msgPayload_s;
    t_sAPPSIG_MsgCfg * msgCfg_ps = NULL;
    t_cbAPPSIG_MsgRcvCallback ** msgCallback_pacb = NULL;
    t_uint8 idxTreatMsg_u8;
    t_uint16 msgId_u16 = 0U;

    Ret_e = RC_OK;
    taskRet_e = RC_OK;
    for(idxTreatMsg_u8 = (t_uint8)0 ; 
        (idxTreatMsg_u8 < APPSIG_TREAT_ELEM_NB)
    &&  (taskRet_e == RC_OK) ; idxTreatMsg_u8++)
    {
        //---- read one element from the queue ----//
        taskRet_e = LIBQUEUE_ReadElement(   &g_RxSoftQueueMngmt_s,
                                            &msgPayload_s,
                                            sizeof(msgPayload_s));

        //---- no element left in queue ----//
        if(taskRet_e == RC_WARNING_NO_OPERATION)
        {
            //---- out of loop ----//
            Ret_e = RC_OK;
            break;
        }
        else if (taskRet_e == RC_OK)
        {
            //---- get the pointor for the message ----// 
            taskRet_e = s_APPSIG_FindSignalMapping( msgPayload_s, 
                                                    &msgCfg_ps, 
                                                    &msgId_u16,
                                                    &msgCallback_pacb);

            //---- it means this msg is configured but we're not dealing the msg ----//
            if(taskRet_e == RC_WARNING_NO_OPERATION)
            {
                taskRet_e = RC_OK;
            }
            else 
            {
                if(taskRet_e == RC_OK)
                {
                    taskRet_e = s_APPSIG_MsgDecoder(msgPayload_s.data_ua8, msgCfg_ps);
                }
                if(taskRet_e == RC_OK)
                {
                    taskRet_e = s_APPSIG_BroadcastUpdate(   msgCfg_ps,
                                                            msgId_u16, 
                                                            msgCallback_pacb);
                }
            }
        } 
    }
    //---- propagate error ----//                      
    if(taskRet_e != RC_OK)
    {
        Ret_e = taskRet_e;
    }

    return Ret_e;
}

/*********************************
 * s_APPSIG_FastTask_TxSignalMngmt
 *********************************/
static t_eReturnCode s_APPSIG_FastTask_TxSignalMngmt(void)
{
    t_eReturnCode Ret_e;
        
    Ret_e = RC_OK;
   //---- first try to send every Serial message if it is configured ----//
   if(GETBIT(APPSIG_PORTGATE_CFG, APPSIG_PORTGATE_SRL) == BIT_IS_SET_32B)
    {
        Ret_e = s_APPSIG_SendTxMsgMngmt(APPSIG_PORTGATE_SRL);
        if(Ret_e < RC_OK)
        {
            ASSERT((t_uint16)Ret_e);
        }
    }
    if(Ret_e >= RC_OK)
    {
        //---- first try to send every CAN message if it is configured ----//
        if(GETBIT(APPSIG_PORTGATE_CFG, APPSIG_PORTGATE_CAN) == BIT_IS_SET_32B)
        {
            Ret_e = s_APPSIG_SendTxMsgMngmt(APPSIG_PORTGATE_CAN);
        }
    }   

    return Ret_e;
}

/*********************************
 * s_APPSIG_Ope_RxDiagnosticMngmt
 *********************************/
static t_eReturnCode s_APPSIG_Ope_RxDiagnosticMngmt(void)
{
    t_eReturnCode Ret_e;

    Ret_e = RC_OK;
    //---- Diagnostic on Rx Serial Message ----//
    if(GETBIT(APPSIG_PORTGATE_CFG, APPSIG_PORTGATE_SRL) == BIT_IS_SET_32B)
    {
        Ret_e = s_APPSIG_RxTimeoutMngmt(APPSIG_PORTGATE_SRL);
        if(Ret_e < RC_OK)
        {
            ASSERT((t_uint16)Ret_e);
        }
    }
    if(Ret_e >= RC_OK)
    {
        //---- first try to send every CAN message if it is configured ----//
        if(GETBIT(APPSIG_PORTGATE_CFG, APPSIG_PORTGATE_CAN) == BIT_IS_SET_32B)
        {
            Ret_e = s_APPSIG_RxTimeoutMngmt(APPSIG_PORTGATE_CAN);
        }
    }

    return Ret_e;
}
/*********************************
 * s_APPSIG_SendTxMsgMngmt
 *********************************/
static t_eReturnCode s_APPSIG_SendTxMsgMngmt(t_eAPPSIG_MsgOrigin f_msgGate_e)
{
    t_eReturnCode Ret_e;
    t_uint16 idxMsg_u16;
    t_uint32 currentTime_u32;
    t_eAPPSIG_MsgDirection direction_e;
    t_sAPPSIG_MsgCfg * msgCfg_pas = (t_sAPPSIG_MsgCfg *)NULL;
    t_sAPPSIG_MsgInfo * msgInfo_pas = (t_sAPPSIG_MsgInfo *)NULL;
    t_cbAPPSIG_SendFrameMsg * sendMsgCallback_pf = (t_cbAPPSIG_SendFrameMsg *)NULL;
    t_uint16 nbMsg_u16 = (t_uint16)0; 

    if(f_msgGate_e >= APPSIG_MSG_ORIGIN_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)0);
    }    
    else 
    {
        Ret_e = RC_OK;
        FMKCPU_GetTick(&currentTime_u32);
        
        //---- get info depeding on gate use ----//
        if(f_msgGate_e == APPSIG_MSG_ORIGIN_CAN)
        {
            sendMsgCallback_pf = &s_APPSIG_SendCanFrame;
            msgInfo_pas = (t_sAPPSIG_MsgInfo *)g_CANSMsgInfo_as;
            msgCfg_pas = (t_sAPPSIG_MsgCfg *)(c_AppSig_CanMsgCfg_as);
            nbMsg_u16 = APPSIG_CAN_MSG_NB;
        }
        else if(f_msgGate_e == APPSIG_MSG_ORIGIN_SRL)
        {
            sendMsgCallback_pf = &s_APPSIG_SendSrlFrame;
            msgInfo_pas = (t_sAPPSIG_MsgInfo *)g_SrlSMsgInfo_as;
            msgCfg_pas = (t_sAPPSIG_MsgCfg *)(c_AppSig_SrlMsgCfg_as);
            nbMsg_u16 = APPSIG_SRL_MSG_NB;
        }
        else 
        {
            Ret_e = RC_ERROR_MISSING_CONFIG;
        }
    
        for(idxMsg_u16 = (t_uint16)0 ; 
        (idxMsg_u16 < nbMsg_u16) 
        && (Ret_e == RC_OK) ;
        idxMsg_u16++)
        {
            //---- reach direction depending on ecu Id ----//
            direction_e = msgCfg_pas[idxMsg_u16].direction_ae[g_EcuId_e];

            if( ((direction_e == APPSIG_MSG_DIR_TX)
            ||   (direction_e == APPSIG_MSG_DIR_RX_TX))
            && (((currentTime_u32 - msgInfo_pas[idxMsg_u16].timeStamp_s.lastTimeSend_u32) >=
                                                        (t_uint32)msgCfg_pas[idxMsg_u16].msgCycleSend_u16)
            || (msgInfo_pas[idxMsg_u16].forceSend_b == TRUE)))
            {
                //---- send serial or can frame ----//
                Ret_e = sendMsgCallback_pf(&msgCfg_pas[idxMsg_u16]);

                if(Ret_e == RC_OK)
                {
                    //--- update last time send ----//
                    msgInfo_pas[idxMsg_u16].timeStamp_s.lastTimeSend_u32 = currentTime_u32;

                    //---- update flag ----//
                    if(msgInfo_pas[idxMsg_u16].forceSend_b == TRUE)
                    {   
                        msgInfo_pas[idxMsg_u16].forceSend_b = FALSE;
                    }
                }
            }
        }
    }

    return Ret_e;
}

/*********************************
 * s_APPSIG_RxTimeoutMngmt
 *********************************/
static t_eReturnCode s_APPSIG_RxTimeoutMngmt(t_eAPPSIG_MsgOrigin f_msgGate_e)
{
    t_eReturnCode Ret_e;
    t_uint32 currentTime_u32;
    t_uint16 idxMsg_u16;
    t_uint8 idxSignal_u8;
    t_uint16 nbMsg_u16;
    t_eAPPSIG_MsgDirection direction_e;
    t_sAPPSIG_MsgInfo * msgInfo_pas = NULL;
    t_sAPPSIG_MsgCfg * msgCfg_pas = NULL;

    if(f_msgGate_e >= APPSIG_MSG_ORIGIN_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)0);
    }    
    else 
    {
        Ret_e = RC_OK;
        FMKCPU_GetTick(&currentTime_u32);

        //---- get info depeding on gate use ----//
        if(f_msgGate_e == APPSIG_MSG_ORIGIN_CAN)
        {
            msgInfo_pas = (t_sAPPSIG_MsgInfo *)g_CANSMsgInfo_as;
            msgCfg_pas = (t_sAPPSIG_MsgCfg *)(c_AppSig_CanMsgCfg_as);
            nbMsg_u16 = (t_uint8)APPSIG_CAN_MSG_NB;
        }
        else if(f_msgGate_e == APPSIG_MSG_ORIGIN_SRL)
        {
            msgInfo_pas = (t_sAPPSIG_MsgInfo *)g_SrlSMsgInfo_as;
            msgCfg_pas = (t_sAPPSIG_MsgCfg *)(c_AppSig_SrlMsgCfg_as);
            nbMsg_u16 = (t_uint8)APPSIG_SRL_MSG_NB;
        }
        else 
        {
            Ret_e = RC_ERROR_MISSING_CONFIG;
        }
        if(Ret_e == RC_OK)
        {
            for(idxMsg_u16 = (t_uint16)0 ; idxMsg_u16 < nbMsg_u16 ; idxMsg_u16++)
            {
                //----for receive message that are not receive often
                //      user put 65535 to say : don't check the signal

                //---- reach direction depending on ecu Id ----//
                direction_e = msgCfg_pas[idxMsg_u16].direction_ae[g_EcuId_e];

                if((direction_e == APPSIG_MSG_DIR_RX)
                || (direction_e == APPSIG_MSG_DIR_RX_TX))
                {
                    if(((currentTime_u32 - msgInfo_pas[idxMsg_u16].timeStamp_s.lastTimeRcv_u32)
                        > (t_uint32)msgCfg_pas[idxMsg_u16].msgTimeout_u16)
                    && (msgCfg_pas[idxMsg_u16].msgTimeout_u16 != (t_uint16)0xFFFF))
                    {
                        APPSDM_ReportDiagEvnt(  APPSDM_DIAG_ITEM_APPSIG_MSG_TIMEOUT,
                                                APPSDM_DIAG_ITEM_REPORT_FAIL,
                                                (t_uint16)idxMsg_u16,
                                                (t_uint16)f_msgGate_e);
                        
                        //---- also update validity of signal in this message ----//
                        for(idxSignal_u8 = (t_uint8)0 ; idxSignal_u8 < msgCfg_pas[idxMsg_u16].nbSignal_u8 ; idxSignal_u8++)
                        {
                            t_eAPPSIG_Signal sigID_e = (msgCfg_pas->msgSignalsCfg_pas[idxSignal_u8].signal_e);
                            g_signalInfo_as[sigID_e].isValid_b = FALSE;
                        }
                    }
                }
            }
        }
        
    }

    return Ret_e;
}

/*********************************
 * s_APPSIG_FindSignalMapping
 *********************************/
static t_eReturnCode s_APPSIG_FindSignalMapping(t_sAPPSIG_msgPayload f_msgPayload_s, 
                                                t_sAPPSIG_MsgCfg ** f_msgCfg_ps,
                                                t_uint16 * f_msgId_pu16,
                                                t_cbAPPSIG_MsgRcvCallback *** f_msgCallback_pacb)
{
    t_eReturnCode Ret_e;
    t_sAPPSIG_MsgCfg * msgPortGateCfg_pas;
    t_sAPPSIG_MsgInfo * msgInfo_pas;
    t_eAPPSIG_MsgDirection direction_e;
    t_uint16 idxMsgPrt_u16;
    t_uint16 msgPortNb_u16;
    t_uint32 currentTime_u32;
    t_bool msgFound_b = FALSE;

    if((f_msgCfg_ps == (t_sAPPSIG_MsgCfg **)NULL)
    || (f_msgId_pu16 == (t_uint16 *)NULL)
    || (f_msgCallback_pacb == (t_cbAPPSIG_MsgRcvCallback ***)NULL))
    {
        Ret_e = RC_ERROR_PTR_NULL;
        ASSERT((t_uint16)0);
    }
    else 
    {
        //--- set the pointor to the CanMsg Configuration ---//
        if(f_msgPayload_s.origin_e == APPSIG_PORTGATE_CAN)
        {
            msgInfo_pas = (t_sAPPSIG_MsgInfo *)g_CANSMsgInfo_as;
            msgPortNb_u16 = (t_uint16)APPSIG_CAN_MSG_NB;
            msgPortGateCfg_pas = (t_sAPPSIG_MsgCfg *)&c_AppSig_CanMsgCfg_as;
        }
        else if(f_msgPayload_s.origin_e == APPSIG_PORTGATE_SRL)
        {   
            msgInfo_pas = (t_sAPPSIG_MsgInfo *)g_SrlSMsgInfo_as;
            msgPortNb_u16 = (t_uint16)APPSIG_SRL_MSG_NB;
            msgPortGateCfg_pas = (t_sAPPSIG_MsgCfg *)&c_AppSig_SrlMsgCfg_as;
        }
        else 
        {
            msgPortNb_u16 = (t_uint16)0;
            msgPortGateCfg_pas = (t_sAPPSIG_MsgCfg *)NULL;
            ASSERT((t_uint16)f_msgPayload_s.origin_e);
            Ret_e = RC_ERROR_MISSING_CONFIG;         
        }

        if(msgPortGateCfg_pas != (t_sAPPSIG_MsgCfg *)NULL)
        {
            //--- we suppose that we don't found the element 
            Ret_e = RC_ERROR_LIMIT_REACHED;
            //---- find the message info from msgPortGate cfg ----//
            for(idxMsgPrt_u16 = (t_uint16)0 ; 
            (idxMsgPrt_u16 < msgPortNb_u16)
            && (msgFound_b == FALSE) ; 
            idxMsgPrt_u16++)
            {
                //---- only work on msg != from UNUSED ----//
                direction_e = msgPortGateCfg_pas[idxMsgPrt_u16].direction_ae[g_EcuId_e];            

                if(msgPortGateCfg_pas[idxMsgPrt_u16].msgId_u32 == (t_uint32)f_msgPayload_s.msgId_u32)
                {
                    if(direction_e == APPSIG_MSG_DIR_UNUSED)
                    {
                        Ret_e = RC_WARNING_NO_OPERATION;
                    }
                    else // RX oR RX_TX
                    {
                        *f_msgCfg_ps = &msgPortGateCfg_pas[idxMsgPrt_u16];
                        *f_msgCallback_pacb = msgInfo_pas[idxMsgPrt_u16].rcvCallback_pacb;
                        *f_msgId_pu16 = idxMsgPrt_u16;
                        //---- update last time msg received ----//
                        FMKCPU_GetTick(&currentTime_u32);
                        msgInfo_pas[idxMsgPrt_u16].timeStamp_s.lastTimeRcv_u32 = currentTime_u32;
                        //---- success -> out of loop ----//
                        Ret_e = RC_OK;
                    }
                    msgFound_b = TRUE;
                }
            }
        }
    }

    return Ret_e;

}

/*********************************
 * s_APPSIG_MsgDecoder
 *********************************/
static t_eReturnCode s_APPSIG_MsgDecoder(   t_uint8 * f_data_pu8, 
                                            const t_sAPPSIG_MsgCfg * msgCfg_ps)
{
    t_eReturnCode Ret_e;
    t_uint8 idxSignal_u8; 
    t_sAPPSIG_SigCfg * signalCfg_ps;
    t_sAPPSIG_MsgSignalsCfg * msgSignalsCfg_pas;
    t_eAPPSIG_Signal signalId_e;
    t_float32 sigVal_f32;
    t_uint32 rawValue_u32;

    if((f_data_pu8 == (t_uint8 *)NULL)
    || (msgCfg_ps == (const t_sAPPSIG_MsgCfg *)NULL))
    {
        Ret_e = RC_ERROR_PTR_NULL;
        ASSERT((t_uint16)0);
    }
    else 
    {
        Ret_e = RC_OK;

        for(idxSignal_u8 = (t_uint8)0 ; idxSignal_u8 < msgCfg_ps->nbSignal_u8 ; idxSignal_u8++)
        {
            //---- get info/ reset container -----//
            sigVal_f32 = (t_float32)0.0f;
            msgSignalsCfg_pas = (t_sAPPSIG_MsgSignalsCfg *)(msgCfg_ps->msgSignalsCfg_pas);
            signalId_e = msgSignalsCfg_pas[idxSignal_u8].signal_e; 

            if(signalId_e >= APPSIG_SIGNAL_NB)
            {
                Ret_e = RC_ERROR_PARAM_INVALID;
                ASSERT((t_uint16)signalId_e);
                break;
            }   
            else
            {
                // pointer to signal configuration startbit and lenght ----//
                signalCfg_ps = (t_sAPPSIG_SigCfg *)&c_AppSig_SignalCfg_as[signalId_e];
                rawValue_u32 = (t_uint32)s_APPSIG_ExtractRawValue(  f_data_pu8,
                                                                    msgSignalsCfg_pas[idxSignal_u8].startBit_u8,
                                                                    signalCfg_ps->bitLenght_u8,
                                                                    signalCfg_ps->sigEncode_e);

                sigVal_f32 = (t_float32)rawValue_u32 * signalCfg_ps->factor_f32 + (t_float32)signalCfg_ps->offset_s32;
                g_signalInfo_as[signalId_e].value_f32 = (t_float32)sigVal_f32;

                //---- update flag ----//
                if(g_signalInfo_as[signalId_e].isRcvOnce_b == FALSE)
                {
                    g_signalInfo_as[signalId_e].isRcvOnce_b = TRUE;
                }
                if(g_signalInfo_as[signalId_e].isValid_b == FALSE)
                {
                    g_signalInfo_as[signalId_e].isValid_b = TRUE;
                }
            }
        }
    }

    return Ret_e;
}

/*********************************
 * s_APPSIG_MsgEncoder
 *********************************/
static t_eReturnCode s_APPSIG_MsgEncoder(   t_uint8 * f_data_pu8, 
                                            const t_sAPPSIG_MsgCfg * f_msgInfo_ps)
{
    t_eReturnCode Ret_e;
    t_uint8 idxSignal_u8; 
    t_sAPPSIG_SigCfg * signalCfg_ps;
    t_sAPPSIG_MsgSignalsCfg * msgSignalsCfg_pas;
    t_eAPPSIG_Signal signalId_e;
    t_float32 sigVal_f32;
    t_uint32 rawValue_u32;

    if((f_data_pu8 == (t_uint8 *)NULL)
    || (f_msgInfo_ps == (const t_sAPPSIG_MsgCfg *)NULL))
    {
        Ret_e = RC_ERROR_PTR_NULL;
        ASSERT((t_uint16)0);
    }
    else
    {
        Ret_e = RC_OK;
        for(idxSignal_u8 = (t_uint8)0; idxSignal_u8 < f_msgInfo_ps->nbSignal_u8 ; idxSignal_u8++)
        {
            //---- get info/reset ----//
            sigVal_f32 = (t_float32)0.0f;
            msgSignalsCfg_pas = (t_sAPPSIG_MsgSignalsCfg *)(f_msgInfo_ps->msgSignalsCfg_pas);
            signalId_e = msgSignalsCfg_pas[idxSignal_u8].signal_e;

            if(signalId_e >= APPSIG_SIGNAL_NB)
            {
                Ret_e = RC_ERROR_PARAM_INVALID;
                ASSERT((t_uint16)signalId_e);
                break;
            }
            else
            {
                signalCfg_ps = (t_sAPPSIG_SigCfg *)&c_AppSig_SignalCfg_as[(t_uint16)signalId_e];
                sigVal_f32 = g_signalInfo_as[(t_uint16)(signalId_e)].value_f32;

                rawValue_u32 = (t_uint32)((sigVal_f32 - (t_float32)signalCfg_ps->offset_s32) 
                                        / (t_float32)signalCfg_ps->factor_f32);

                s_APPSIG_InsertRawValue( f_data_pu8,
                                         msgSignalsCfg_pas[idxSignal_u8].startBit_u8,
                                         signalCfg_ps->bitLenght_u8,
                                         signalCfg_ps->sigEncode_e,
                                         (t_uint32)rawValue_u32 );
            }
        }
    }

    return Ret_e;
}

/*********************************
 * s_APPSIG_InitializeSrlGate
 *********************************/
static t_eReturnCode s_APPSIG_InitializeSrlGate(void)
{
    t_eReturnCode Ret_e;
    t_sFMKSRL_DrvSerialCfg SrlCfg_s;
    SrlCfg_s.runMode_e = FMKSRL_LINE_RUNMODE_DMA;
    SrlCfg_s.hwProtType_e = FMKSRL_HW_PROTOCOL_UART;

    SrlCfg_s.hwCfg_s.Baudrate_e = FMKSRL_LINE_BAUDRATE_115200,
    SrlCfg_s.hwCfg_s.Mode_e = FMKSRL_LINE_MODE_RX_TX;
    SrlCfg_s.hwCfg_s.Parity_e = FMKSRL_LINE_PARITY_NONE,
    SrlCfg_s.hwCfg_s.Stopbit_e = FMKSRL_LINE_STOPBIT_1,
    SrlCfg_s.hwCfg_s.wordLenght_e = FMKSRL_LINE_WORDLEN_8BITS,

    SrlCfg_s.CfgSpec_u.uartCfg_s.hwFlowCtrl_e = FMKSRL_UART_HW_FLOW_CTRL_NONE;
    SrlCfg_s.CfgSpec_u.uartCfg_s.Type_e = FMKSRL_UART_TYPECFG_UART;

    Ret_e = FMKSRL_InitDrv( APPSIG_PORTGATE_SRL_LINE,
                            SrlCfg_s,
                            s_APPSIG_SerialRcvCallback,
                            (t_cbFMKSRL_TransmitMsgEvent *)NULL_FUNCTION);
    if(Ret_e == RC_OK)
    {
        Ret_e = FMKSRL_ConfigureReception(  APPSIG_PORTGATE_SRL_LINE,
                                            FMKSRL_OPE_RX_CYCLIC_SIZE,
                                            APPSIG_SRL_FRAME_PAYLOAD_LEN);
    }

    return Ret_e;
}

/*********************************
 * s_APPSIG_InitializeCanGate
 *********************************/
static t_eReturnCode s_APPSIG_InitializeCanGate(void)
{
    t_eReturnCode Ret_e;

    t_sFMKFDCAN_RxItemEventCfg rxItemEvnCfg_s = {
        .ItemId_s.Identifier_u32 = APPSIG_CAN_ID_COMMON,
        .ItemId_s.FramePurpose_e = FMKFDCAN_FRAME_PURPOSE_DATA,
        .ItemId_s.IdType_e = FMKFDCAN_IDTYPE_EXTENDED,
        .Dlc_e = FMKFDCAN_DLC_8,
        .maskId_u32 = APPSIG_CAN_ID_MASK,
        .callback_cb = s_APPSIG_CanRcvCallback
    };

    Ret_e = FMKFDCAN_ConfigureRxItemEvent(APPSIG_PORTGATE_CAN_NODE, rxItemEvnCfg_s);

    return Ret_e;
}

/*********************************
 * s_APPSIG_ExtractRawValue
 *********************************/
static void s_APPSIG_SerialRcvCallback( t_uint8 * f_rxData_pu8, 
                                        t_uint16 f_dataSize_u16, 
                                        t_eFMKSRL_RxCallbackInfo f_InfoCb_e)
{
    static t_uint8 s_idxWrite_u8 = (t_uint8)0;
    static t_uint8 s_RxBuffer_ua8[APPSIG_SRL_FRAME_PAYLOAD_LEN];
    t_sAPPSIG_msgPayload msgPayload_s;
    t_bool receptionComplete_b = False;
    t_eReturnCode Ret_e;

    if(f_dataSize_u16 != APPSIG_SRL_FRAME_PAYLOAD_LEN)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)(f_dataSize_u16));
    }
    if(s_idxWrite_u8 >= APPSIG_SRL_FRAME_PAYLOAD_LEN)
    {
        Ret_e = RC_ERROR_LIMIT_REACHED;
        ASSERT((t_uint16)(s_idxWrite_u8));
    }
    else
    {
        switch (f_InfoCb_e)
        {
            case FMKSRL_CB_INFO_RECEIVE_PENDING:
            {
                Ret_e = SafeMem_memcpy( (&s_RxBuffer_ua8[s_idxWrite_u8]),
                                        f_rxData_pu8,
                                        f_dataSize_u16);
                break;
            }
            case FMKSRL_CB_INFO_RECEIVE_ENDING:
            {
                Ret_e = SafeMem_memcpy( (&s_RxBuffer_ua8[s_idxWrite_u8]),
                                        f_rxData_pu8,
                                        f_dataSize_u16);
                if(Ret_e == RC_OK)
                {
                    s_idxWrite_u8 = (t_uint8)0;
                    receptionComplete_b = (t_bool)True;
                }
                break;
            }
            case FMKSRL_CB_INFO_RECEIVE_ERR:
            case FMKSRL_CB_INFO_RECEIVE_OVERFLOW:
            {
                //----- Report Diagnostic Evnt -----//
                break;
            }
        }
        if(receptionComplete_b == (t_bool)True)
        {
            if((s_RxBuffer_ua8[0] != APPSIG_SRL_START_BYTE_0)
            || (s_RxBuffer_ua8[1] != APPSIG_SRL_START_BYTE_1))
            {
                ASSERT((t_uint16)(s_RxBuffer_ua8[0] << (t_uint8)8 | s_RxBuffer_ua8[1]));
            }
            else 
            {
                //---- copy into buffer ----//
                msgPayload_s.msgId_u32 = (t_uint32)s_RxBuffer_ua8[2];
                msgPayload_s.origin_e = APPSIG_MSG_ORIGIN_SRL;
                (void)SafeMem_memcpy(   msgPayload_s.data_ua8, 
                                        &s_RxBuffer_ua8[3], 
                                        (sizeof(t_uint8) * APPSIG_DATA_PAYLOAD_LEN));
                Ret_e = LIBQUEUE_WriteElement(  &g_RxSoftQueueMngmt_s, 
                                                &msgPayload_s, 
                                                sizeof(t_sAPPSIG_msgPayload));

                if(Ret_e != RC_OK)
                {
                    ASSERT((t_uint16)Ret_e);
                }
                else 
                {
                    (void)SafeMem_memclear( s_RxBuffer_ua8, 
                                            (sizeof(t_uint8) * APPSIG_SRL_FRAME_PAYLOAD_LEN));
                }
            } 
        }
    }

    return;
}

/*********************************
 * s_APPSIG_CanRcvCallback
 *********************************/
static void  s_APPSIG_CanRcvCallback(t_eFMKFDCAN_NodeList f_Node_e,
                                    t_sFMKFDCAN_RxItemEvent f_RxItem_s, 
                                    t_eFMKFDCAN_NodeStatus f_NodeStatus_e)
{
    t_eReturnCode Ret_e;
    t_sAPPSIG_msgPayload msgPayload_s;

    if(f_Node_e != APPSIG_PORTGATE_CAN_NODE)
    {
        ASSERT((t_uint16)f_Node_e);
    }
    else if(f_RxItem_s.CanMsg_s.Dlc_e != APPSIG_DATA_PAYLOAD_LEN)
    {
        ASSERT((t_uint16)(f_RxItem_s.ItemId_s.Identifier_u32));
    }
    else if(f_NodeStatus_e != FMKFDCAN_NODE_STATE_OK)
    {
        ASSERT((t_uint16)f_NodeStatus_e);
    }
    else 
    {
        msgPayload_s.msgId_u32 = (t_uint32)f_RxItem_s.ItemId_s.Identifier_u32;
        msgPayload_s.origin_e = APPSIG_MSG_ORIGIN_CAN;

        Ret_e = SafeMem_memcpy( &msgPayload_s.data_ua8, 
                                f_RxItem_s.CanMsg_s.data_pu8,
                                (t_uint16)APPSIG_DATA_PAYLOAD_LEN);
        if(Ret_e != RC_OK)
        {
            ASSERT((t_uint16)Ret_e);
        }
        else 
        {
            Ret_e = LIBQUEUE_WriteElement(  &g_RxSoftQueueMngmt_s,
                                            &msgPayload_s,
                                            sizeof(t_sAPPSIG_msgPayload));
            if(Ret_e != RC_OK)
            {
                ASSERT((t_uint16)Ret_e);
            }
        }      
    }

    return;
}

/*********************************
 * s_APPSIG_ExtractRawValue
 *********************************/
static t_uint32 s_APPSIG_ExtractRawValue(   const t_uint8 *f_data_pu8,
                                            t_uint8 f_startBit_u8,
                                            t_uint8 f_bitLength_u8,
                                            t_eAPPSIG_SigEncode f_encode_e)
{
    t_uint32 rawValue_u32 = 0;

    for (t_uint8 i = 0; i < f_bitLength_u8; i++)
    {
        t_uint8 msgBit;

        if (f_encode_e == APPSIG_SIG_ENCODE_INTEL) // Intel (LSB at startBit)
        {
            msgBit = f_startBit_u8 + i;
        }
        else // Motorola (MSB at startBit)
        {
            t_uint8 byte = f_startBit_u8 / 8;
            t_uint8 bit = f_startBit_u8 % 8;

            msgBit = (byte * 8 + bit) - i;
            msgBit = ((7 - (msgBit / 8)) * 8) + (msgBit % 8);
        }

        t_uint8 byteIndex = msgBit / 8;
        t_uint8 bitInByte = msgBit % 8;

        t_uint8 bitVal = (f_data_pu8[byteIndex] >> bitInByte) & 0x01;
        rawValue_u32 |= ((t_uint32)bitVal << i);
    }

    return rawValue_u32;
}

 

/*********************************
 * s_APPSIG_InsertRawValue
 *********************************/
static void s_APPSIG_InsertRawValue(    t_uint8 *f_data_pu8,
                                        t_uint8 f_startBit_u8,
                                        t_uint8 f_bitLength_u8,
                                        t_eAPPSIG_SigEncode f_encode_e,
                                        t_uint32 f_rawValue_u32)
{
    for (t_uint8 i = 0; i < f_bitLength_u8; i++)
    {
        t_uint8 bitIndex = i;
        t_uint8 msgBit;

        if (f_encode_e == APPSIG_SIG_ENCODE_INTEL) // Intel = LSB at startBit, increasing bit index
        {
            msgBit = f_startBit_u8 + i;
        }
        else // Motorola = MSB at startBit, decreasing bit index (across bytes)
        {
            t_uint8 byte = f_startBit_u8 / 8;
            t_uint8 bit = f_startBit_u8 % 8;

            msgBit = (byte * 8 + bit) - i;
            // Correction pour l’inversion des octets à la frontière
            msgBit = ((7 - (msgBit / 8)) * 8) + (msgBit % 8);
        }

        t_uint8 byteIndex = msgBit / 8;
        t_uint8 bitInByte = msgBit % 8;

        // Clear bit
        f_data_pu8[byteIndex] &= ~(1U << bitInByte);
        // Set bit if needed
        if (f_rawValue_u32 & (1U << bitIndex))
        {
            f_data_pu8[byteIndex] |= (1U << bitInByte);
        }
    }
}


/*********************************
 * s_APPSIG_InsertRawValue
 *********************************/
static t_eReturnCode s_APPSIG_SendSrlFrame(t_sAPPSIG_MsgCfg * f_msgInfo_ps)
{
    t_eReturnCode Ret_e;
    t_uint8 msgData_au8[APPSIG_SRL_FRAME_PAYLOAD_LEN];

    //---- Initialize data container ----//
    memset(msgData_au8, (t_uint8)0, (sizeof(t_uint8) * APPSIG_SRL_FRAME_PAYLOAD_LEN));

    //---- first byte is for the Id ----//
    msgData_au8[0] = (t_uint8)APPSIG_SRL_START_BYTE_0;
    msgData_au8[1] = (t_uint8)APPSIG_SRL_START_BYTE_1;
    msgData_au8[2] = (t_uint8)f_msgInfo_ps->msgId_u32;

    Ret_e = s_APPSIG_MsgEncoder(&msgData_au8[3], f_msgInfo_ps);

    if(Ret_e == RC_OK)
    {
        Ret_e = FMKSRL_Transmit(APPSIG_PORTGATE_SRL_LINE,
                                FMKSRL_TX_ONESHOT,
                                msgData_au8,
                                APPSIG_SRL_FRAME_PAYLOAD_LEN,
                                (t_uint16)0,
                                (t_bool)False);
    }

    return Ret_e;
}

/*********************************
 * s_APPSIG_SendCanFrame
 *********************************/
static t_eReturnCode s_APPSIG_SendCanFrame(t_sAPPSIG_MsgCfg * f_msgInfo_ps)
{
    t_eReturnCode Ret_e;
    t_uint8 msgData_au8[APPSIG_DATA_PAYLOAD_LEN];
    t_sFMKFDCAN_TxItem canTxItem_s;

    if(f_msgInfo_ps == (t_sAPPSIG_MsgCfg *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
        ASSERT((t_uint16)0);
    }
    else 
    {
        Ret_e = s_APPSIG_MsgEncoder(&msgData_au8[0], f_msgInfo_ps);

        if(Ret_e == RC_OK)
        {
            canTxItem_s.ItemId_s.Identifier_u32 = f_msgInfo_ps->msgId_u32;
            canTxItem_s.ItemId_s.FramePurpose_e = FMKFDCAN_FRAME_PURPOSE_DATA; 
            canTxItem_s.ItemId_s.IdType_e = FMKFDCAN_IDTYPE_EXTENDED;
            canTxItem_s.BitRate_e = FMKFDCAN_BITRATE_SWITCH_OFF;
            canTxItem_s.frameFormat_e = FMKFDCAN_FRAME_FORMAT_CLASSIC;
            canTxItem_s.CanMsg_s.Direction_e = FMKFDCAN_NODE_DIRECTION_TX;
            canTxItem_s.CanMsg_s.Dlc_e = FMKFDCAN_DLC_8;
            canTxItem_s.CanMsg_s.data_pu8 = &msgData_au8[0];

            Ret_e = FMKFDCAN_SendTxItem(APPSIG_PORTGATE_CAN_NODE, canTxItem_s);
        }
    }
    return Ret_e;
}

/*********************************
 * s_APPSIG_FastTask
 *********************************/
static void s_APPSIG_FastTask(void)
{
    t_eReturnCode Ret_e;
    //---- Call Rx Signal Managment ----//
    Ret_e = s_APPSIG_FastTask_TxSignalMngmt();
    if(Ret_e < RC_OK)
    {
        ASSERT((t_uint16)Ret_e);
    }

    return;
}

/*********************************
 * s_APPSIG_BroadcastUpdate
 *********************************/
static t_eReturnCode s_APPSIG_BroadcastUpdate(  t_sAPPSIG_MsgCfg  * f_msgCfg_ps, 
                                                t_uint16 f_msgEnmID_u16,
                                                t_cbAPPSIG_MsgRcvCallback ** f_rcvMsgCb_pacb)
{
    t_eReturnCode Ret_e;
    t_uint8 idxSubscibers_u8;
    t_eAPPSIG_Signal sigToBroadcast_e;
    t_uint8 idxmsgSignal_u8;

    if((f_msgCfg_ps == (t_sAPPSIG_MsgCfg  *)NULL)
    || (f_rcvMsgCb_pacb == (t_cbAPPSIG_MsgRcvCallback **)NULL))
    {
        Ret_e = RC_ERROR_PTR_NULL;
        ASSERT((t_uint16)0);
    }
    else 
    {
        Ret_e = RC_OK;
        (void)SafeMem_memclear(g_signalValBufferCb_af32, sizeof(g_signalValBufferCb_af32));
        (void)SafeMem_memclear(g_signalIDBufferCb_ae, sizeof(g_signalIDBufferCb_ae));

        for(idxmsgSignal_u8 = (t_uint8)0 ; idxmsgSignal_u8 < f_msgCfg_ps->nbSignal_u8 ; idxmsgSignal_u8++)
        {
            sigToBroadcast_e = f_msgCfg_ps->msgSignalsCfg_pas[idxmsgSignal_u8].signal_e;

            for(idxSubscibers_u8 = (t_uint8)0 ; idxSubscibers_u8 < APPSIG_SIG_RCV_SUBSRIBERS_MAX ; idxSubscibers_u8++)
            {
                //---- as we regiter the callback by order, if the fist is NULL
                //      others are ---//
                if(g_signalInfo_as[sigToBroadcast_e].rcvCallback_pacb[idxSubscibers_u8] != NULL_FUNCTION)
                {
                    g_signalInfo_as[sigToBroadcast_e].rcvCallback_pacb[idxSubscibers_u8](sigToBroadcast_e,
                                                                                        g_signalInfo_as[sigToBroadcast_e].value_f32);
                }
                else 
                {
                    //---- other are also NULL_FUNCTION ----//
                    break; 
                }
            }

            //---- build msg callback signal value buffer ----//
            if(idxmsgSignal_u8 < APPSIG_BUFFER_MSG_SIG_CB)
            {
                g_signalIDBufferCb_ae[idxmsgSignal_u8] = sigToBroadcast_e;
                g_signalValBufferCb_af32[idxmsgSignal_u8] = g_signalInfo_as[sigToBroadcast_e].value_f32;
            }
            else
            {
                ASSERT((t_uint16)idxmsgSignal_u8);
            }
        }

        for(idxSubscibers_u8 = (t_uint8)0 ; idxSubscibers_u8 < APPSIG_MSG_RCV_SUBSRIBERS_MAX ; idxSubscibers_u8++)
        {
            //---- as we regiter the callback by order, if the fist is NULL
            //      others are ---//
            if(f_rcvMsgCb_pacb[idxSubscibers_u8] != NULL_FUNCTION)
            {
                f_rcvMsgCb_pacb[idxSubscibers_u8](  f_msgEnmID_u16,
                                                    f_msgCfg_ps->nbSignal_u8,
                                                    g_signalIDBufferCb_ae,
                                                    g_signalValBufferCb_af32);
            }
            else 
            {
                //---- other are also NULL_FUNCTION ----//
                break; 
            }
        }
    }

    return Ret_e;
}
//************************************************************************************
// End of File
//************************************************************************************

/**
 *
 *	@brief
 *	@note   
 *
 *
 *	@param[in] 
 *	@param[out]
 *	 
 *
 *
 */
