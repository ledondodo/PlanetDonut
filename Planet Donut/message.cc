// message.cc - v2
// Arthur CHANSEL - 324265
// Benoît VIGNON - 327737

#include "message.h"

std::string message::identical_robot_uid(unsigned uid)
{
    return std::string("Error: Two robots of the same base have identical uid: ")
           + std::to_string(uid) + std::string("\n");
}

std::string message::field_superposition(double x1, double y1, double x2, double y2)
{
    if(x1 > x2 || (x1 == x2 && y1 > y2))
    {
        double tmpx1(x1), tmpy1(y1);
        x1 = x2;
        y1 = y2;
        x2 = tmpx1;
        y2 = tmpy1;
    }

    return std::string("Error: Two resource fields are in superposition. Their centers"
                       " are in (")
           + std::to_string(x1) + std::string(";") + std::to_string(y1)
           + std::string(") and in (") + std::to_string(x2) + std::string(";")
           + std::to_string(y2) + std::string(")\n");
}

std::string message::base_superposition(double x1, double y1, double x2, double y2)
{
    if(x1 > x2 || (x1 == x2 && y1 > y2))
    {
        double tmpx1(x1), tmpy1(y1);
        x1 = x2;
        y1 = y2;
        x2 = tmpx1;
        y2 = tmpy1;
    }

    return std::string("Error: Two bases are in superposition. Their centers are in (")
           + std::to_string(x1) + std::string(";") + std::to_string(y1)
           + std::string(") and in (") + std::to_string(x2) + std::string(";")
           + std::to_string(y2) + std::string(")\n");
}

std::string message::base_field_superposition(double base_x, double base_y,
                                              double x_field, double y_field)
{
    return std::string("Error: Base at (") + std::to_string(base_x) + std::string(";")
           + std::to_string(base_y) + std::string(") and resource field at (")
           + std::to_string(x_field) + std::string(";") + std::to_string(y_field)
           + std::string(") are in superposition\n");
}

std::string message::missing_robot_communication(double base_x, double base_y)
{
    return std::string("Base at (") + std::to_string(base_x) + std::string(";")
           + std::to_string(base_y)
           + std::string(") does not have a communication robot at its center\n");
}

std::string message::success()
{
	return std::string("Correct file\n");
}

