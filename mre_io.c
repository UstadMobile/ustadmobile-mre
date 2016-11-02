/**
	MRE IO: File operations, etc 
**/

#include "share.h"
#include "mre_io.h"

/*****************************************************************************
 * FUNCTION
 *  mre_get_drv
 * DESCRIPTION
 *  This function returns a no corresponding to a drive found
 * PARAMETERS
 *  none
 * RETURNS
 *  g_mre_drv            [OUT]      drive found on device
*****************************************************************************/

VMINT mre_get_drv (void)
{
    return g_mre_drv;
}

/*****************************************************************************
 * FUNCTION
 *  mre_set_drv
 * DESCRIPTION
 *  This function stores a no corresponding to a drive found
 * PARAMETERS
 *  none
 * RETURNS
 *  none
*****************************************************************************/
void mre_set_drv (void)
{
    VMINT drv;
    vm_log_debug ("Entering mre_set_drv function");
    if ( (drv = vm_get_removable_driver ()) <0 )
    {
        drv = vm_get_system_driver ();
    }
    g_mre_drv = drv;
    vm_log_debug ("Exiting mre_set_drv function");
}



