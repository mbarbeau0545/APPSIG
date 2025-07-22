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
    /**
     *	@brief
    *	@note   
    *
    */
    t_eReturnCode APPSIG_Init(void);
    /**
     *	@brief
    *	@note   
    *
    *
    */
    t_eReturnCode APPSIG_Cyclic(void);
    /**
    *
    *	@brief
    *	@note   
    *
    *
    *	@param[in] 
    *
    */
    t_eReturnCode APPSIG_GetState(t_eCyclicModState *f_State_pe);
    /**
    *
    *	@brief
    *	@note   
    *
    *
    *	@param[in] 
    *
    */
    t_eReturnCode APPSIG_SetState(t_eCyclicModState f_State_e);
    /**
    *
    *	@brief
    *	@note   
    *
    *
    *	@param[in] 
    *
    */
    t_eReturnCode APPSIG_SetSignalValue(t_eAPPSIG_Signal f_signal_e, t_float32 f_value_f32);
    /**
    *
    *	@brief
    *	@note   
    *
    *
    *	@param[in] 
    *
    */
    t_eReturnCode APPSIG_GetSignalValue(t_eAPPSIG_Signal f_signal_e, t_float32 * f_value_pf32);

    t_eReturnCode APPSIG_AddSomeToQueue(t_uint8 *f_data_pu8);
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
 