
#include "../../../../Coordinator/Processing/Main/Coordinator_Process.h"
#include "../../../../Motion/Processing/Main/Motion_Process.h"
#include "../../../../Shared_Data/Kernel/Base/c_kernel_base.h"
#include "../../../../talos_hardware_def.h"

using namespace Talos;
void run(void);
int main(void)
{
	//Start the 'core'. 
	//this will configure the board (unless you are runnign via windows)
	//so that needed peripherals and io is ready.
	Hardware_Abstraction_Layer::Core::initialize();

	Kernel::Base::f_initialize();
	//init framework comms (not much going on in here yet)
	Kernel::Comm::f_initialize(
		NULL,
		NULL,
		NULL,
		NULL,
		NULL);
	//init framwork cpus (assign an ID number to each cpu object. Init the data buffers
	Kernel::CPU::f_initialize(
		HOST_CPU_ID, CORD_CPU_ID, MACH_CPU_ID, SPDL_CPU_ID, PRPH_CPU_ID, Hardware_Abstraction_Layer::Core::cpu_tick_ms);
	Kernel::Comm::print(0, "Hello\r\n");
	Coordinator::Main_Process::cord_initialize();
	Motion::Main_Process::mot_initialize();

	Kernel::Comm::host_ring_buffer.put("g0x100\r\n\0");

	run();
}

bool reload = false;
char test_line[50] = "g0x30\r\n\0";
void run(void)
{
	uint8_t state = 0;

	while (state == 0)
	{
		if (reload)
			Kernel::Comm::host_ring_buffer.put(test_line);

		Coordinator::Main_Process::cord_run();
		Motion::Main_Process::mot_run();
	}
}