
#include "extern_events_types.h"
//s_data_events Talos::Shared::FrameWork::Events::extern_data_events;
s_ancillary_events Talos::Shared::FrameWork::Events::extern_ancillary_events;
s_system_events Talos::Shared::FrameWork::Events::extern_system_events;
void(*Talos::Shared::FrameWork::Error::Handler::extern_pntr_error_handler)(c_ring_buffer<char> * buffer, s_framework_error error);
void(*Talos::Shared::FrameWork::Error::Handler::extern_pntr_ngc_error_handler)(char * ngc_line, s_framework_error error);
//void(*Talos::Shared::FrameWork::Error::Handler::extern_test)();