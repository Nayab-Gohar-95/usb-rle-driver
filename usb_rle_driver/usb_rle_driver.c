#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/string.h>

#define MAX_COMPRESSED_SIZE 1024  // Adjust this as necessary

// Function prototypes
char *rle_compress(const char *input, char *output);
ssize_t write_file(const char *file_path, const char *compressed_data);

// Function to compress data using Run-Length Encoding (RLE)
char *rle_compress(const char *input, char *output) {
    int i = 0, j = 0;
    printk(KERN_INFO "Starting RLE compression\n");
    while (input[i] != '\0') {
        char current_char = input[i];
        int count = 1;
        while (input[i + 1] == current_char) {
            count++;
            i++;
        }
        output[j++] = current_char;
        output[j++] = count + '0';  // Convert count to character
        i++;
    }
    output[j] = '\0';  // Null-terminate the string
    printk(KERN_INFO "RLE compression completed: %s\n", output);
    return output;
}

// Function to write compressed data to a file in kernel space
ssize_t write_file(const char *file_path, const char *compressed_data) {
    struct file *file;
    ssize_t bytes_written;

    printk(KERN_INFO "write_file called with data: %s\n", compressed_data);

    // Open the file
    file = filp_open(file_path, O_WRONLY | O_CREAT, 0644);
    if (IS_ERR(file)) {
        printk(KERN_ERR "Failed to open file: %ld\n", PTR_ERR(file));
        return PTR_ERR(file);
    }

    // Write the compressed data
    bytes_written = kernel_write(file, compressed_data, strlen(compressed_data), &file->f_pos);

    if (bytes_written < 0) {
        printk(KERN_ERR "Failed to write to file\n");
    } else {
        printk(KERN_INFO "Successfully wrote %ld bytes\n", bytes_written);
    }

    // Write a marker to verify function execution
    kernel_write(file, "Hello", strlen("Hello"), &file->f_pos);

    // Close the file
    filp_close(file, NULL);
    return bytes_written;
}

// USB probe function (called when a USB device is detected)
static int usb_probe(struct usb_interface *interface, const struct usb_device_id *id) {
    printk(KERN_INFO "USB device detected, initiating compression\n");

    // Define a buffer for compressed data
    char compressed_data[MAX_COMPRESSED_SIZE];
    char *file_data = "How are you?.";  // Example data to compress
    char *file_path = "/mnt/testfile.txt";  // Path to the file where compressed data will be written

    // Perform RLE compression
    char *compressed = rle_compress(file_data, compressed_data);
    printk(KERN_INFO "Compressed data: %s\n", compressed);

    // Write the compressed data to a file
    if (write_file(file_path, compressed) >= 0) {
        printk(KERN_INFO "File written successfully\n");
    } else {
        printk(KERN_ERR "Failed to write the file\n");
    }

    return 0;
}

// USB disconnect function (called when a USB device is removed)
static void usb_disconnect(struct usb_interface *interface) {
    printk(KERN_INFO "USB device disconnected\n");
}

// Define USB device ID table
static const struct usb_device_id usb_table[] = {
    { USB_DEVICE(0x23a5, 0x5786) }, // Example device ID for the USB disk
    { } // Terminating entry
};

MODULE_DEVICE_TABLE(usb, usb_table);

// Define USB driver
static struct usb_driver usb_driver = {
    .name = "usb_rle_driver",
    .id_table = usb_table,
    .probe = usb_probe,
    .disconnect = usb_disconnect,
};

// Module initialization function
static int __init usb_driver_init(void) {
    int result = usb_register(&usb_driver);
    if (result) {
        printk(KERN_ERR "usb_register failed with error %d\n", result);
        return result;
    }
    printk(KERN_INFO "USB driver registered\n");
    return 0;
}

// Module cleanup function
static void __exit usb_driver_exit(void) {
    usb_deregister(&usb_driver);
    printk(KERN_INFO "USB driver unregistered\n");
}

module_init(usb_driver_init);
module_exit(usb_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("USB RLE Compression Driver");
