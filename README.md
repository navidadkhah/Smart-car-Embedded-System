# Smart Car with RTOS and Sensor Controllers

Welcome to the **Smart Car Project**! This repository showcases a system where a smart car is built using various sensors, controllers, and an RTOS (Real-Time Operating System) to manage multiple tasks like obstacle detection, cruise control, and light management. This project was modeled using **Tinkercad** and implemented in **PlatformIO** with **FreeRTOS**.

## Table of Contents

- [About the Project](#about-the-project)
- [Features](#features)
- [Modeling](#modeling)
- [Real-Time Operating System (RTOS)](#real-time-operating-system-rtos)
- [Implementation](#implementation)
- [Unit Tests](#unit-tests)
- [Simulation](#simulation)
- [Contributing](#contributing)
- [License](#license)

## About the Project

This project focuses on designing and developing a smart car system that can:

- Follow a line and avoid obstacles.
- Manage speed through cruise control.
- Control lights based on ambient lighting.
- Monitor battery usage.
- Respond to signals such as door open/close and seat belt status.

It uses **FreeRTOS** to schedule and manage various tasks in real-time, ensuring smooth operation even with multiple simultaneous processes.

## Features

1. **Line Following and Obstacle Detection**  
   The car uses infrared sensors to follow a path and ultrasonic sensors to detect obstacles. Based on the sensor feedback, the car can adjust its course or brake.

2. **Cruise Control**  
   The car maintains a constant speed using an encoder to adjust for slopes.

3. **Light Management**  
   Lights are controlled based on ambient light using photodiodes, automatically turning on in low-light conditions.

4. **Battery Monitoring**  
   The battery's status is tracked, and the remaining energy is displayed, with warnings if energy levels are low.

5. **Seat Belt and Door Monitoring**  
   The system checks if the doors are closed and the seat belts are fastened, issuing alerts if either is unfastened.

## Modeling

The system was modeled using **Tinkercad** and **Simulink** to simulate various components before implementation. Below is a breakdown of each model used:

### Vehicle Movement Model

- **Description**: This model simulates the car's movement along a predefined path with line-following logic based on input from the IR sensors.
- **Screenshot**: *(Insert a screenshot of the vehicle movement model)*

### Cruise Control Model

- **Description**: Cruise control was modeled to maintain a constant speed, adjusting for slopes and other terrain features using encoders and speed sensors.
- **Screenshot**: *(Insert a screenshot of the cruise control model)*

### Light Management Model

- **Description**: The car's lights are controlled automatically based on ambient light levels using photodiodes, with the model adjusting light states accordingly.
- **Screenshot**: *(Insert a screenshot of the light management model)*

### Obstacle Detection Model

- **Description**: This model simulates how the car detects obstacles using ultrasonic sensors and applies varying levels of braking based on the distance to the obstacle.
- **Screenshot**: *(Insert a screenshot of the obstacle detection model)*

### Battery Monitoring Model

- **Description**: The battery monitoring system calculates the car's remaining energy based on time elapsed and displays the status on an LCD screen.
- **Screenshot**: *(Insert a screenshot of the battery monitoring model)*

## Real-Time Operating System (RTOS)

RTOS, or **Real-Time Operating System**, is a specialized operating system designed to manage hardware resources efficiently and execute tasks in real time. In this project, we use **FreeRTOS**, an open-source RTOS that ensures:

- **Multitasking**: Different tasks, like monitoring sensors, controlling motors, and managing lights, run simultaneously without interrupting each other.
- **Real-Time Scheduling**: Tasks are prioritized, ensuring critical tasks (e.g., obstacle detection) get higher priority over less critical ones (e.g., seat belt monitoring).
- **Low Latency**: The system responds quickly to external stimuli, such as obstacles or changes in lighting, thanks to the real-time execution of tasks.

Using FreeRTOS allows our smart car to operate efficiently and handle various scenarios with minimal delay.

## Implementation

This project was implemented using **PlatformIO** in **VS Code**, with FreeRTOS handling task management. The key tasks include:

1. **Line Following Task**: Monitors infrared sensors and adjusts motor speed and direction based on sensor input.
2. **Cruise Control Task**: Maintains constant speed using encoder feedback.
3. **Light Management Task**: Monitors ambient light levels and controls car lights.
4. **Obstacle Detection Task**: Detects obstacles with ultrasonic sensors and adjusts braking accordingly.
5. **Battery Monitoring Task**: Tracks battery energy levels and displays the status.

### Code Snippet
Here is an example of how tasks are created and prioritized in FreeRTOS:
```cpp
xTaskCreate(vTaskLineFollowing, "LineFollow", 100, NULL, 2, NULL);
xTaskCreate(vTaskCruiseControl, "CruiseControl", 100, NULL, 2, NULL);
xTaskCreate(vTaskLightManagement, "LightControl", 100, NULL, 1, NULL);
```

## Unit Tests

To ensure the correct functionality of the system, we used unit testing with the **Unity Testing Framework**. Each critical task, such as line following, cruise control, light management, and battery monitoring, was tested independently to verify that it performs as expected under various conditions.

### Test Cases

1. **Line Following Test**:
   - **Goal**: Verify that the car follows the line correctly using the IR sensors.
   - **Test**: Simulate different line positions and ensure the motors adjust speed and direction appropriately.
   - **Result**: *(Insert test results)*

2. **Cruise Control Test**:
   - **Goal**: Test that the car maintains a constant speed despite changes in slope.
   - **Test**: Use encoder feedback to adjust speed and simulate changes in slope.
   - **Result**: *(Insert test results)*

3. **Light Management Test**:
   - **Goal**: Ensure that the lights turn on/off based on ambient light levels.
   - **Test**: Simulate varying light conditions and check if the lights are turned on/off correctly.
   - **Result**: *(Insert test results)*

4. **Battery Monitoring Test**:
   - **Goal**: Verify that the battery levels are displayed correctly and the car shuts down when energy is depleted.
   - **Test**: Simulate energy consumption and check LCD outputs and car behavior at low energy levels.
   - **Result**: *(Insert test results)*

### Running Tests

You can run the unit tests using the following command in **PlatformIO**:
```bash
pio test
```

## Simulation

You can view the **Tinkercad simulation** of this project [here](https://www.tinkercad.com/things/1XwYA2YhJHc-finalproject?sharecode=bQ98UG3xcL_wpaDuBa7_YqRBDrAiC1OnK-2HdcJsHyM).

### Suggested Photo Locations

- **Line Following System**: Place a screenshot of the car following a line, with infrared sensors in action.  
  _(Suggested location: Above the "Line Following and Obstacle Detection" section)_
  
- **Obstacle Detection**: Add a photo of the car detecting an obstacle and braking.  
  _(Suggested location: Under the "Obstacle Detection Task" description)_
  
- **Light Management**: Insert an image showing the lights turning on in low-light conditions.  
  _(Suggested location: Near the "Light Management Task" explanation)_

## Contributing

We welcome contributions! Feel free to fork this project and submit pull requests.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
