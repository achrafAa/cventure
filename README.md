# Cventure

A playful way to learn C programming through interactive exercises. Each exercise is designed to teach you a specific concept while having fun fixing bugs and completing challenges.

## 🚀 Getting Started

### Prerequisites
- GCC or Clang compiler
- Make
- Basic understanding of terminal usage

### Installation
1. Clone the repository:
```bash
git clone https://github.com/achrafAa/cventure.git
cd cventure
```

2. Build the project:
```bash
make
```

## 🎯 How to Use

### Running Exercises
- Run all exercises: `make`
- Run exercises for a specific chapter: `make chapter=1`
- Reset progress: `make reset`

### Progress Tracking
Cventure tracks your progress by chapter, allowing you to:
- Complete exercises in any order
- Skip ahead to later chapters
- Return to earlier chapters to try new exercises
- Continue from where you left off

### 📚 Cheatscroll
The `cheatscroll` directory contains quick reference guides for each topic:
- [`1_hello.md`](cheatscroll/1_hello.md): Hello World and basic output


These guides provide quick reference material and examples to help you complete the exercises.

## 🎮 Exercise Structure
Each exercise consists of:
- A source file with a bug or incomplete code
- A test to verify your solution
- Hints to help you along the way

## 🛠️ Development
- `make build`: Build the project
- `make clean`: Clean build artifacts

## 📝 License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🤝 Contributing
Contributions are welcome! Please feel free to submit a Pull Request.

## 🐛 Bug Reports
Found a bug? Please open an issue with a detailed description of the problem. 