/**
 * @file        .h
 * @brief       Memomry access control for DMA & user access\n.
 * @note        TemplateDetailsDescription.\n
 *
 * @author      xxxxxx
 * @date        jj/mm/yyyy
 * @version     1.0
 */
  
 #ifndef APP_SIG_H_INCLUDED
 #define APP_SIG_H_INCLUDED
 
    // ********************************************************************
    // *                      Includes
    // ********************************************************************
    #include "TypeCommon.h"
    #include "APP_CFG/ConfigFiles/APPSIG_ConfigPublic.h"

    // ********************************************************************
    // *                      Defines
    // ********************************************************************

    // ********************************************************************
    // *                      Types
    // ********************************************************************
    /**
     * @brief Callback user to be noticed whenever a reception
     *         of a certain msg has arrived
     * @param[in] f_msgID_u16 : msg identifier from t_eAPPSIG_SrlMsgList or t_eAPPSIG_CanMsgList
     * @param[in] f_origine_e : CAN or SERIAL msg identifier
     */
   typedef void (t_cbAPPSIG_SignalRcvCallback)(t_eAPPSIG_Signal f_signal_e, t_float32 f_sigValue_f32);
   /**
     * @brief Callback user to be 
     noticed whenever a reception
     *         of a certain msg has arrived
     * @param[in] f_msgID_u16 : msg identifier from t_eAPPSIG_SrlMsgList or t_eAPPSIG_CanMsgList
     * @param[in] f_origine_e : CAN or SERIAL msg identifier
     */
       typedef void (t_cbAPPSIG_MsgRcvCallback)(t_uint16 f_msgID_u16,
                                                t_uint8 f_nbSignal_u8,
                                                t_eAPPSIG_Signal *f_signal_ae, 
                                                t_float32 *f_sigValue_af32);
   //-----------------------------ENUM TYPES-----------------------------//
    /* CAUTION : Automatic generated code section for Enum: Start */

    /* CAUTION : Automatic generated code section for Enum: End */

    //-----------------------------STRUCT TYPES---------------------------//

    /* CAUTION : Automatic generated code section for Structure: Start */

    /* CAUTION : Automatic generated code section for Structure: End */

    // ********************************************************************
    // *                      Prototypes
    // ********************************************************************
        
    // ********************************************************************
    // *                      Variables
    // ********************************************************************
    /** @brief Initializes the signal-management module. @return Module initialization status. */
    t_eReturnCode APPSIG_Init(void);
    /** @brief Executes the cyclic signal-management processing. @return Processing status. */
    t_eReturnCode APPSIG_Cyclic(void);
    /** @brief Gets the module lifecycle state. @param[out] f_State_pe Destination for the current state. @return Status; RC_ERROR_PTR_NULL if f_State_pe is NULL. */
    t_eReturnCode APPSIG_GetState(t_eCyclicModState *f_State_pe);
    /** @brief Sets the module lifecycle state. @param[in] f_State_e Requested state. @return Status of the transition. */
    t_eReturnCode APPSIG_SetState(t_eCyclicModState f_State_e);
    /** @brief Stores a value for a configured signal. @param[in] f_signal_e Signal identifier. @param[in] f_value_f32 Value to store. @return Status of the update. */
    t_eReturnCode APPSIG_SetSignalValue(t_eAPPSIG_Signal f_signal_e, t_float32 f_value_f32);
    /** @brief Reads the last value of a configured signal. @param[in] f_signal_e Signal identifier. @param[out] f_value_pf32 Destination for the value. @return Status of the read. */
    t_eReturnCode APPSIG_GetSignalValue(t_eAPPSIG_Signal f_signal_e, t_float32 * f_value_pf32);
   /** @brief Requests transmission of a configured message. @param[in] f_origin_e Message transport origin. @param[in] f_msgID_u16 Configured message identifier. @return Status of the request. */
   
   t_eReturnCode APPSIG_ForceMsgSend(t_eAPPSIG_MsgOrigin f_origin_e, t_uint16 f_msgID_u16);
   /** @brief Registers a callback for a received signal. @param[in] f_signal_e Signal identifier. @param[in] f_msgRcvCallback_pcb Callback to invoke on reception. @return Registration status. */
   t_eReturnCode APPSIG_AddRcvSigCallback(t_eAPPSIG_Signal f_signal_e, 
                                          t_cbAPPSIG_SignalRcvCallback * f_msgRcvCallback_pcb);
   /** @brief Registers a callback for a received message. @param[in] f_msgID_u16 Message identifier. @param[in] f_msgOrigin_e Message transport origin. @param[in] f_msgRcvCallback_pcb Callback to invoke on reception. @return Registration status. */
   t_eReturnCode APPSIG_AddRcvMsgCallback(  t_uint16 f_msgID_u16,
                                            t_eAPPSIG_MsgOrigin f_msgOrigin_e,
                                            t_cbAPPSIG_MsgRcvCallback * f_msgRcvCallback_pcb);
   //********************************************************************************
   //                      Public functions - Prototyupes
   //********************************************************************************
     
 #endif // LIBRAMP_H_INCLUDED
 //************************************************************************************
 // End of File
 //************************************************************************************
 
 /**
  *	@brief
  *	@note   
  *
  *
  *	@param[in] 
  *	@param[in]
  *	 
  *
  *
  */
 
