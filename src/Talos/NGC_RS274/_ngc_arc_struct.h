
#ifndef NGC_ARC_STRUCT_H
#define NGC_ARC_STRUCT_H
struct s_arc_values
{
	s_axis_property horizontal_offset;
	s_axis_property vertical_offset;
	s_axis_property normal_offset;
	e_parsing_errors plane_error;
	float *Radius;
};

#endif