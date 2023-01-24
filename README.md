Overview
This repository contains a program that calculates the reliability of a complex system represented by an electrical circuit. The program is written in C++ and uses a combination of mathematical modeling and Monte Carlo simulation to calculate the probability that the system will function correctly.

File Structure
main.cpp: This file contains the main function that runs the program.
CircuitModel.h: This file contains the declaration of the OrigCircuit class, which represents the electrical circuit and its various components.
CircuitModel.cpp: This file contains the implementation of the OrigCircuit class.
Redistribution.h: This file contains the declaration of the Redistribution class, which is responsible for redistributing failed processors in the circuit in order to increase system reliability.
Redistribution.cpp: This file contains the implementation of the Redistribution class.
Reliable.h: This file contains the declaration of the Reliable class, which is responsible for calculating the system's reliability.
Reliable.cpp: This file contains the implementation of the Reliable class.
How it works
The program starts by creating an instance of the OrigCircuit class, which is used to represent the electrical circuit and its various components. The program then creates an instance of the Reliable class, which is used to calculate the system's reliability. The Reliable class uses mathematical modeling and Monte Carlo simulation to generate a large number of random system states and to calculate the probability that the system will function correctly in each state.

How to run
The program is written in C++ and requires a C++ compiler to run. It can be executed by running the command in terminal:

Copy code
g++ -std=c++14 main.cpp OrigCircuit.cpp Reliable.cpp Redistribution.cpp -o main
./main

Configuration
The number of modules and the number of connections in the circuit can be configured in the OrigCircuit class. The probability of failure for each type of module can be configured in the Reliable class.

Limitations
The program is designed to work with a specific type of electrical circuit and may not be suitable for other types of systems. Additionally, the program uses a simplified mathematical model to calculate the system's reliability, which may not be accurate for all types of systems.

todo: update readme