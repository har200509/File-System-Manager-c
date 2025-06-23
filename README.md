# 🗂️ File Management System in C

A comprehensive file management system built with low-level C system calls. Demonstrates advanced file I/O operations, directory manipulation, and system programming concepts.

## ✨ Features

- **File Operations**: Create, read, write, delete, copy, move, rename
- **Directory Management**: List files, display attributes, traverse directories  
- **Permission Control**: Set and modify file permissions
- **Smart Sorting**: Sort files by size or modification time
- **Error Handling**: Robust error checking with detailed feedback

## 🚀 Quick Start

```bash
gcc -o file_manager file_manager.c
./file_manager
```

## 🔧 Core Functions

```c
// File Operations
void createFile(const char *filename);
void readFile(const char *filename);
void writeFile(const char *filename, const char *content);
void deleteFile(const char *filename);
void copyFile(const char *source, const char *destination);

// Directory & Attributes
void listFiles(const char *directory);
void displayFileAttributes(const char *filename);
void setFilePermissions(const char *filename, mode_t mode);

// Sorting
void sortFiles(const char *directory, int (*compare)(const void*, const void*));
```

## 💡 Technical Highlights

- **Low-level System Calls**: Uses `open()`, `read()`, `write()` instead of stdio
- **POSIX Compliant**: Works on Linux, macOS, and Unix systems
- **Memory Efficient**: Dynamic allocation with proper cleanup
- **Extensible Sorting**: Function pointer-based comparison system

## 📋 Requirements

- POSIX-compliant system (Linux/macOS/Unix)
- GCC or C99-compatible compiler
- Standard system headers

## 🎯 Use Cases

- System programming education
- File backup and synchronization tools
- Log file management and organization
- Embedded system file operations
