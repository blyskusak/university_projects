# City Transit Simulation

## Project Description
This project is an object-oriented simulation of city traffic written in Java (package `SimulationOfTrams`). The program models the daily operation of tram lines and passenger behavior using a custom-built event queue. It was created as an academic project focusing on Object-Oriented Programming principles, algorithmic time management, and extensibility.

## Core Mechanics
* **Trams and Lines:** Trams operate on fixed routes from 6:00 AM until late evening. They travel in both directions, making scheduled stops and respecting layover times at terminii. 
* **Passengers:** Each passenger has a designated home stop. Every day, they leave home at a random time between 6:00 AM and 12:00 PM. Upon boarding a tram, they choose a random destination along the route. If a stop is full, passengers abandon their trip for the day or continue riding if they are already inside a tram.
* **Event-Driven Architecture:** The core timeline is managed by a custom Priority Queue. Entities (passengers, trams) schedule future actions (e.g., "arrive at stop", "leave home") as events, which are processed in chronological order.
* **Custom Time Management:** The simulation uses a dedicated `Time` class to handle virtual days, hours, and minutes independently of real-time execution.

## Project Structure (Key Classes)
* `Main` / `Simulate`: The entry point and the main engine managing the simulation loop, data initialization, and daily resets.
* `PriorityQueue` & `PriorityQueueElement`: A custom event queue implementation using a dynamically resizing array and custom sorting logic based on event time and priority.
* `Passenger`: Handles individual passenger states (waiting, riding, returning home).
* `Vehicle` (Abstract) & `Tram`: `Vehicle` serves as an abstract base class tracking common properties (ID, direction, schedule, passenger array), allowing for easy future expansion (e.g., adding buses). `Tram` implements specific movement and passenger exchange logic.
* `TramLine` & `TramStop`: Core transit entities modeling the physical world. `TramStop` manages its daily capacity limits using a fixed-size array that resets at the end of each simulated day.
* `Losowanie`: A dedicated wrapper for random number generation, ensuring deterministic or easily controllable randomness.

## Technical Requirements and Limitations
* **No Standard Collections:** In accordance with the assignment requirements, data storage relies strictly on standard Java arrays. Standard Java Collections (like `ArrayList` or `PriorityQueue`) were deliberately avoided, apart from utilizing `Arrays.sort()` for the custom event queue.
* **Input Data:** Simulation parameters are provided via standard input using the `java.util.Scanner` class.

## Program Output
Upon execution, the program prints out the simulation details to the standard output:
1. Initial simulation parameters.
2. A detailed, chronological event log (e.g., `Day 0 6:15: Passenger 4 got in Tram 0...`).
3. End-of-day summaries and final simulation statistics, including:
   * Total number of completed passenger rides.
   * Total and average waiting times.
