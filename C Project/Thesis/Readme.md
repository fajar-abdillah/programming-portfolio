# Thesis Project

This is my thesis project titled "Design Subsystem of Telecommunication (TTC-Telemetry, Tracking, and Command) Based on Microcontroller for Nano-Satellite UGMSat-1".
I'm using PIC16F877A as microcontroller and C programming language to programming it using MPLAB X IDE. 
This project's aim is to process the command from ground station (computer) and send housekeeping data from OBDH subsystem to ground station.

## TTC-Programming-Master

It's the main TTC subsystem program that I made. It used to receive and process command from ground station and asking the data from OBDH subsystem.

## TT-Programming-Slave-for-OBDH-Dummy

It's de OBDH subsystem dummy that sent the data to TTC subsystem if receive the command.
