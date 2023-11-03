# Assignment 2 | HY-345

## Stivaktakis Giorgos | csd4300

### Description
    A program that simulates students reading in a room with a limited number of seats.
    When a student wants to read, he/she enters the room and sits on an available seat.
    If there are no available seats, he/she waits until all student that are studying leave.
    When a student finishes reading, he/she leaves the room.
    Students AMs are inside the student_data.csv file.

### How to run
    make all   # Compile and create the executable
    ./threads  # Run the executable

### Extra features
    execute with -n flag to run with numbers instead of AMs (e.g. ./threads -n)

### How to clean
    make clean # Remove the executable and the object files