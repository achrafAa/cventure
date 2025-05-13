# Contributing to Cventure 🐛

Thank you for your interest in contributing to Cventure! This document provides guidelines and instructions for contributing.

## 🎯 How to Contribute

### Adding New Exercises

1. **Structure**
   - Place new exercises in the appropriate category directory
   - Follow the naming convention: `chapter_number/chapter_number_exercise_number_description.c`
   - Example: `exercises/1_hello/1_001_hello.c`
   - Include a test file in the `tests` directory
   - Create a corresponding hint file in the `hints` directory

2. **Exercise Format**
   ```c
   /*
    * Exercise X: Title
    * 
    * Description of the exercise and what needs to be done
    */
   ```

3. **Hint Format**
   - Create a hint file at `hints/chapter_directory/exercise_name.hint`
   - Example: `hints/1_hello/1_001_hello.hint`
   - Hints should be clear, concise, and helpful without giving away the full solution
   - Include examples, explanations, and common mistakes to help users understand the concept
   - Avoid putting hints in the exercise files - keep all help content in the hint files

4. **Testing**
   - Each exercise should have a corresponding test in `tests/chapter_number/chapter_number_exercise_number_description_test.c`
   - Tests should verify both success and failure cases
   - Include clear error messages

### Creating Test Files

1. **Test File Structure**
   ```c
   #include "../../src/test_suite.h"
   #include <string.h>
   #include <stdio.h>
   #include <stdlib.h>

   // Default for linter only
   #ifndef __EXERCISE_FILE__
   #define __EXERCISE_FILE__ "exercises/chapter_number/chapter_number_exercise_number_description.c"
   #endif

   // Global variable to store program output
   static char program_output[1024];

   // Function to compile and run the exercise program
   static void test_program(void) {
       // Implementation details...
   }

   // Test functions
   TestResult test_specific_functionality(void) {
       test_program();
       // Verify expected behavior
       return (TestResult){ .passed = true };
   }

   // Entry point
   int main(void) {
       TestCase cases[] = {
           { .name = "Test description", .test_func = test_specific_functionality },
       };
       
       TestSuite suite = {
           .name = "Exercise Test Suite",
           .test_cases = cases,
           .test_count = sizeof(cases) / sizeof(TestCase)
       };
       
       run_test_suite(&suite);
       print_test_results(&suite);
       
       return suite.failed_count > 0 ? 1 : 0;
   }
   ```

2. **Registering Tests**
   - Update `src/exercises.h` to include the test suite path and name:
   ```c
   {
       "exercises/chapter_number/chapter_number_exercise_number_description.c",
       "Expected output",
       true,
       "tests/chapter_number/chapter_number_exercise_number_description_test.c",
       "Exercise Test Suite Name"
   },
   ```

3. **Chapter-based Progress System**
   - The system tracks progress by chapter number
   - Chapter numbers are extracted from the exercise path (e.g., `exercises/1_hello/...` is chapter 1)
   - Exercise numbers within a chapter are extracted from the filename (e.g., `1_001_hello.c` is exercise 1)
   - Adding new exercises to earlier chapters won't affect progress in later chapters

4. **Hint System**
   - Hints are stored in separate files in the `hints` directory
   - Hint files are named to match their corresponding exercise files
   - Hints follow the path pattern: `hints/chapter_directory/exercise_name.hint`
   - When adding a new exercise, always create a corresponding hint file
   - Make hints comprehensive with examples and explanations
   - The build.c file will automatically create empty hint files if they don't exist

5. **Configuration System**
   - The platform uses a `.config.json` file to store user preferences
   - Current configuration options:
     - `hard_mode`: Controls whether hints are displayed (true = no hints)
   - Configuration can be set via command line flags or make targets
   - Use the `read_config()` and `update_config()` functions to interact with the configuration

### Improving Existing Exercises

1. **Documentation**
   - Improve comments and hints
   - Add more detailed explanations
   - Fix typos or unclear instructions

2. **Code Quality**
   - Fix bugs
   - Improve code style
   - Add better error handling

## 🛠️ Development Workflow

1. Fork the repository
2. Create a new branch: `git checkout -b feature/your-feature`
3. Make your changes
4. Run tests: `make test`
5. Commit your changes: `git commit -m "Description of changes"`
6. Push to your fork: `git push origin feature/your-feature`
7. Create a Pull Request

## 📝 Code Style

- Follow the C style guide in the repository
- Use meaningful variable and function names
- Add comments for complex logic
- Keep exercises focused and concise

## 🐛 Bug Reports

When reporting bugs, please include:
- Description of the issue
- Steps to reproduce
- Expected behavior
- Actual behavior
- Environment details (OS, compiler version)

## 💡 Feature Requests

When suggesting features, please include:
- Description of the feature
- Use cases
- Implementation suggestions (if any)

## 🤝 Questions?

Feel free to open an issue for any questions or clarifications! 