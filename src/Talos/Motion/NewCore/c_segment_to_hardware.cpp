/*
* c_processor.cpp
*
* Created: 5/8/2019 2:18:48 PM
* Author: Family
*/

#include "../../Configuration/c_configuration.h"
#include "c_segment_to_hardware.h"
#include "c_ngc_to_block.h"
#include "c_block_to_segment.h"
#include "c_state_control.h"
#include "c_hardware_out.h"
#include <math.h>

#ifdef MSVC
#include <iostream>
#include <fstream>

using namespace std;
static ofstream myfile;

#endif // MSVC

namespace mtn_cfg = Talos::Configuration::Motion;
namespace int_cfg = Talos::Configuration::Interpreter;
namespace mtn_ctl_sta = Talos::Motion::Core::States;
namespace mot_dat = Talos::Motion::Core::Input;
namespace seg_dat = Talos::Motion::Core::Process;
namespace hrd_out = Talos::Motion::Core::Output;


//#define HAL_SET_DIRECTION_PINS()
//#define HAL_RELEASE_DRIVE_BREAKS()


namespace Talos
{
	namespace Motion
	{
		namespace Core
		{
			namespace Output
			{
				/*
				Before interpolation can begin:

				What feed mode are we in? is this a time based motion that we can use a timer on, or
				is it dependent on other components like spindle rpm

				Do we have to wait on other hardware? Spindle up to speed, servo brakes released, etc.
				*/

				//default the gate to new motion.
				void(*Segment::pntr_next_gate)(void) = Segment::__new_motion;
				void(*Segment::pntr_driver)(void) = NULL;
				static uint32_t total_steps = 0;

				Segment::s_persisted Segment::_persisted{};

				void Segment::gate_keeper()
				{
					//This should always point to something... 
					if (Segment::pntr_next_gate != NULL)
						Segment::pntr_next_gate();
				}

				void Segment::__new_motion()
				{

#ifdef MSVC
					myfile.open("acceleration.txt");
#endif // MSVC
					//__configure_spindle();
					//__release_brakes();
					__motion_start();
				}

				void Segment::__configure_spindle()
				{
					//if there is a spindle record in the buffer, it was put there to run with the current
					//motion block. the same spindle settings may persist over several motion blocks. When
					//a new record appears in the spindle buffer, that indicates a change to the spindle
					//configuration was detected when the block was processed and that spindle record should
					//be processed before the motion block.
					if (!mot_dat::Block::spindle_buffer.has_data())
						return;

					memcpy(&_persisted.spindle_block, mot_dat::Block::spindle_buffer.get(), sizeof(__s_spindle_block));

					hrd_out::Hardware::Spindle::start(_persisted.spindle_block);

					if (_persisted.spindle_block.states.get(e_spindle_state::synch_with_motion))
					{
						//call the synch function
						__spindle_wait_synch();
						//point the gate keeper to synch function so that it calls over and over until
						//1. the timeout waiting for spindle synch, and an error occurs
						//2. the spindle comes on and reaches the target speed.
						Segment::pntr_next_gate = __spindle_wait_synch;
					}
				}

				void Segment::__spindle_wait_synch()
				{
					//loop until timeout or spindle reaches speed

					//@speed start motion
					uint32_t rpm = hrd_out::Hardware::Spindle::get_speed(_persisted.spindle_block);
					if (rpm = _persisted.spindle_block.rpm)
					{
						//Segment::pntr_next_gate = __start_motion;
					}

					//@timeout set error
					//Segment::pntr_next_gate = __spindle_wait_synch;
				}

