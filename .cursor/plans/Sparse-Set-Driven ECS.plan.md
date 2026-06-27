# Engineering Plan: Sparse-Set-Driven ECS (C++)

An ergonomic, high-performance, data-oriented ECS framework prioritizing O(1) component mutations, zero-query system execution, and native CPU vectorization (SIMD) via a hybrid SoA memory layout.

---

## [ ] Phase 1: Identity Foundation & Safe ID Recycling

**Goal:** Build a high-performance ID management layout that guarantees zero collisions in individual SoA tables when IDs are reused across frames.

* [ ] **1.1 Structure `Entity` Data Type**
* Implement `Entity` using bit-packing or a struct containing an `index: uint32_t` and a `generation: uint32_t` within a 64-bit integer space (`uint64_t`).
* Utilize a sentinel value (e.g., `0` or `std::numeric_limits<uint32_t>::max()`) to leverage memory layout optimization for optional configurations without extra boolean flags.


* [ ] **1.2 Build the `EntityManager` State**
* Implement a dense `std::vector<uint32_t>` tracker for active entities.
* Create a contiguous free-list array (`std::vector<uint32_t>`) to cache destroyed indices.


* [ ] **1.3 Implement the Allocation & Recycling Loop**
* Write `create_entity()` to fetch from the free-list or increment the index pointer if empty.
* Write `destroy_entity()` to push the dead index into the free-list and safely increment its `generation` counterpart.


* [ ] **1.4 Verification Tests**
* Validate generation ticking on massive spawn/despawn stress tests.



---

## [ ] Phase 2: Template Metaprogramming & Hybrid SoA Storages

**Goal:** Build the compile-time code generation pipeline that takes standard user structs and structures them into highly contiguous SoA data tables.

* [ ] **2.1 Initialize Compile-Time Metaprogramming Pipeline**
* Set up a modern C++ layout exploiting standard macros (e.g., X-macros), template metaprogramming, or a custom pre-build generator to unpack raw structures.


* [ ] **2.2 Implement Structured Layout Parsing**
* Write type-traits or multi-argument preprocessor templates to inspect fields, primitive types, and offsets of any targeted struct.


* [ ] **2.3 Code Gen: Separate Field-Level Slices (Hot Path)**
* Auto-generate a backend storage structure holding individual `std::vector<T_field>` fields instead of an array of structs.
* Expose raw, contiguous memory spans via `as_field_slice()` and mutable view variants using `std::span` (C++20).


* [ ] **2.4 Code Gen: Smart Reference Wrappers (UX Path)**
* Generate a custom proxy reference wrapper containing individual pointers or references to the separate SoA entries.
* Overload assignment operators and member accesses so developers can use clean code like `player.x = 5.0f` while mutating independent underlying field arrays.


* [ ] **2.5 Integration Proof**
* Ensure the layout outputs standard-layout and trivially copyable types that safely satisfy multi-threaded constraints (`std::is_standard_layout_v`, `std::is_trivially_copyable_v`).



---

## [ ] Phase 3: The World State & System-Level Sparse Sets

**Goal:** Connect global game logic files directly to entity IDs, ensuring O(1) random data access without dynamic engine queries.

* [ ] **3.1 Structure the `World` Hub**
* Create a centralized data container managing individual SoA tables securely using `std::type_index` and dynamic type-safe downcasting via `std::any` or internal pointer offsets.


* [ ] **3.2 Implement Sparse Set Indexing Inside Systems**
* Equip system classes (e.g., `PlayerControllerSystem`, `MovementSystem`) with a private `SparseSet` layout:
* `dense: std::vector<Entity>`: Keeps IDs packed sequentially for contiguous loops.
* `sparse: std::vector<size_t>`: Maps global `Entity` indices to their location inside the system's `dense` array.




* [ ] **3.3 Implement `register_entity` / `unregister_entity**`
* Write O(1) mutations for systems using the swap-and-pop pattern to ensure the `dense` ID array never leaves holes/null fragments for the CPU cache.


* [ ] **3.4 Write the Object Access API**
* Build the update pattern where the system loops over its own internal `dense` IDs and queries the `World` directly via absolute indexing.



---

## [ ] Phase 4: Job System & Component Dependency Scheduling

**Goal:** Run independent systems across multiple CPU worker threads safely without risking data races or memory corruption.

* [ ] **4.1 Parallelize Internal System Loops**
* Integrate standard multi-threading models, task systems, or parallel execution policies (such as `std::execution::par`).
* Transform the inner system execution loops from linear evaluation to parallelized loops over dense ID blocks.


* [ ] **4.2 Build the System Dependency Graph (DAG)**
* Create a resource registration structure where systems declare resource usage using access markers: `Read<Position>` or `Write<Position>`.


* [ ] **4.3 Design the Runtime Scheduler Execution**
* Build the execution kernel that reads the system graph:
* Group systems with overlapping `Read` parameters to fire simultaneously across threads.
* Queue systems requiring `Write` access to execute in isolation, ensuring thread-safe data synchronization.




* [ ] **4.4 Final Validation Benchmark**
* Profile cache-miss metrics and multi-threaded scaling comparing this Sparse-Set-Driven implementation against raw Object-Oriented layouts.