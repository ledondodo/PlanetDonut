# PlanetDonut
Planet Donut is a simulation of robots searching for ressources on a "donut-shaped planet", such as the map is looping on itself.
This was my second C++ programming project at EPFL, in Bachelor Microengineering.

## Launch the program
First open the directory using the terminal, and type "make". It will create the executable file, and update the dependancies.
To launch the program type "./projet".
To launch the program and open the map XX type "./projet maps/mapXX". You can find all the maps in the folder "maps".

## How it works ?
As you launch the program, it displays a new window.

On the left you got a set of buttons to interact with the program.
You can "open" a new map, "save" the current map or "exit" the program.
As you load a map, you can "start" the simulation (updates continuously), "stop" it, or update the simulation step by step with the "step" button. (the step function only works if the simulation is stopped)
You can display the link between every robots with "toggle link", and the range of each robots with "toggle range".

On the bottom you can read the advencement of each base. You can get the number of each type of robots, and the level of ressources stored.

On the right you can see the map of the simulation.

## Create your own map
If you want to create your own map and try a simulation, you can refer to the document [planetdonut](planetdonut.pdf) page 8 wich details how to write the .txt file.

## Libraries
This project uses GTKmm graphic interface.

## License
[MIT License](LICENSE)
