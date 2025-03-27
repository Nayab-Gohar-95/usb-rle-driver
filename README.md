# USB RLE Compression Driver for Linux

## Overview
The **USB RLE Compression Driver** is a Linux kernel module that implements **Run-Length Encoding (RLE) compression and decompression** for data transfer over USB. This driver optimizes file transmission by compressing data before sending and decompressing it upon reception, enhancing performance and reducing bandwidth usage.

## Features
- **RLE Compression:** Compresses data before sending it over USB to minimize data size.
- **RLE Decompression:** Automatically decompresses received data before writing it to the destination.
- **Plug-and-Play USB Support:** Works with USB storage devices for efficient file transfer.
- **Kernel Module Implementation:** Fully implemented as a Linux kernel module.
- **Logging & Debugging:** Provides logs for tracking compression ratios and debugging errors.

## What is RLE (Run-Length Encoding)?
Run-Length Encoding (RLE) is a simple lossless data compression algorithm that replaces consecutive repeating characters with a single character followed by its count. For example:

**Uncompressed:** `AAABBBCCCCDDDD`
**Compressed:** `A3B3C4D4`

This technique is highly efficient for data with repetitive patterns, reducing file sizes and improving transmission speed.

## Installation & Setup
### Prerequisites
- **Linux Kernel Headers:** Ensure you have kernel headers installed for building the module.
- **GCC and Make:** Required for compiling the kernel module.

### Steps to Install
1. Clone the repository:
   ```sh
   git clone https://github.com/yourusername/usb-rle-driver.git
   cd usb-rle-driver
   ```
2. Build the kernel module:
   ```sh
   make
   ```
3. Load the driver into the kernel:
   ```sh
   sudo insmod usb_rle_driver.ko
   ```
4. Verify the module is loaded:
   ```sh
   lsmod | grep usb_rle_driver
   ```

## Usage
1. **Connect a USB device** to the system.
2. **Write a file to the USB** (it will be compressed automatically):
   ```sh
   cp testfile.txt /mnt/usb/
   ```
3. **Read a file from the USB** (it will be decompressed automatically):
   ```sh
   cat /mnt/usb/compressedfile.txt
   ```

## Uninstallation
To remove the module from the kernel:
```sh
sudo rmmod usb_rle_driver
```

## Testing
1. **Check kernel logs for compression statistics:**
   ```sh
   dmesg | tail -n 20
   ```
2. **Compare file sizes before and after transfer** to verify compression efficiency.

## Future Improvements
- **Adaptive Compression**: Dynamically switch compression algorithms based on data type.
- **Multi-threaded Processing**: Improve performance for large files.

