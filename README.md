# BKMove — Assignment 2 Student Starter

The project layout follows BKMove Assignment 1. The editable files are in the root along with the public API, demo, Makefile and README; provided framework dependencies are grouped in `support/`.

| Part | File to implement | Public test |
|---|---|---|
| Q1a — BST (2 pts) | `BST.h` | `make q1a` |
| Q1b — AVL (2 pts) | `AVL.h` | `make q1b` |
| Q2 — Heap (2 pts) | `Heap.h` | `make q2` |
| Q3 — Bus Stop Finder (2 pts) | `BKMove.cpp` | `make q3` |
| Q4 — Live Arrival Board (2 pts) | `BKMove.cpp` | `make q4` |

Run from `init/`:

```sh
make           # Show help, like Assignment 1
make demo      # Compile demo, including unresolved TODO placeholders
make q1a      # 9 independent BST cases; TODO starter is expected to fail
make q3 TEST=4 # Only case 4 for Bus Stop Finder
make all      # 51 named public cases across five parts
```

## What a BKMove object stores

Each `BKMove` instance starts empty and must retain its own state between method calls. There are **two logical groups of data**, not two prescribed STL containers:

1. **Registered stops (Q3):** `StopInfo` records, each identified by a unique `stopId`. They can be searched by `stopId` or by a prefix of `searchKey` (with pagination). `getStopCount()` reports how many remain.
2. **Pending arrivals (Q4):** `ArrivalEvent` records belonging to registered stops. A pending `eventId` is unique across the entire BKMove instance. Events remain pending until `processNextArrival` or `cancelArrival` removes them; `peekNextArrival` and `rescheduleArrival` do not remove them.

`removeStop(stopId)` must reject a stop with **one or more pending arrivals at that stop**. Arrivals at a different stop do not prevent removal. These are the observable rules; students choose the storage/indexes that meet the performance requirements.

**BKMove state and private members:** Each `BKMove` object begins with no registered stops and no pending arrival events. Its methods must maintain those two groups of data across calls. The `private` section of `BKMove.h` is intentionally left **without any predefined member variables**:

```cpp
private:
    // Q3/Q4: Students propose their own private data members and helpers.
```

Students choose their own data members and any necessary helper types or methods. They may add classes or structs, derive new classes from BST/AVL/Heap, or extend the existing structures, provided the required public APIs and complexity constraints are preserved. **No particular internal design is required.** Students may edit the `private` section of `BKMove.h` and internal implementations in the allowed files; the public method signatures and the original fields of `StopInfo` and `ArrivalEvent` must remain unchanged. Additional record fields are permissible only if the documented initialization and API behavior are preserved. The constructor/destructor stubs in `BKMove.cpp` may be adapted when the chosen representation requires it. Each `BKMove` instance manages its own state and resources; no shared global storage.

**Files for assessment:** Submit exactly `BST.h`, `AVL.h`, `Heap.h`, `BKMove.h`, and `BKMove.cpp`, subject to the final LMS instructions. Place every additional helper class, struct, or method within these five files; no sixth header/source file is accepted. `support/`, `main.cpp`, tests, and `Makefile` are provided infrastructure and are not editable submissions.

The framework uses pointer-valued BST/AVL nodes with ownership-transfer semantics for `remove`. Be careful not to double-delete a stored value. `Heap<T>` comes from the original teaching framework; its copy constructor copies stored values (including pointers) but not pointed-to objects. Avoid copying pointer-owning heaps. No duplicate `Heap (copy).h` is distributed.

This package includes only the starter code, public tests, and conceptual design questions. It does not contain instructor solutions or private tests. The conceptual hints and build/debug instructions are included below.


---

# Design Considerations (student-facing)

These prompts are meant to help you understand the **problem** without prescribing a solution.

## Q3 — Bus Stop Finder

An AVL tree supports efficient insertion, deletion, and searching by key. However, finding a bus stop by its name is different from jumping directly to an arbitrary page of search results.

Consider what happens if the system must skip thousands of stops before reaching the requested page. Can the tree avoid visiting every preceding stop?

You may extend the internal representation of the provided data structures, provided that their required behavior is preserved.

## Q4 — Live Arrival Board

A binary heap efficiently maintains the next arriving bus. However, real-time updates introduce another challenge: a bus may be delayed or its scheduled arrival may be canceled.

Consider how the system can locate a particular arrival event without examining every event in the heap, and how its ordering should be maintained after an update.

You are free to design additional internal mechanisms as long as the required operations and performance constraints are satisfied.

---

# Build and debugging

The project uses the same Makefile interface and per-question tests as Assignment 1.

```sh
make           # Show help
make demo      # Build the starter demo (compiles before TODOs are completed)
./.build/demo
make q1a       # BST: 9 named public cases
make q1b       # AVL: 10 named public cases
make q2        # Heap: 10 named public cases
make q3        # Bus Stop Finder: 10 named public cases
make q4        # Live Arrival Board: 12 named public cases
make q3 TEST=4 # Run only case 4 for Bus Stop Finder
make check-rules # Check for obvious disallowed STL containers/heap calls
make all       # Run 51 public cases
make clean
```

Public cases have independent names and PASS/FAIL messages. Running all cases continues after an ordinary failed assertion, but a crash or memory error can terminate a test executable. `TEST` numbers start at 1; invalid numbers are errors, not passes. The starter is expected to fail cases whose TODOs are incomplete. Public cases are examples, not the entire private grading suite. The source rule check is only basic feedback, not proof of algorithmic compliance.

To debug memory errors after completing the algorithms:

```sh
g++ -std=c++17 -O1 -g -Wall -Wextra -fsanitize=address,undefined \
  -fno-omit-frame-pointer -I. BKMove.cpp tests/q4/test_q4.cpp -o .build/test_q4_asan
./.build/test_q4_asan
```

Build on Linux/macOS with a compiler supporting C++17. All student-editable files remain in the `init/` root. Provided framework headers live in `support/`; public tests live in `tests/`.

---

# Assessment & source rules

| Part | Points | What is graded |
|---|---:|---|
| Q1a BST | 2 | Algorithm correctness, invariants and ownership |
| Q1b AVL | 2 | Correctness, AVL balance and logarithmic operations |
| Q2 Heap | 2 | Correctness, comparator and O(m) heapify |
| Q3 Stop Finder | 2 | 1 point functional correctness + 1 point required performance |
| Q4 Arrival Board | 2 | 1 point functional correctness + 1 point required performance |

Performance marks require meaningful functional correctness. A passing public test does not establish complexity. Performance is evaluated against the documented asymptotic bounds, supported by implementation review and private scale tests; timing alone is not a proof.

`make check-rules` checks obvious uses of banned STL associative containers, priority queues and STL heap functions in the five editable files. It allows `std::vector` and `std::string`. This is not a definitive academic-integrity or performance check.

**Dependencies:** BST is the base for AVL; Stop Finder may reuse BST/AVL, and Live Arrival Board uses the directory of stops established by Q3 and may reuse Heap. The public tests use your submitted components, so unfinished prerequisite components may cause dependent test failures.

**Submission:** Keep the BKMove public API and supplied support code unchanged. The assessed deliverables are `BST.h`, `AVL.h`, `Heap.h`, `BKMove.h` and `BKMove.cpp`; only the private section of `BKMove.h` may be changed; place your implementation helpers inside these files. The instructor will announce the submission channel, deadline and archive naming rules separately.
