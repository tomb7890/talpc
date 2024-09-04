[![Build Status](https://travis-ci.com/tomb7890/talpc.svg?branch=master)](https://travis-ci.com/tomb7890/talpc) 
## C++ Coding Projects

This repository showcases my solutions to two C++ programming projects, focusing on data representation, manipulation, and file parsing. 

### 1. Fixed Point Class

**Objective:** Implement a C++ class to represent and perform operations on fixed-point numbers using a custom 32-bit encoding.

**Features:**

- Constructors for creating fixed-point objects from floating-point values and packed 32-bit integers.
- Conversion to string representation (e.g., "1.234").
- Conversion to the nearest floating-point equivalent.

**Implementation Details:** 
-  While this initial implementation is functional, I plan to add more detailed comments to `FixedPoint.h` and `FixedPoint.cpp`, explaining the design choices, algorithms, and potential optimizations in the future.

### 2. Text File Parser

**Objective:** Develop a C++ class to parse and modify configuration files with a specific section-based format.

**Features:**

- File loading and parsing.
- Retrieval of string, integer, and floating-point values by section and key.
- Setting and writing new values for specific keys, preserving the original formatting.

**Testing:**
- Includes a comprehensive suite of unit tests using Google Test to validate functionality and ensure robustness.

**Implementation Details:**
- Refer to `ConfigParser.h` and `ConfigParser.cpp` for insights into the parsing logic and file-writing strategies. I'll be adding more detailed comments in the future to further enhance code understanding.

###  Future Improvements

- **Error Handling:** Improve error handling by using exceptions or a custom error reporting mechanism for more informative feedback.
- **Documentation:** Generate Doxygen documentation to provide a clean and browsable API reference.

