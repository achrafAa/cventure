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
- Run exercises in hard mode (no hints): `make hard`
- Run exercises in normal mode (with hints): `make normal`
- Reset progress: `make reset`

### Configuration
Cventure uses a configuration file (`.config.json`) to store your preferences:

- Set hard mode as default: `make config-hard`
- Set normal mode as default: `make config-normal`

You can also manually edit `.config.json` to change your settings.

### Hint System
Cventure includes a smart hint system to help you learn:
- First attempt: No hints shown (encouraging you to try on your own)
- Second attempt: Detailed hints appear
- Hard mode: No hints are displayed, for an extra challenge

Hints are stored in separate files in the `hints` directory, following this structure:
```
hints/
  └── 1_hello/
      ├── 1_001_hello.hint
      ├── 1_002_welcome_message.hint
      └── ...
```

Each hint file contains detailed guidance about the corresponding exercise, including examples and explanations of the concepts involved.

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
- A hint file to help you along the way

## 🛠️ Development
- `make build`: Build the project
- `make clean`: Clean build artifacts

## 📝 License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🤝 Contributing
Contributions are welcome! Please feel free to submit a Pull Request.

## 🐛 Bug Reports
Found a bug? Please open an issue with a detailed description of the problem. 