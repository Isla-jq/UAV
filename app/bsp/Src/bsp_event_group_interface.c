#include "bsp_event_group_interface.h"

static EventGroupHandle_t xCreatedEventGroup;

void bsp_event_group_init() {
    xCreatedEventGroup = xEventGroupCreate();
    if( xCreatedEventGroup == NULL )
    {
        return;
    }
}


EventGroupHandle_t bsp_get_event_group() {
    return xCreatedEventGroup;
}