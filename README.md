[![Build Status](https://travis-ci.com/tomb7890/talpc.svg?branch=master)](https://travis-ci.com/tomb7890/talpc) 


## C++ Coding Challenges

This repository showcases my solutions to two C++ programming challenges, focusing on data representation, manipulation, and file parsing. 

### 1. Fixed Point Class

**Challenge:** Implement a C++ class to represent and perform operations on fixed-point numbers using a custom 32-bit encoding.

**Features:**

- Constructors for creating fixed-point objects from floating-point values and packed 32-bit integers.
- Conversion to string representation (e.g., "1.234").
- Conversion to the nearest floating-point equivalent.

**Implementation Details:** 
- See comments within `FixedPoint.h` and `FixedPoint.cpp` for detailed explanations of design choices, algorithms, and potential optimizations.

### 2. Text File Parser

**Challenge:** Develop a C++ class to parse and modify configuration files with a specific section-based format.

**Features:**

- File loading and parsing.
- Retrieval of string, integer, and floating-point values by section and key.
- Setting and writing new values for specific keys, preserving the original formatting.

**Implementation Details:**
- Refer to `ConfigParser.h` and `ConfigParser.cpp` for in-depth comments on error handling, parsing logic, and file writing strategies.

###  Future Improvements

- **Unit Tests:** Implement comprehensive unit tests using a framework like Google Test or Catch2 to ensure code correctness and robustness.
- **Error Handling:** Improve error handling by using exceptions or a custom error reporting mechanism for more informative feedback.
- **Documentation:** Generate Doxygen documentation to provide a clean and browsable API reference.

###  About Me

[Your Name/Brief Introduction] - I'm passionate about writing clean, efficient, and well-documented C++ code.  

---
