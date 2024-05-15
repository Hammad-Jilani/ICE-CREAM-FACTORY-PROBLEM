# Ice Cream Factory Optimization Project
## Overview
The Ice Cream Factory Optimization Project aims to optimize ice cream production in factories by implementing multi-threading techniques to manage concurrency effectively, prevent errors, and improve efficiency. The project explores synchronization mechanisms such as semaphores and mutexes to synchronize processes, prevent errors, and create a smoother production line.

## Features
* **Temperature Control**: Simulate temperature fluctuations within the factory environment to ensure optimal production conditions.
* **Inventory Management**: Track ingredient levels and freezer capacity to prevent shortages and optimize production.
* **Production Scheduling**: Handle peak demand scenarios by adjusting production schedules and prioritizing flavors.
* **Quality Control**: Implement mechanisms to ensure the quality of produced ice cream, including flavor matching and packaging checks.
* **Cleaning and Sanitation**: Model cleaning cycles for production equipment while managing thread access to maintain efficiency.
* **Custom System Calls**: Implement custom system calls for wait and signal functionalities to synchronize processes effectively.
## Test Cases
The project includes test cases to evaluate the system's performance in various scenarios, including:

* **Temperature Monitoring and Control**: Simulate temperature fluctuations and evaluate the system's response.
* **Peak Demand**: Test the system's ability to handle sudden surges in demand for specific flavors.
* **Resource Management**: Evaluate how the system handles ingredient shortages, packaging mismatches, and limited storage space.
* **Unexpected Events**: Simulate power outages and assess the system's ability to maintain data consistency and resume production.
* **Additional Considerations**: Test the system's performance in managing cleaning cycles, stock management, and production limits.
## Usage
1. Clone the repository:\br
git clone https://github.com/yourusername/ice-cream-factory.git
2. Compile the code:\br
gcc -o ice_cream_factory ice_cream_factory.c -lpthread
3. Run the executable:\br
./ice_cream_factory
## Contributions
Contributions to the Ice Cream Factory Optimization Project are welcome! If you encounter any issues or have suggestions for improvements, feel free to open an issue or submit a pull request.

## License
This project is licensed under the MIT License. See the LICENSE file for details.
