/*********************************************************************
 * @file        APPACT_ConfigPublic.h
 * @brief       Template_BriefDescription.
 * @note        TemplateDetailsDescription.\n
 *
 * @author      xxxxxx
 * @date        jj/mm/yyyy
 * @version     1.0
 */
  
#ifndef APPSIG_CONFIGPUBLIC_H_INCLUDED
#define APPSIG_CONFIGPUBLIC_H_INCLUDED

    // ********************************************************************
    // *                      Includes
    // ********************************************************************
    #include "TypeCommon.h"
    // ********************************************************************
    // *                      Defines
    // ********************************************************************

    // ********************************************************************
    // *                      Types
    // ********************************************************************
    ///@brief ENum for Signal Encoding
    typedef enum 
    {
        APPSIG_SIG_ENCODE_INTEL = 0,
        APPSIG_SIG_ENCODE_MOTOROLA,

        APPSIG_SIG_ENCODE_NB
    } t_eAPPSIG_SigEncode;
    ///@brief Enum to know where come from the signal 
    typedef enum 
    {
        APPSIG_MSG_ORIGIN_CAN = 0,
        APPSIG_MSG_ORIGIN_SRL,

        APPSIG_MSG_ORIGIN_NB
    } t_eAPPSIG_MsgOrigin;

    ///@brief Enum to know the direction of the message
    typedef enum 
    {
        APPSIG_MSG_DIR_RX = 0,          //---- msg is expected to be receive ----//
        APPSIG_MSG_DIR_TX,              //---- msg is expected to be send and received ----//
        APPSIG_MSG_DIR_RX_TX,           //---- msg is expected to receive and transmitted ----//

        APPSIG_MSG_DIR_NB
    } t_eAPPSIG_MsgDirection;

    /* CAUTION : Automatic generated code section for Enum: Start */
    /**
    * @brief Signal list available.
    */
    typedef enum
    {
        APPSIG_SIGNAL_FMKCPU_MODSTATE = 0,
        APPSIG_SIGNAL_FMKCDA_MODSTATE,
        APPSIG_SIGNAL_FMKSRL_MODSTATE,
        APPSIG_SIGNAL_FMKFDCAN_MODSTATE,
        APPSIG_SIGNAL_FMKIO_MODSTATE,
        APPSIG_SIGNAL_FMKHRT_MODSTATE,
        APPSIG_SIGNAL_FMKTIM_MODSTATE,
        APPSIG_SIGNAL_APPSIG_MODSTATE,
        APPSIG_SIGNAL_APPSYS_MODSTATE,
        APPSIG_SIGNAL_APPSNS_MODSTATE,
        APPSIG_SIGNAL_APPACT_MODSTATE,
        APPSIG_SIGNAL_APPLGC_MODSTATE,
        APPSIG_SIGNAL_APPSDM_MODSTATE,
        APPSIG_SIGNAL_APPSPM_MODSTATE,
        APPSIG_SIGNAL_CYCLIC_DURATION,
        APPSIG_SIGNAL_FASTTASKDURATION,
    
        APPSIG_SIGNAL_NB,
    } t_eAPPSIG_Signal;

    /**
    * @brief message serial available.
    */
    typedef enum
    {
        APPSIG_SRL_MSG_APPLICATIONINFO1 = 0,
    
        APPSIG_SRL_MSG_NB,
    } t_eAPPSIG_SrlMsgList;

    /**
    * @brief message can available.
    */
    typedef enum
    {
    
        APPSIG_CAN_MSG_NB,
    } t_eAPPSIG_CanMsgList;

    /* CAUTION : Automatic generated code section for Enum: End */
	//-----------------------------ENUM TYPES-----------------------------//

	/* CAUTION : Automatic generated code section for Structure: Start */

	/* CAUTION : Automatic generated code section for Structure: End */
	//-----------------------------STRUCT TYPES---------------------------//

	/* CAUTION : Automatic generated code section : Start */

	/* CAUTION : Automatic generated code section : End */
	//-----------------------------TYPEDEF TYPES---------------------------//
    // ********************************************************************
    // *                      Prototypes
    // ********************************************************************
    
    // ********************************************************************
    // *                      Variables
    // ********************************************************************
    
    

    //********************************************************************************
    //                      Public functions - Prototyupes
    //********************************************************************************

#endif // APPSDM_CONFIGPUBLIC_H_INCLUDED           
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
