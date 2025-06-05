# System Resource Monitor and Logger

A sophisticated system monitoring tool developed in C that tracks and logs various system resources in real-time. This project demonstrates practical implementation of operating system concepts and system programming.

## Features

- **CPU Monitoring**
  - User time tracking
  - System time analysis
  - Idle time monitoring
  - I/O wait time tracking
  - IRQ and SoftIRQ statistics

- **Memory Management**
  - RAM usage monitoring
  - Swap space tracking
  - Memory allocation statistics

- **Disk Usage**
  - Total disk space monitoring
  - Free space tracking
  - Used space analysis

- **Process Information**
  - Load averages (1, 5, and 15 minutes)
  - Running tasks count
  - Total tasks monitoring
  - Process ID tracking

- **Network Statistics**
  - Bandwidth usage monitoring
  - Data received/sent tracking
  - Interface-specific statistics

- **Cloud Integration**
  - Automated log upload to Dropbox
  - Secure file transfer
  - Persistent storage

## Prerequisites

Before running this project, ensure you have the following installed:

- GCC compiler
- libcurl development library
- Linux operating system (for /proc filesystem access)
- Dropbox API token

## Installation

1. Clone the repository:
   ```bash
   git clone [your-repository-url]
   cd [repository-name]
   ```

2. Install required dependencies:
   ```bash
   sudo apt-get update
   sudo apt-get install libcurl4-openssl-dev
   ```

3. Configure Dropbox token:
   - Open `logger.c`
   - Replace `DROPBOX_TOKEN` with your Dropbox API token

4. Compile the program:
   ```bash
   gcc logger.c -o logger -lcurl
   ```

## Usage

1. Run the program:
   ```bash
   ./logger
   ```

2. The program will:
   - Create a `system_usage.log` file
   - Monitor system resources every 60 seconds
   - Upload logs to Dropbox automatically

3. To stop the program, press `Ctrl+C`

## Log File Format

The log file (`system_usage.log`) contains the following information:

```
[Timestamp]
---------------------------------
CPU Usage:
    - User Time:       [ticks] (HH:MM:SS)
    - System Time:     [ticks] (HH:MM:SS)
    - Idle Time:       [ticks] (HH:MM:SS)
    ...

Memory Usage:
    - Total Memory:   [MB]
    - Free Memory:    [MB]

Disk Usage:
    - Total Disk:     [GB]
    - Used Disk:      [GB]
    - Free Disk:      [GB]

Process Info:
    - Load Averages:  [1min] [5min] [15min]
    - Running Tasks:  [count]
    - Total Tasks:    [count]

Network Usage:
    - Received: [bytes]
    - Sent:     [bytes]

Swap Usage:
    - Total Swap:     [MB]
    - Free Swap:      [MB]
---------------------------------
```

## Project Structure

```
.
├── logger.c          # Main source code
├── system_usage.log  # Generated log file
└── README.md         # This file
```

## Technical Details

- Written in C programming language
- Uses Linux /proc filesystem for system information
- Implements libcurl for Dropbox API integration
- Features modular design with separate functions for different monitoring aspects
- Implements error handling and logging mechanisms

## Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- Linux kernel documentation for /proc filesystem
- libcurl documentation
- Dropbox API documentation