				void Segment::__motion_start()
				{
					/*
					Clear the gate keeper so that no new calls come in here.
					The motion will execute everything thats coming in, as it
					comes in. We do not need to 'start' motion again until:
					1. The feed mode changes to something requiring a different
					drive mode. Such as spindle synch. Motion will appear to
					just continue to the operator, but in reality we have:
						a.'stopped'
						b. 'reconfigured the spindle for the new feed mode
						c. started the motion again, all within about 5us
					2. All motion is executed and the interpollation 'stops'.
					At the stop point, the gatekeeper is reset so that new motions
					can be started again.

					Effectively this will run as one continuous motion until 1 of
					the 2 conditions above are met.
					*/

					Segment::pntr_next_gate = NULL;

					//The timer isr will call 'driver' 
					Segment::pntr_driver = __run_interpolation;

					mtn_ctl_sta::Output::states.set(mtn_ctl_sta::Output::e_states::interpolation_running);
					//start the timer, this will set all of the wheel in motion
					//hrd_out::Hardware::Motion::initialize();

					hrd_out::Hardware::Motion::enable();
					hrd_out::Hardware::Motion::start();
					//null gate keeper??? not sure what to do yet... 
					//Segment::pntr_next_gate = NULL;

				}

				void Segment::__release_brakes()
				{
					//release output drive breaks
					hrd_out::Hardware::Motion::brakes(0);

					//The breaks usually have some kind of delay. we need to determine
					//how long the delay should be, set the timer to that period and
					//re-point the next gate to the next logical step.
					//Segment::pntr_next_gate = Segment::__run_interpolation;
				}

				void Segment::__run_interpolation()
				{
					//this has been called from an ISR or thread

					//Send outputs to physical hardware
					hrd_out::Hardware::Motion::step(&_persisted.step_outbits);

					if (Segment::_persisted.active_timer_item == NULL)
					{
						bool end = __load_segment();
						//were there any more segments? if not we are done.
						if (end)
						{
							__end_interpolation();
							return;
						}
					}
					//seems stable to here.
					// Reset step out bits.
					_persisted.step_outbits = 0;
					for (int i = 0; i < MACHINE_AXIS_COUNT; i++)
					{
						// Execute step displacement profile by Bresenham line algorithm
						_persisted.bresenham_counter[i] += _persisted.active_timer_item->common.bres_obj->steps[i];
						if (_persisted.bresenham_counter[i] >
							_persisted.active_timer_item->common.bres_obj->step_event_count)
						{
							_persisted.step_outbits |= (1 << i);
							_persisted.bresenham_counter[i] -=
								_persisted.active_timer_item->common.bres_obj->step_event_count;

							//We arent changing system position on a backlash comp motion.
							if (!_persisted.active_timer_item->common.control_bits.system.get(e_system_block_state::block_state_skip_sys_pos_update))
							{
								/*if (active_timer_item->common.bres_obj->direction_bits._flag & (1 << i))
									Motion_Core::Hardware::Interpolation::system_position[i]--;
								else
									Motion_Core::Hardware::Interpolation::system_position[i]++;*/
							}
						}
					}

					_persisted.active_timer_item->steps_to_execute_in_this_segment--;
					total_steps++;
					//If feedmode is spindle synch, calculate the correct delay value for
					//the feedrate, based on spindle current speed
					//TODO: is there a better way to do this without several if statements?
					//if (_persisted.active_timer_item->common.control_bits.feed.get(
					//	e_feed_block_state::feed_mode_units_per_rotation))
					//{
					//	//only adjust the delay value if we are in 'cruise' state.
					//	//The arbitrator still controls motion during accel and decel
					//	if (_persisted.active_timer_item->common.control_bits.speed.get(
					//		e_speed_block_state::motion_state_cruising))
					//	{
					//		//Hardware_Abstraction_Layer::MotionCore::Stepper::OCR1A_set
					//		//(Hardware_Abstraction_Layer::MotionCore::Spindle::spindle_encoder->feedrate_delay);
					//	}
					//}

					if (_persisted.active_timer_item->steps_to_execute_in_this_segment == 0)
					{
						//We are done with this timer item. Calling a get will move the indexes so the 
						//buffer is not full if it was full before.
						seg_dat::Segment::timer_buffer.get();
						_persisted.active_timer_item = NULL;

					}
					//_persisted.step_outbits ^= Motion_Core::Hardware::Interpolation::step_port_invert_mask;  // Apply step port invert mask

				}

