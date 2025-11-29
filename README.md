# C Multi-Client File Transfer System (TCP Sockets)

This project implements a simple **file transfer system** using **C**, **TCP sockets**, and **fork()** to support multiple clients at the same time.  
Clients can request a file by name, and the server sends it if it exists.

---

## 🚀 Features

### **Server**
- Handles multiple clients using `fork()`
- Streams file data in chunks
- Sends `"NOTFOUND"` when a file is missing
- Clean error logging

### **Client**
- Menu-driven interface
- Requests a file name from user
- Downloads and saves file locally
- Detects unavailable files

---
