/*********************************************************************
 * @file        APPACT_ConfigPrivate.h
 * @brief       Template_BriefDescription.
 * @note        TemplateDetailsDescription.\n
 *
 * @author      xxxxxx
 * @date        jj/mm/yyyy
 * @version     1.0
 */
  
#ifndef APPSIG_CONFIGPRIVATE_H_INCLUDED
#define APPSIG_CONFIGPRIVATE_H_INCLUDED





    // ********************************************************************
    // *                      Includes
    // ********************************************************************
    #include "./APPSIG_ConfigPublic.h"
    #include "TypeCommon.h"
    //uncomment
    #include "FMK_HAL/FMK_SRL/Src/FMK_SRL.h"
    // ********************************************************************
    // *                      Defines
    // ********************************************************************


    ///@brief Define to set the end of signal in the MSg
    #define APPSIG_SIGNAL_NONE (APPSIG_SIGNAL_NB)

    ///@brief Define the number of max element we can put in the queue 
    #define APPSIG_RX_BUFFER_SIZE ((t_uint8)30)

    ///@brief Number of data expected to be received 
    #define APPSIG_DATA_PAYLOAD_LEN  ((t_uint8)8)       // For CAN
    #define APPSIG_SRL_DATA_PAYLOAD_LEN ((t_uint8)9)    // for Serial
    ///@brief Configuration Gate for the module 
    #define APPSIG_PORTGATE_CAN (APPSIG_MSG_ORIGIN_CAN)

    #define APPSIG_PORTGATE_SRL (APPSIG_MSG_ORIGIN_SRL)
    #define APPSIG_PORTGATE_SRL_LINE (FMKSRL_SERIAL_LINE_2)

    #define APPSIG_PORTGATE_CFG (1 << APPSIG_MSG_ORIGIN_SRL)
    /* CAUTION : Automatic generated code section for Define: Start */
    ///@brief Serial Message Id
    #define APPSIG_SRL_ID_SYMBOL1         ((t_uint8)0x10)
    ///@brief CAN Message Id
    #define APPSIG_CAN_ID_SYMBOL2         ((t_uint8)0x20)
    /* CAUTION : Automatic generated code section for Define: End */
    

    ///@brief number of queue element treated per cycle 
    #define APPSIG_TREAT_ELEM_NB        ((t_uint8)10)

    #define APPSIG_MSG_RX_ONLY          ((t_uint32)0xFFFF)
    // ********************************************************************
    // *                      Types
    // ********************************************************************
    /* CAUTION : Automatic generated code section for Enum: Start */

    /* CAUTION : Automatic generated code section for Enum: End */
	//-----------------------------ENUM TYPES-----------------------------//
    

	/* CAUTION : Automatic generated code section for Structure: Start */

	/* CAUTION : Automatic generated code section for Structure: End */
	//-----------------------------STRUCT TYPES---------------------------//
	/* CAUTION : Automatic generated code section : Start */

	/* CAUTION : Automatic generated code section : End */
	//-----------------------------TYPEDEF TYPES---------------------------//
    ///@brief Information for a signal 
	typedef struct 
    {
        t_uint8 bitLenght_u8;                    //---- the lenght of the signal ----//
        t_eAPPSIG_SigEncode sigEncode_e;         //---- to know if the decode has to be made in little or big Endian
        t_float32 factor_f32;                    //---- multiply factor for the signal ----//
        t_sint16 offset_s16;                     //---- offset to add / soustract signal ----//     
    } t_sAPPSIG_SigCfg;

    ///@brief Structure to identify the signals into a message
    typedef struct 
    {
        t_eAPPSIG_Signal signal_e;
        t_uint8 startBit_u8;
    } t_sAPPSIG_MsgSignalsCfg;
    ///@brief Information for a message 
    typedef struct 
    {
        t_uint32 msgId_u32;                     //---- the identifier of the message ----//
        t_eAPPSIG_MsgDirection direction_e;     //---- Rx, Tx message direction ----//
        t_uint16 msgCycleSend_u16;             //---- message cyclic updated ----//
        t_uint16 msgTimeout_u16;                //---- Time upon if a msg is not receive a Deev will be raise ----//             
        const t_sAPPSIG_MsgSignalsCfg * msgSignalsCfg_pas;      //---- signal in the message ----// 
        t_uint8 nbSignal_u8;
    } t_sAPPSIG_MsgInfo;

    
	/* CAUTION : Automatic generated code section : Start */

	/* CAUTION : Automatic generated code section : End */
	
    // ********************************************************************
    // *                      Prototypes
    // ********************************************************************
        
    // ********************************************************************
    // *                      Variables
    // ********************************************************************
    /* CAUTION : Automatic generated code section for Variable: Start */
    ///@brief Signal Configuration
    const t_sAPPSIG_SigCfg c_AppSig_SignalCfg_as[APPSIG_SIGNAL_NB] = {
        {(t_uint8)4,                            APPSIG_SIG_ENCODE_INTEL,                        (t_float32)4.0f,                            (t_sint16)0                             },// APPSIG_SIGNAL_SIGNAL1
        {(t_uint8)8,                            APPSIG_SIG_ENCODE_INTEL,                        (t_float32)1.0f,                            (t_sint16)0                             },// APPSIG_SIGNAL_SIGNAL2
        {(t_uint8)4,                            APPSIG_SIG_ENCODE_INTEL,                        (t_float32)1.0f,                            (t_sint16)0                             },// APPSIG_SIGNAL_SIGNAL3
        {(t_uint8)6,                            APPSIG_SIG_ENCODE_INTEL,                        (t_float32)1.0f,                            (t_sint16)0                             },// APPSIG_SIGNAL_SIGNAL4
        {(t_uint8)2,                            APPSIG_SIG_ENCODE_INTEL,                        (t_float32)1.0f,                            (t_sint16)0                             },// APPSIG_SIGNAL_SIGNAL5
        {(t_uint8)1,                            APPSIG_SIG_ENCODE_INTEL,                        (t_float32)1.0f,                            (t_sint16)0                             },// APPSIG_SIGNAL_SIGNAL6
        {(t_uint8)4,                            APPSIG_SIG_ENCODE_INTEL,                        (t_float32)1.0f,                            (t_sint16)0                             },// APPSIG_SIGNAL_SIGNAL7
        {(t_uint8)16,                           APPSIG_SIG_ENCODE_INTEL,                        (t_float32)1.0f,                            (t_sint16)0                             },// APPSIG_SIGNAL_SIGNAL8
        {(t_uint8)16,                           APPSIG_SIG_ENCODE_INTEL,                        (t_float32)1.0f,                            (t_sint16)0                             },// APPSIG_SIGNAL_SIGNAL9
        {(t_uint8)8,                            APPSIG_SIG_ENCODE_INTEL,                        (t_float32)1.0f,                            (t_sint16)0                             },// APPSIG_SIGNAL_SIGNAL10
        {(t_uint8)32,                           APPSIG_SIG_ENCODE_INTEL,                        (t_float32)1.0f,                            (t_sint16)0                             },// APPSIG_SIGNAL_SIGNAL11
        {(t_uint8)32,                           APPSIG_SIG_ENCODE_INTEL,                        (t_float32)1.0f,                            (t_sint16)0                             },// APPSIG_SIGNAL_SIGNAL12
        {(t_uint8)8,                            APPSIG_SIG_ENCODE_INTEL,                        (t_float32)1.0f,                            (t_sint16)0                             },// APPSIG_SIGNAL_SIGNAL13
    };

    ///@brief Variable for decoding Symbol1
    const t_sAPPSIG_MsgSignalsCfg c_AppSig_Srl_Symbol1_as[9] = {
    {APPSIG_SIGNAL_SIGNAL1,        (t_uint8)0                    },
    {APPSIG_SIGNAL_SIGNAL2,        (t_uint8)4                    },
    {APPSIG_SIGNAL_SIGNAL3,        (t_uint8)12                   },
    {APPSIG_SIGNAL_SIGNAL4,        (t_uint8)16                   },
    {APPSIG_SIGNAL_SIGNAL5,        (t_uint8)22                   },
    {APPSIG_SIGNAL_SIGNAL6,        (t_uint8)24                   },
    {APPSIG_SIGNAL_SIGNAL7,        (t_uint8)25                   },
    {APPSIG_SIGNAL_SIGNAL8,        (t_uint8)32                   },
    {APPSIG_SIGNAL_SIGNAL9,        (t_uint8)48                   },
    };


    ///@brief Variable for decoding SIGNAL9
    const t_sAPPSIG_MsgSignalsCfg c_AppSig_Can_Symbol2_as[3] = {
    {APPSIG_SIGNAL_SIGNAL8,        (t_uint8)0                    },
    {APPSIG_SIGNAL_SIGNAL11,       (t_uint8)16                   },
    {APPSIG_SIGNAL_SIGNAL10,       (t_uint8)48                   },
    };


    ///@brief Serial Message Information
    const t_sAPPSIG_MsgInfo c_AppSig_SrlMsgCfg_as[APPSIG_SRL_MSG_NB] = {
    //  Identifier                          Direction                    CyclicSend                      TimeOut                        Sig Cfg                  nbSignal
    {APPSIG_SRL_ID_SYMBOL1,         APPSIG_MSG_DIR_RX_TX,          (t_uint16)1000,                (t_uint16)1000,                c_AppSig_Srl_Symbol1_as,       (t_uint8)9}, // APPSIG_SRL_SYMBOL1
    };

    ///@brief CAN Message Information
    const t_sAPPSIG_MsgInfo c_AppSig_CanMsgCfg_as[APPSIG_CAN_MSG_NB] = {
    //  Identifier                          Direction                    CyclicSend                      TimeOut                        Sig Cfg                  nbSignal
    {APPSIG_CAN_ID_SYMBOL2,         APPSIG_MSG_DIR_RX_TX,          (t_uint16)400,                 (t_uint16)50,                  c_AppSig_Can_Symbol2_as,       (t_uint8)3}, // APPSIG_CAN_SYMBOL2
    };

    /* CAUTION : Automatic generated code section for Variable: End */
    //********************************************************************************
    //                      Public functions - Prototyupes
    //********************************************************************************

#endif // APPSDM_CONFIGPRIVATE_H_INCLUDED           
//************************************************************************************
// End of File
//************************************************************************************

/**
 *
 *	@brief
 *	@note   
 *
 *
 *	@params[in] 
 *	@params[out]
 *	 
 *
 *
 */