				bool Segment::__load_segment()
				{
					bool done = true;

					if (seg_dat::Segment::timer_buffer.has_data())
					{
						//dont 'get' here otherwise the item we are pointing at will get over written
						//by the segment loader in block_to_segment
						_persisted.active_timer_item = seg_dat::Segment::timer_buffer.peek();
					}
					else
						int x = 0;


					if (_persisted.active_timer_item != NULL)
					{
						done = false;
						//does this new timer indicate a feed mode change
						if (_persisted.active_timer_item->common.control_bits.feed.get(e_feed_block_state::feed_mode_change))
						{
#ifdef MSVC
							myfile << "*****feed mode change*****" << "\r";
#endif
							//does the change require spindle synch
							if (_persisted.active_timer_item->common.control_bits.feed.get(
								e_feed_block_state::feed_mode_units_per_rotation))
							{
#ifdef MSVC
								myfile << "*****units per rotation*****" << "\r";
#endif
								//configure for new feed mode
							}
							else
							{
#ifdef MSVC
								myfile << "*****normal*****" << "\r";
#endif
							}
							//save off flags
							_persisted.control_bits.feed._flag =
								_persisted.active_timer_item->common.control_bits.feed._flag;
						}
#ifdef MSVC
						myfile << _persisted.active_timer_item->steps_to_execute_in_this_segment << ",";
						myfile << _persisted.active_timer_item->timer_delay_value << ",";
						myfile << _persisted.active_timer_item->common.tracking.line_number << ",";
						myfile << _persisted.active_timer_item->common.tracking.sequence << ",";
						myfile << total_steps << ",";
						myfile << "\r";
						myfile.flush();
#endif
						//set timer delay value to the active_timer objects delay time						
						hrd_out::Hardware::Motion::time_adjust(&_persisted.active_timer_item->timer_delay_value);

						// If the new segment starts a new planner block, initialize stepper variables and counters.
						// NOTE: When the segment data index changes, this indicates a new planner block.
						if (_persisted.active_bresenham != _persisted.active_timer_item->common.bres_obj)
						{
							/*
							//New line segment, so this should be the start of a block.
							mtn_ctl_sta::Output::states.set(mtn_ctl_sta::Output::e_states::ngc_block_done);
							_persisted.active_line_number = _persisted.active_timer_item->common.tracking.line_number;
							_persisted.last_complete_sequence = _persisted.active_sequence;

							mtn_ctl_sta::Output::block_stats.common.sequence = _persisted.last_complete_sequence;
							mtn_ctl_sta::Output::block_stats.common.line_number = _persisted.active_line_number;
							//mtn_ctl_sta::Output::block_stats.start_time = *Hardware_Abstraction_Layer::Core::cpu_tick_ms;
							*/
							_persisted.active_sequence = _persisted.active_timer_item->common.tracking.sequence;
							_persisted.active_bresenham = _persisted.active_timer_item->common.bres_obj;

							//if we are loading a new block directions MIGHT change, so clear the set flag
							uint16_t non_vol = _persisted.active_timer_item->common.bres_obj->direction_bits._flag;
							hrd_out::Hardware::Motion::direction(&non_vol);

							//a new block has been detected. that means there is a potential for new ngc settings
							//for the spindle. better go check
							//Segment::__configure_spindle();

							// Initialize Bresenham line and distance counters
							for (int i = 0; i < MACHINE_AXIS_COUNT; i++)
								_persisted.bresenham_counter[i]
								= _persisted.active_timer_item->common.bres_obj->step_event_count;
						}
					}
					else
						int x = 0;
					return done;
				}

				void Segment::__end_interpolation()
				{
					hrd_out::Hardware::Motion::stop();
#ifdef MSVC
					myfile << 0 << ",";
					myfile << 0 << ",";
					myfile << 0 << ",";
					myfile << _persisted.active_line_number << ",";
					myfile << _persisted.active_sequence << ",";
					myfile << total_steps << ",";
					myfile << "\r";
					myfile.flush();
					myfile.close();
#endif // MSVC
					__set_brakes();
					hrd_out::Hardware::Motion::disable();
				}

				void Segment::__set_brakes()
				{
					hrd_out::Hardware::Motion::brakes(0);

					//set this to null in case the timer fires while we are in here.
					Segment::pntr_driver = NULL;

					Segment::pntr_next_gate = Segment::__new_motion;
				}
			}
		}
	}
}
