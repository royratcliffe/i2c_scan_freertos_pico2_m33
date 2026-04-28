# Multicore SMP I^2^C Scanning Example for FreeRTOS on Raspberry Pi Pico (Cortex-M33)

TL;DR? Run FreeRTOS on both cores of the Raspberry Pi Pico to perform efficient I^2^C scanning, gather run time statistics using DWT, and manage memory with Heap-4.
The example code adds a visual indicator to the output of the I^2^C scan to show which core is executing the task. If the current core is core 0, it prints two spaces; if it's core 1, it prints two underscores. This allows the Serial Monitor to see which core is running the task at any given time, especially when the output is interleaved with other tasks that may be running on different cores.

``` text
I2C1 Bus Scan
   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
00 .  .  .  .  .__.__.__.  .  .  .  .__.__.__.  .
10 .__.__.__.__.  .  .  .__.__.__.  .  .  .__.__.
20 .  .  .  .__.__.__.  .  .  .  .__.__.__.  .  .
30 .__.__.__.  .  .  .__.__.__.  .  .  .__.__.__.
40 .  .  .__.__.__.  .  .  .  .__.__.__.  .  .  .
50 .__.  .  .  .  .__.__.__.  .  .  .__@__.__.  .
60 .  .__.__.__.  .  .  .__@__.__.__.  .  .  .__.
70 .  .  .  .__.__.__.__.  .  .  .__.__.__.__.  .
```

It looks a little bit chaotic. In fact, it almost looks like there are two separate tasks running the same scan function, one on each core. But there is only one task; it is just being scheduled on both cores by the FreeRTOS scheduler.
This is a key aspect of FreeRTOS's multi-core scheduling capabilities, allowing tasks to run on any available core without being pinned to a specific one, which can help with load balancing and improving overall system performance.

## Overview
This example demonstrates how to implement a multicore Symmetric Multiprocessing (SMP) configuration using FreeRTOS on the Raspberry Pi Pico, which is based on the dual-core Cortex-M33 microcontroller. The example focuses on performing I^2^C scanning across multiple devices while leveraging the capabilities of both cores to enhance performance and efficiency. The example also showcases the use of the Data Watchpoint and Trace (DWT) unit for gathering run time statistics, and it integrates FreeRTOS into the build process using CMake's FetchContent module. Additionally, the example utilises the Heap-4 memory management scheme provided by FreeRTOS to manage dynamic memory allocation effectively.

## Key Features
- **SMP Configuration**: Utilises both cores of the Raspberry Pi Pico to run FreeRTOS tasks concurrently, allowing for efficient I^2^C scanning.
- **DWT for Run Time Statistics**: Demonstrates how to use the DWT unit to gather high-resolution run time statistics for FreeRTOS tasks, aiding in performance analysis and optimisation.
- **FreeRTOS Build-Time Integration**: Integrates FreeRTOS into the build process using CMake's FetchContent module, simplifying dependency management and ensuring easy updates to FreeRTOS.
- **Heap-4 Memory Management**: Utilises the Heap-4 memory management scheme for efficient dynamic memory allocation and deallocation, minimising fragmentation in embedded applications.

## SMP Configuration
This example is configured to run in Symmetric Multiprocessing (SMP) mode on the Raspberry Pi Pico, which features a dual-core Cortex-M33 microcontroller. The SMP configuration allows both cores to run FreeRTOS tasks concurrently, enabling efficient use of the available processing power for tasks such as I^2^C scanning. The example demonstrates how to set up FreeRTOS in SMP mode, create tasks that run on both cores, and manage shared resources effectively to perform I^2^C scanning across multiple devices.

## DWT
This example demonstrates how to use the Data Watchpoint and Trace (DWT) unit on the Cortex-M33 to gather run time statistics for tasks in FreeRTOS. The DWT provides a high-resolution cycle counter used to measure the execution time of tasks, which is essential for performance analysis and optimisation in real-time applications.

## FreeRTOS Build-Time Integration
The example utilises CMake's FetchContent module to integrate FreeRTOS into the build process. The FetchContent approach simplifies dependency management compared to using a Git submodule. The CMake configuration is set up to fetch the FreeRTOS source code from its official repository, ensuring that a specific tagged version or commit hash is used. This method also allows for easy updates to FreeRTOS without needing to manually manage submodules, making it a more efficient and streamlined approach for incorporating FreeRTOS into a project.

## FreeRTOS Heap-4 Memory Management
The example uses the Heap-4 memory management scheme provided by FreeRTOS. Heap-4
is a simple and efficient memory allocator that combines the best features of the other heap schemes. It allows for dynamic memory allocation and deallocation while minimising fragmentation. Heap-4 is suitable for applications that require a balance between performance and memory usage, making it an ideal choice for embedded systems like the Raspberry Pi Pico. In this example, Heap-4 is configured to manage the memory used by FreeRTOS tasks, queues, and other kernel objects.
