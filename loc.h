//
// Created by flasque on 19/10/2024.
//

#ifndef UNTITLED1_LOC_H
#define UNTITLED1_LOC_H

/**
 * @brief Enum for the orientation of the robot (NORTH, EAST, SOUTH, WEST)
 */
typedef enum e_orientation
{
    NORTH,
    EAST,
    SOUTH,
    WEST
} t_orientation;

/**
 * @brief Structure for the position of the robot
 */
typedef struct e_position
{
    int x;
    int y;
} t_position;

/**
 * @brief Structure for the localisation of the robot
 */
typedef struct s_localisation
{
    t_position      pos;
    t_orientation   ori;
} t_localisation;

/**
 * @brief Function to initialise the localisation of the robot
 * @param x : the x position of the robot
 * @param y : the y position of the robot
 * @param ori : the orientation of the robot
 * @return the localisation of the robot
 */
t_localisation loc_init(int, int, t_orientation);

/**
 * @brief Function to check a valid position for the robot
 * @param loc : the localisation of the robot
 * @param x_max : the maximum x position
 * @param y_max : the maximum y position
 * @return 1 if the position is valid, 0 otherwise
 */
int isValidLocalisation(t_position, int, int);

/**
 * @brief get the LEFT position of a position
 * @param pos : the position
 * @return the LEFT position
 */
t_position LEFT(t_position);

/**
 * @brief get the RIGHT position of a position
 * @param pos : the position
 * @return the RIGHT position
 */
t_position RIGHT(t_position);

/**
 * @brief get the UP position of a position
 * @param pos : the position
 * @return the UP position
 */
t_position UP(t_position);

/**
 * @brief get the DOWN position of a position
 * @param pos : the position
 * @return the DOWN position
 */
t_position DOWN(t_position);

#endif //UNTITLED1_LOC_H
