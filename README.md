# 🪙 Personal Money Tracker

A robust, terminal-based **Personal Money Tracker** written in C. It implements fundamental data structures—including **Linked Lists**, **Stacks**, and **Queues**—to deliver a functional finance management utility with local file persistence.

---

## 🚀 Key Features

* **Interactive Transaction Management:** Add money (income) and spend money (expense) with real-time balance tracking.
* **Stack-Based Undo System:** Revert your last transaction instantly. Reverts updates to both your balance and budget records.
* **Queue-Based Payment Reminders:** Store, view, pay, or dismiss upcoming bills and reminders in a First-In-First-Out (FIFO) queue.
* **Persistent Storage:** Saves all configurations, transaction histories, queue states, and custom categories automatically to `tracker_data.txt` upon exit and restores them on launch.
* **Custom Categories:** Add and track custom budget categories dynamically.
* **Smart Budget Suggestions:** Set a monthly budget and get suggestions detailing where you can cut down on spending.

---

## 🛠️ Data Structures Used

This project demonstrates practical use cases of core computer science data structures:
1. **Linked List:** Stores full transaction history dynamically, keeping memory usage optimal while maintaining chronological order.
2. **Stack:** Tracks transaction IDs sequentially to facilitate the **Undo Last Transaction** functionality.
3. **Queue:** Implements a FIFO queue to manage payment reminders, ensuring you pay your oldest outstanding bills first.
4. **Arrays:** Manages default and custom expense categories dynamically.

---

## 💻 How to Compile and Run

Make sure you have a C compiler like `gcc` installed on your system.

### 1. Compile the program
```bash
gcc "Personal Money Tracker.c" -o tracker.exe
```

### 2. Run the executable
* **Windows:**
  ```cmd
  .\tracker.exe
  ```
* **macOS / Linux:**
  ```bash
  ./tracker
  ```

---

## 📂 Data Format

Data is stored locally in a text file named `tracker_data.txt` in the following structure:
* **Line 1:** `[Current Balance] [Monthly Budget] [Total Spent] [Next Transaction ID]`
* **Line 2:** `[Category Count]`
* **Subsequent Lines:** Category names, one per line.
* **Next Line:** `[Transaction Count]`
* **Subsequent Lines:** List of transactions formatted as `ID|Type|Category|Amount|Current Balance`
* **Next Line:** `[Stack Top Index]`
* **Subsequent Lines:** Transaction IDs currently in the undo stack.
* **Next Line:** `[Queue Front Index] [Queue Rear Index]`
* **Subsequent Lines:** Reminders formatted as `Note|Amount`

---

## 📜 License
This project is open-source and available under the MIT License.
