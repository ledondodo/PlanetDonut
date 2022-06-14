// message.h - v2
// Arthur CHANSEL - 324265
// Benoît VIGNON - 327737

#ifndef MESSAGE_H_INCLUDED
#define MESSAGE_H_INCLUDED

#include <string>

/**
 * Module: message
 * This module is used to generate the strings
 * for you to have consistent error messages.
 * 
 * please note that "gisement" is translated by "field" in the messages
 *
 * the last string documents a successful reading
 */

namespace message
{
	// Two robots belonging to the same base have the same uid
	std::string identical_robot_uid(unsigned uid);

	// Two resource fields overlap [print center coordinates]
	std::string field_superposition(double x1, double y1, double x2, double y2);

	// Two bases overlap [print center coordinates]
	std::string base_superposition(double x1, double y1, double x2, double y2);

	// A base overlaps a resource field
	std::string base_field_superposition(double base_x,  double base_y, 
										 double x_field, double y_field);

	// A base does not have a communication robot at its center
	std::string missing_robot_communication(double base_x, double base_y);

	// Everything went well => file reading and all validation checks
	std::string success();
}

#endif // MESSAGE_H_INCLUDED
