# Contributing to Cventure 🐛

Thank you for your interest in contributing to Cventure! This document provides guidelines and instructions for contributing.

## 🎯 How to Contribute

### Adding New Exercises

1. **Structure**
   - Place new exercises in the appropriate category directory
   - Follow the naming convention: `XXX_description.c`
   - Include a test file in the `tests` directory

2. **Exercise Format**
   ```c
   /*
    * Exercise X: Title
    * 
    * Description of the exercise and what needs to be done
    * 
    * Hints:
    * - Hint 1
    * - Hint 2
    */
   ```

3. **Testing**
   - Each exercise should have a corresponding test in `tests/category/XXX_description_test.c`
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
   #define __EXERCISE_FILE__ "path/to/exercise.c"
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
       "exercises/category/XXX_description.c",
       "Expected output",
       "Hint for the exercise",
       true,
       "tests/category/XXX_description_test.c",
       "Exercise Test Suite Name"
   },
   ```

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